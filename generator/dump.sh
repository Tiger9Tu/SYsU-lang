( export PATH=/workspace/sysu/bin:$PATH   CPATH=/workspace/sysu/include:$CPATH   LIBRARY_PATH=/workspace/sysu/lib:$LIBRARY_PATH  
 LD_LIBRARY_PATH=/workspace/sysu/lib:$LD_LIBRARY_PATH &&   clang -E /workspace/SYsU-lang/$1 |   clang -cc1 -ast-dump$2 | sed '2,15d')
