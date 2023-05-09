( export PATH=$HOME/sysu/bin:$PATH   CPATH=$HOME/sysu/include:$CPATH   LIBRARY_PATH=$HOME/sysu/lib:$LIBRARY_PATH  
 LD_LIBRARY_PATH=$HOME/sysu/lib:$LD_LIBRARY_PATH &&   clang -E /workspaces/SYsU-lang/$1 |   clang -cc1 -ast-dump$2 | sed '2,15d')
