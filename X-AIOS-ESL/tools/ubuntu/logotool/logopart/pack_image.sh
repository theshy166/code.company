#!/bin/bash
declare -a files=()
while IFS= read -r -d '' file; do
    files+=("$file")
done < <(find . -maxdepth 1 -type f -name "*.jpg" -print0)

if [ ${#files[@]} -eq 0 ]; then
    echo "No .jpg files found in the current directory."
    exit 1
fi

pack_args=""
#pack_args+="param.txt"

for file in "${files[@]}"; do
    pack_args+=" $file"
done

pack_command="./resource_tool --image=logo.img --pack ${pack_args}"
echo "Executing command: ${pack_command}"
eval "$pack_command"

exit 0
