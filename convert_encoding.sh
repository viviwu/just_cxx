#!/bin/bash

# 递归遍历当前路径下的文本文件
find . -type f \( -name "*.txt" -o -name "*.h" -o -name "*.cpp" -o -name "*.c" \) | while read -r file; do

    # 检查文件编码是否为UTF-8
    file_encoding=$(file -b --mime-encoding "$file")

    if [[ $file_encoding != "utf-8" ]]; then
        echo "Converting file: $file"

        # 临时文件路径
        temp_file="${file}.temp"

        # 尝试以gb18030编码读取文件内容，并以UTF-8编码保存到临时文件
        if iconv -f gb18030 -t utf-8 "$file" > "$temp_file" 2>/dev/null; then
            # 替换原文件内容为UTF-8编码内容
            mv "$temp_file" "$file"
            echo "Converted successfully."
        else
            # 无法以gb18030编码读取文件内容，忽略
            echo "Ignoring file due to unsupported encoding."
            rm "$temp_file"  # 删除临时文件
        fi
    fi

done
