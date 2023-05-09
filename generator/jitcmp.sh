printf "my JIT output:\n"
( export PATH=$HOME/sysu/bin:$PATH   CPATH=$HOME/sysu/include:$CPATH  
 LIBRARY_PATH=$HOME/sysu/lib:$LIBRARY_PATH   LD_LIBRARY_PATH=$HOME/sysu/lib:$LD_LIBRARY_PATH &&   
clang -E /home/tiger9tu/SYsU-lang/$1  |  
 clang -cc1 -ast-dump=json |  sysu-generator |   lli --load=libsysy.so --load=libsysu.so )

printf "\nclang JIT output:\n"
( export PATH=$HOME/sysu/bin:$PATH   CPATH=$HOME/sysu/include:$CPATH  
 LIBRARY_PATH=$HOME/sysu/lib:$LIBRARY_PATH   LD_LIBRARY_PATH=$HOME/sysu/lib:$LD_LIBRARY_PATH &&   
clang -E /home/tiger9tu/SYsU-lang/$1  | clang -cc1 -O0 -S -emit-llvm |  lli --load=libsysy.so --load=libsysu.so )