#!/usr/bin/env fish
#
# จัด PDF ที่โหลดมือมาแบบลอย ๆ ให้เข้าโฟลเดอร์เป็นระเบียบ
#
#   fish tools/organize.fish docs/sheets
#   fish tools/organize.fish docs/sheets --cpp      # สร้าง .cpp จาก template ด้วย
#   fish tools/organize.fish docs/sheets --dry-run  # ดูเฉย ๆ ยังไม่ย้าย
#   fish tools/organize.fish docs/sheets --prefix 20261005_AM
#
# กติกา: ไฟล์ "โจทย์ สอวน (th).pdf" → โฟลเดอร์ "โจทย์_สอวน/โจทย์_สอวน.pdf"
#        ตัดวงเล็บทิ้ง แทนเว้นวรรคด้วย _ (ชื่อไทยเก็บไว้ครบ ไม่ตัดทิ้ง)
#        ถ้าชื่อไฟล์เป็นรหัสโจทย์ที่มีใน tools/scores/*.txt จะได้โฟลเดอร์ชื่อ
#        "<ลำดับ>_<รหัส>_<ชื่อโจทย์>" เช่น C1PC01 (th).pdf → 01_C1PC01_Alice_and_Bob/C1PC01.pdf
#        (ไฟล์ข้างในคงชื่อรหัส เพราะ grader บังคับชื่อไฟล์ตอน submit)
#        ไม่มีไฟล์คะแนนก็ยังจัดได้ — ใช้ชื่อไฟล์เป็นชื่อโฟลเดอร์ตามปกติ

argparse 'cpp' 'dry-run' 'prefix=' 'h/help' -- $argv
or exit 1

set -l script_dir (realpath (dirname (status filename)))
set -l root (realpath $script_dir/..)

if set -q _flag_h; or test (count $argv) -eq 0
    echo 'จัด PDF ที่ลอยอยู่ในโฟลเดอร์ ให้เข้าโฟลเดอร์ของตัวเอง'
    echo
    echo '  fish tools/organize.fish <โฟลเดอร์> [--cpp] [--dry-run] [--prefix 20261005_AM]'
    echo
    echo 'ตัวอย่าง:'
    echo '  fish tools/organize.fish docs/sheets --prefix 20261005_AM'
    exit 0
end

set -l target $argv[1]
if not string match -q '/*' -- $target
    set target $root/$target
end

if not test -d $target
    echo "❌ ไม่เจอโฟลเดอร์ $target"
    exit 1
end

set -l template $root/template.cpp
set -l prefix ''
if set -q _flag_prefix
    set prefix $_flag_prefix
end

# ชื่อโฟลเดอร์แบบมีลำดับ+ชื่อโจทย์ (01_C1PC01_Alice_and_Bob) มาจาก tools/scores/*.txt
# ผ่าน tools/folder_names.py — อ่านไม่ได้ก็ไม่เป็นไร ปล่อยว่างแล้วใช้ชื่อไฟล์เป็นชื่อโฟลเดอร์
# fish ไม่มี associative array จึงเก็บเป็นสอง list คู่กัน: รหัสที่ตำแหน่ง i ↔ ชื่อที่ตำแหน่ง i
set -l task_ids
set -l task_folders
set -l map_script $root/tools/folder_names.py
if test -f $map_script
    for line in (python3 $map_script --print-map 2>/dev/null)
        set -l parts (string split \t -- $line)
        if test (count $parts) -ge 2
            set -a task_ids $parts[1]
            set -a task_folders $parts[2]
        end
    end
end

echo "🚀 จัดระเบียบ PDF ใน $target"
echo "--------------------------------------------------"

set -l count 0
set -l skipped 0

for file in $target/*.pdf
    test -e "$file"; or continue

    set -l base (basename -s .pdf -- "$file")
    set -l clean (string replace -ra '\s*\([^)]*\)' '' -- $base)   # ตัด (th) (C++) ทิ้ง
    set clean (string replace -ra '\s+' '_' -- $clean)             # เว้นวรรค → _
    set clean (string trim -- $clean)

    if test -n "$prefix"
        set clean "$prefix"_"$clean"
    end

    if test -z "$clean"
        echo "⚠️  ข้าม: ชื่อไฟล์กลายเป็นว่าง («$base»)"
        set skipped (math $skipped + 1)
        continue
    end

    # โฟลเดอร์ใช้ชื่อจาก tools/scores/*.txt (01_C1PC01_Alice_and_Bob)
    # ส่วนไฟล์ข้างในยังชื่อตามรหัส (C1PC01.pdf / C1PC01.cpp) ตามที่ grader บังคับตอน submit
    set -l folder $clean
    set -l idx (contains -i -- $clean $task_ids)
    if test -n "$idx"
        set folder $task_folders[$idx]
    end
    set -l dir $target/$folder

    # มีโฟลเดอร์ของข้อนี้อยู่แล้วหรือยัง — เทียบด้วยรหัสที่ถอดจากชื่อโฟลเดอร์
    # (รองรับทั้งชื่อใหม่ 01_C1PC01_ชื่อโจทย์ และชื่อเก่าที่มีแต่รหัส)
    set -l existing
    for cand in $target/*
        test -d "$cand"; or continue
        set -l cand_id (string replace -r '^([0-9]+_)?([^_]+).*$' '$2' -- (basename $cand))
        if test "$cand_id" = "$clean"
            set -a existing (basename $cand)
        end
    end
    if test (count $existing) -gt 0
        echo "⚠️  $clean — มีโฟลเดอร์อยู่แล้ว: $existing (ไฟล์ $base.pdf ยังค้างอยู่ข้างนอก)"
        set skipped (math $skipped + 1)
        continue
    end

    if set -q _flag_dry_run
        echo "👀 $base.pdf → $folder/$clean.pdf"
        set count (math $count + 1)
        continue
    end

    mkdir -p $dir
    mv -- "$file" $dir/$clean.pdf
    echo "✅ $folder — ย้าย PDF เข้าโฟลเดอร์แล้ว"

    if set -q _flag_cpp
        if not test -e $dir/$clean.cpp
            if test -f $template
                cp $template $dir/$clean.cpp
                echo "   📝 สร้าง $clean.cpp จาก template.cpp"
            else
                touch $dir/$clean.cpp
                echo "   ⚠️  สร้าง $clean.cpp เปล่า ๆ (ไม่เจอ template.cpp)"
            end
        end
    end

    set count (math $count + 1)
end

echo "--------------------------------------------------"
if set -q _flag_dry_run
    echo "👀 จะย้าย $count ไฟล์ · ข้าม $skipped (โหมด --dry-run ยังไม่แตะอะไร)"
else
    echo "🎉 ย้าย $count ไฟล์ · ข้าม $skipped"
end
