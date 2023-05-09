
echo "my output:"
( export PATH=$HOME/sysu/bin:$PATH   CPATH=$HOME/sysu/include:$CPATH   LIBRARY_PATH=$HOME/sysu/lib:$LIBRARY_PATH   LD_LIBRARY_PATH=$HOME/sysu/lib:$LD_LIBRARY_PATH &&   clang -E /home/tiger9tu/SYsU-lang/$1 |   clang -cc1 -ast-dump=json |   sysu-generator $2)
printf "\n\n\n"
echo "clang output:"
( export PATH=$HOME/sysu/bin:$PATH   CPATH=$HOME/sysu/include:$CPATH   LIBRARY_PATH=$HOME/sysu/lib:$LIBRARY_PATH   LD_LIBRARY_PATH=$HOME/sysu/lib:$LD_LIBRARY_PATH && clang -E /home/tiger9tu/SYsU-lang/$1 | clang -cc1 -O0 -S -emit-llvm | sed '1,5d' )