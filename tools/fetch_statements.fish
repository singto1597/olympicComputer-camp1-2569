#!/usr/bin/env fish
#
# ดึงโจทย์ PDF จาก grader + สร้างโฟลเดอร์ + ไฟล์ .cpp จาก template ให้เลย
#
#   fish tools/fetch_statements.fish C1P01 C1P02 C1P03
#   fish tools/fetch_statements.fish --from-list
#   fish tools/fetch_statements.fish --dest problems/2_incamp/yrc-grader C1Y01
#
# ต้องมี tools/secrets.fish ก่อน:
#   cp tools/secrets.fish.example tools/secrets.fish   แล้วเติมค่า

argparse 'from-list' 'dest=' 'h/help' -- $argv
or exit 1

set -l script_dir (realpath (dirname (status filename)))
set -l root (realpath $script_dir/..)
set -l template $root/template.cpp

if set -q _flag_h
    echo 'ดึงโจทย์ PDF จาก grader พร้อมสร้างโฟลเดอร์และไฟล์ .cpp'
    echo
    echo '  fish tools/fetch_statements.fish C1P01 C1P02'
    echo '  fish tools/fetch_statements.fish --from-list          # อ่านจาก tools/tasks.txt'
    echo '  fish tools/fetch_statements.fish --dest problems/2_incamp/yrc-grader C1Y01'
    exit 0
end

# ---------- ปลายทาง ----------
set -l dest_rel problems/2_incamp/cmu-grader
if set -q _flag_dest
    set dest_rel $_flag_dest
end

set -l dest_dir $dest_rel
if not string match -q '/*' -- $dest_dir
    set dest_dir $root/$dest_rel
end

# ---------- อ่าน cookie / URL ----------
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
if test -z "$USER_COOKIE"
    echo "❌ ยังไม่ได้ใส่ USER_COOKIE ใน tools/secrets.fish"
    exit 1
end

set -l base (string replace -r '/+$' '' -- $GRADER_BASE_URL)

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

# ---------- ตัวช่วยเช็คว่าได้ PDF จริง ไม่ใช่หน้า login ----------
function is_valid_pdf -a path
    test -s "$path"; or return 1
    test (stat -c %s -- "$path") -ge 5120; or return 1
    string match -q '%PDF*' -- (head -c 4 -- "$path"); or return 1
    return 0
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
    set -l pdf $target_dir/$id.pdf
    set -l cpp $target_dir/$id.cpp

    mkdir -p $target_dir

    if is_valid_pdf "$pdf"
        echo "⏭️  $id — มี PDF อยู่แล้ว"
        set skipped (math $skipped + 1)
    else
        echo "⬇️  กำลังโหลด $id ..."
        curl -fsSL --retry 3 --retry-delay 2 \
            -H "Cookie: $USER_COOKIE" \
            -A "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36" \
            -o "$pdf" "$base/tasks/$id/statements/pdf"
        set -l curl_status $status

        if not is_valid_pdf "$pdf"
            set -l size 0
            if test -e "$pdf"
                set size (stat -c %s -- "$pdf")
                rm -f "$pdf"
            end
            if test $curl_status -ne 0
                echo "   ⚠️  โหลดไม่สำเร็จ (curl exit $curl_status)"
            else
                echo "   ⚠️  ได้ไฟล์ที่ไม่ใช่ PDF ($size B) — cookie อาจหมดอายุ หรือโจทย์ยังไม่เปิดให้ดู"
            end
            rmdir $target_dir 2>/dev/null   # เก็บโฟลเดอร์เปล่าที่เพิ่งสร้างทิ้ง
            set failed (math $failed + 1)
            continue
        end

        echo "   ✅ โหลดแล้ว"
        set ok (math $ok + 1)
    end

    if not test -e "$cpp"
        if test -f $template
            cp $template $cpp
            echo "   📝 สร้าง $id.cpp จาก template.cpp"
        else
            touch $cpp
            echo "   ⚠️  สร้าง $id.cpp เปล่า ๆ (ไม่เจอ template.cpp)"
        end
    end
end

echo "--------------------------------------------------"
echo "🎉 โหลดใหม่ $ok · มีอยู่แล้ว $skipped · ล้มเหลว $failed"
if test $failed -gt 0
    echo "   เคล็ดลับ: cookie ของ grader อายุสั้น — ก็อปใหม่จาก DevTools แล้ววางทับใน tools/secrets.fish"
end
