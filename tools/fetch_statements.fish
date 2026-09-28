#!/usr/bin/env fish
#
# ดึงโจทย์จาก grader + สร้างโฟลเดอร์ + ไฟล์ .cpp จาก template ให้เลย
#
#   fish tools/fetch_statements.fish C1PR01 C1PR02
#   fish tools/fetch_statements.fish --from-list
#   fish tools/fetch_statements.fish --force C1PR01          # โหลดทับของเดิม
#   fish tools/fetch_statements.fish --dest problems/2_incamp/yrc-grader C1Y01
#
# ต้องตั้งค่าใน tools/secrets.fish ก่อน — ดู tools/secrets.fish.example

argparse 'from-list' 'dest=' 'force' 'h/help' -- $argv
or exit 1

set -l script_dir (realpath (dirname (status filename)))
set -l root (realpath $script_dir/..)
set -l template $root/template.cpp
set -l jar $script_dir/.cookies.txt
set -l ua "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36"

if set -q _flag_h
    echo 'ดึงโจทย์จาก grader พร้อมสร้างโฟลเดอร์และไฟล์ .cpp'
    echo
    echo '  fish tools/fetch_statements.fish C1PR01 C1PR02'
    echo '  fish tools/fetch_statements.fish --from-list              # อ่านจาก tools/tasks.txt'
    echo '  fish tools/fetch_statements.fish --force C1PR01           # โหลดทับ'
    echo '  fish tools/fetch_statements.fish --dest problems/2_incamp/yrc-grader C1Y01'
    exit 0
end

# ---------- ปลายทาง ----------
set -l dest_rel problems/1_precamp/cmu-grader
if set -q _flag_dest
    set dest_rel $_flag_dest
end

set -l dest_dir $dest_rel
if not string match -q '/*' -- $dest_dir
    set dest_dir $root/$dest_rel
end

# ---------- อ่านค่าตั้งต้น ----------
set -l secrets $script_dir/secrets.fish
if not test -f $secrets
    echo "❌ ไม่เจอ tools/secrets.fish"
    echo "   เริ่มด้วย:  cp tools/secrets.fish.example tools/secrets.fish"
    exit 1
end
source $secrets

if test -z "$GRADER_BASE_URL"
    echo "❌ ยังไม่ได้ใส่ GRADER_BASE_URL ใน tools/secrets.fish"
    exit 1
end

set -l base (string replace -r '/+$' '' -- $GRADER_BASE_URL)
set -l auth_mode cookie
if set -q GRADER_AUTH
    set auth_mode $GRADER_AUTH
end

set -l stmt_path /tasks/{id}/statements/pdf
if set -q GRADER_STATEMENT_PATH
    set stmt_path $GRADER_STATEMENT_PATH
end

# ---------- ตัวช่วย ----------
function is_pdf -a path
    test -s "$path"; or return 1
    test (stat -c %s -- "$path") -ge 512; or return 1
    string match -q '%PDF*' -- (head -c 4 -- "$path" | string collect)
end

function is_html -a path
    test -s "$path"; or return 1
    string match -qi '*<html*' -- (head -c 1024 -- "$path" | string collect)
end

# เช็คว่า session ใน jar ยังใช้ได้ (หน้า contest ต้องมีคำว่า logout)
function cms_session_ok -a base jar
    test -f "$jar"; or return 1
    set -l page (mktemp)
    set -l code (curl -sS -b "$jar" -c "$jar" -o "$page" -w '%{http_code}' "$base" 2>/dev/null)
    set -l ok 0
    if test "$code" = 200; and grep -qi 'logout' "$page"
        set ok 1
    end
    rm -f "$page"
    return (math 1 - $ok)
end

# ล็อกอินแบบฟอร์ม CMS: GET หน้าแรก → เก็บ _xsrf → POST /login
function cms_login -a base jar
    if test -z "$GRADER_USER"; or test -z "$GRADER_PASS"
        echo "❌ โหมด cms ต้องมี GRADER_USER กับ GRADER_PASS ใน tools/secrets.fish" >&2
        return 1
    end

    set -l page (mktemp)
    curl -sS -c "$jar" -b "$jar" -o "$page" "$base" 2>/dev/null
    set -l xsrf (sed -n 's/.*name="_xsrf" value="\([^"]*\)".*/\1/p' "$page" | head -1)
    rm -f "$page"

    if test -z "$xsrf"
        echo "❌ ดึง _xsrf จากหน้า login ไม่ได้ — เช็ค GRADER_BASE_URL" >&2
        return 1
    end

    set -l code (curl -sS -c "$jar" -b "$jar" -o /dev/null -w '%{http_code}' \
        -X POST "$base/login" -H "Referer: $base" \
        --data-urlencode "username=$GRADER_USER" \
        --data-urlencode "password=$GRADER_PASS" \
        --data-urlencode "_xsrf=$xsrf" 2>/dev/null)

    if test "$code" != 302
        echo "❌ ล็อกอินไม่สำเร็จ (http $code) — เช็ค username/password" >&2
        return 1
    end
    return 0
end

function show_help_hint -a base
    echo "   เคล็ดลับ: เปิด $base ในเบราว์เซอร์แล้วล็อกอินก่อน"
    echo "            ถ้าเว็บเปลี่ยนไป ดูรหัสโจทย์ใหม่จากหน้า overview แล้วอัปเดต tools/tasks.txt"
end

# ---------- เตรียม auth ----------
set -l curl_auth

switch "$auth_mode"
    case cms
        if cms_session_ok $base $jar
            echo "🔑 ใช้ session เดิมใน tools/.cookies.txt"
        else
            echo "🔑 ล็อกอินเข้า $base ..."
            if not cms_login $base $jar
                exit 1
            end
            echo "   ✅ ล็อกอินแล้ว (เก็บ session ไว้ที่ tools/.cookies.txt)"
        end
        set curl_auth -b $jar -c $jar
    case cookie
        if test -z "$GRADER_COOKIE"
            echo "❌ โหมด cookie ต้องมี GRADER_COOKIE ใน tools/secrets.fish"
            exit 1
        end
        set curl_auth -H "Cookie: $GRADER_COOKIE"
    case none
        # ไม่ต้องยืนยันตัวตน
    case '*'
        echo "❌ GRADER_AUTH ไม่รู้จัก: $auth_mode (ใช้ cms / cookie / none)"
        exit 1
end

# ---------- รวมรหัสโจทย์ ----------
set -l ids $argv

if set -q _flag_from_list
    set -l list_file $script_dir/tasks.txt
    if not test -f $list_file
        echo "❌ ไม่เจอ $list_file"
        exit 1
    end
    for line in (cat $list_file)
        set -l id (string trim -- $line)
        if test -z "$id"; or string match -q '#*' -- $id
            continue
        end
        set -a ids $id
    end
end

if test (count $ids) -eq 0
    echo "❌ ไม่ได้ระบุรหัสโจทย์ — ใส่เป็น argument หรือใช้ --from-list"
    exit 1
end

# ---------- ลุย ----------
echo "🚀 ดึงโจทย์จาก $base"
echo "   ปลายทาง: $dest_dir"
echo "--------------------------------------------------"

set -l ok 0
set -l skipped 0
set -l failed 0

for id in $ids
    set -l target_dir $dest_dir/$id
    set -l cpp $target_dir/$id.cpp
    set -l have (find $target_dir -maxdepth 1 -name "$id.pdf" -o -maxdepth 1 -name "$id.html" 2>/dev/null | head -1)

    if test -n "$have"; and not set -q _flag_force
        echo "⏭️  $id — มี statement อยู่แล้ว"
        set skipped (math $skipped + 1)
        continue
    end

    mkdir -p $target_dir

    set -l url "$base"(string replace '{id}' $id -- $stmt_path)
    set -l code 0
    set -l curl_status 0
    set -l kind unknown
    set -l tmp (mktemp)
    set -l retried 0

    # ลูปนี้มีรอบเดียว ปกติ — รอบสองมีแค่ตอน session หมดอายุกลางทางแล้วล็อกอินใหม่สำเร็จ
    while true
        rm -f "$tmp"
        set tmp (mktemp)
        set code (curl -sSL --retry 3 --retry-delay 2 $curl_auth -A "$ua" \
            -o "$tmp" -w '%{http_code}' "$url" 2>/dev/null)
        set curl_status $status

        set kind unknown
        if test "$code" = 200
            if is_pdf "$tmp"
                set kind pdf
            else if is_html "$tmp"
                if grep -qi 'name="password"' "$tmp"
                    set kind login        # หน้า login โผล่มา = session หมดอายุ
                else
                    set kind html
                end
            end
        end

        if test "$kind" = login; and test "$auth_mode" = cms; and test $retried -eq 0
            echo "      🔑 session หมดอายุ — ล็อกอินใหม่"
            rm -f "$jar"
            set retried 1
            if cms_login $base $jar
                continue
            end
        end
        break
    end

    switch "$kind"
        case pdf
            mv -f "$tmp" $target_dir/$id.pdf
            echo "⬇️  $id — ได้ PDF แล้ว"
            set ok (math $ok + 1)
        case html
            # เว็บนี้บาง task ให้ statement เป็น HTML ไม่ใช่ PDF — เก็บไว้เปิดในเบราว์เซอร์
            mv -f "$tmp" $target_dir/$id.html
            echo "⬇️  $id — ได้เป็น HTML (ไม่ใช่ PDF) เก็บเป็น $id.html แทน"
            echo "      เปิดในเบราว์เซอร์แล้ว Print → Save as PDF ถ้าอยากได้ PDF"
            set ok (math $ok + 1)
        case login
            rm -f "$tmp"
            echo "   ⚠️  $id — ล็อกอินไม่ผ่าน ยังได้หน้า login กลับมา"
            show_help_hint $base
            rmdir $target_dir 2>/dev/null
            set failed (math $failed + 1)
            continue
        case '*'
            set -l size 0
            if test -e "$tmp"
                set size (stat -c %s -- "$tmp")
            end
            rm -f "$tmp"
            if test $curl_status -ne 0
                echo "   ⚠️  $id — โหลดไม่สำเร็จ (http $code, curl exit $curl_status)"
            else
                echo "   ⚠️  $id — ได้ไฟล์ชนิดที่อ่านไม่ออก ($size B, http $code)"
                echo "      อาจยังไม่ล็อกอิน หรือโจทย์ยังไม่เปิดให้ดู"
                show_help_hint $base
            end
            rmdir $target_dir 2>/dev/null   # เก็บโฟลเดอร์เปล่าที่เพิ่งสร้างทิ้ง
            set failed (math $failed + 1)
            continue
    end

    if not test -e "$cpp"
        if test -f $template
            cp $template $cpp
            echo "      📝 สร้าง $id.cpp จาก template.cpp"
        else
            touch $cpp
            echo "      ⚠️  สร้าง $id.cpp เปล่า ๆ (ไม่เจอ template.cpp)"
        end
    end
end

echo "--------------------------------------------------"
echo "🎉 โหลดใหม่ $ok · มีอยู่แล้ว $skipped · ล้มเหลว $failed"
if test $failed -gt 0
    show_help_hint $base
end
