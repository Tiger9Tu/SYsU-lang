export PATH=$HOME/sysu/bin:$PATH \
  CPATH=$HOME/sysu/include:$CPATH \
  LIBRARY_PATH=$HOME/sysu/lib:$LIBRARY_PATH \
  LD_LIBRARY_PATH=$HOME/sysu/lib:$LD_LIBRARY_PATH &&
  clang -E /workspace/SYsU-lang/optimizer/target/$1.c |
  clang -cc1 -ast-dump=json |
  sysu-generator > /workspace/SYsU-lang/optimizer/target/$1.my.ll

  export PATH=$HOME/sysu/bin:$PATH \
  CPATH=$HOME/sysu/include:$CPATH \
  LIBRARY_PATH=$HOME/sysu/lib:$LIBRARY_PATH \
  LD_LIBRARY_PATH=$HOME/sysu/lib:$LD_LIBRARY_PATH &&
  clang -E /workspace/SYsU-lang/optimizer/target/$1.c|
  clang -cc1 -ast-dump=json |
  sysu-generator | sysu-optimizer > /workspace/SYsU-lang/optimizer/target/$1.myopt.ll

 ( export PATH=$HOME/sysu/bin:$PATH \
  CPATH=$HOME/sysu/include:$CPATH \
  LIBRARY_PATH=$HOME/sysu/lib:$LIBRARY_PATH \
  LD_LIBRARY_PATH=$HOME/sysu/lib:$LD_LIBRARY_PATH &&
  clang -E /workspace/SYsU-lang/optimizer/target/$1.c |
  clang -cc1 -O0 -S -emit-llvm > /workspace/SYsU-lang/optimizer/target/$1.clo0.ll )

 ( export PATH=$HOME/sysu/bin:$PATH \
  CPATH=$HOME/sysu/include:$CPATH \
  LIBRARY_PATH=$HOME/sysu/lib:$LIBRARY_PATH \
  LD_LIBRARY_PATH=$HOME/sysu/lib:$LD_LIBRARY_PATH &&
  clang -E /workspace/SYsU-lang/optimizer/target/$1.c |
  clang -cc1 -O1 -S -emit-llvm > /workspace/SYsU-lang/optimizer/target/$1.clo1.ll )

  ( export PATH=$HOME/sysu/bin:$PATH \
  CPATH=$HOME/sysu/include:$CPATH \
  LIBRARY_PATH=$HOME/sysu/lib:$LIBRARY_PATH \
  LD_LIBRARY_PATH=$HOME/sysu/lib:$LD_LIBRARY_PATH &&
  clang -E /workspace/SYsU-lang/optimizer/target/$1.c |
  clang -cc1 -O2 -S -emit-llvm > /workspace/SYsU-lang/optimizer/target/$1.clo2.ll )

  ( export PATH=$HOME/sysu/bin:$PATH \
  CPATH=$HOME/sysu/include:$CPATH \
  LIBRARY_PATH=$HOME/sysu/lib:$LIBRARY_PATH \
  LD_LIBRARY_PATH=$HOME/sysu/lib:$LD_LIBRARY_PATH &&
  clang -E /workspace/SYsU-lang/optimizer/target/$1.c |
  clang -cc1 -O3 -S -emit-llvm > /workspace/SYsU-lang/optimizer/target/$1.clo3.ll )