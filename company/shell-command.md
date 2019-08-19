#1、循环内遍历双变量
```
im1_files=(~/prev1/*.png)
im2_files=(~/prev3/*.png)

for ((i=0;i<=${#im1_files[@]};i++)); do
   run_black.sh "${im1_files[i]}" "${im2_files[i]}"
done
```
