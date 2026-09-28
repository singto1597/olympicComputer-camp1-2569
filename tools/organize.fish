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

    set -l dir $target/$clean

    if test -d $dir
        echo "⚠️  $clean — มีโฟลเดอร์อยู่แล้ว (ไฟล์ $base.pdf ยังค้างอยู่ข้างนอก)"
        set skipped (math $skipped + 1)
        continue
    end

    if set -q _flag_dry_run
        echo "👀 $base.pdf → $clean/$clean.pdf"
        set count (math $count + 1)
        continue
    end

    mkdir -p $dir
    mv -- "$file" $dir/$clean.pdf
    echo "✅ $clean — ย้าย PDF เข้าโฟลเดอร์แล้ว"

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
