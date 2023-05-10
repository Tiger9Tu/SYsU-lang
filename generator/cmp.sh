
echo "my output:"
( export PATH=/workspace/sysu/bin:$PATH   CPATH=/workspace/sysu/include:$CPATH   LIBRARY_PATH=/workspace/sysu/lib:$LIBRARY_PATH   LD_LIBRARY_PATH=/workspace/sysu/lib:$LD_LIBRARY_PATH &&   clang -E /workspace/SYsU-lang/$1 |   clang -cc1 -ast-dump=json |   sysu-generator $2)
printf "\n\n\n"
echo "clang output:"
( export PATH=/workspace/sysu/bin:$PATH   CPATH=/workspace/sysu/include:$CPATH   LIBRARY_PATH=/workspace/sysu/lib:$LIBRARY_PATH   LD_LIBRARY_PATH=/workspace/sysu/lib:$LD_LIBRARY_PATH && clang -E /workspace/SYsU-lang/$1 | clang -cc1 -O0 -S -emit-llvm | sed '1,5d' )