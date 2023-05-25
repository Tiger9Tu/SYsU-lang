; ModuleID = '-'
source_filename = "generator/target/median2.sysu.my.ll"

@n = global i32 0
@a = global [10000000 x i32] zeroinitializer

declare void @_sysy_starttime(i32)

declare void @_sysy_stoptime(i32)

declare i32 @_sysy_getarray(i32*)

declare void @_sysy_putarray(i32, i32*)

define i32 @meanless_calculation(i32 %0, i32 %1) {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %y = alloca i32, align 4
  store i32 %1, i32* %y, align 4
  %i = alloca i32, align 4
  %ret = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %ret, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.then, %entry
  %cast = load i32, i32* %i, align 4
  %parm.arr.cast = load i32, i32* %x, align 4
  %2 = icmp slt i32 %cast, %parm.arr.cast
  br i1 %2, label %land.lhs.true, label %while.end

land.lhs.true:                                    ; preds = %while.cond
  %cast1 = load i32, i32* %i, align 4
  %parm.arr.cast2 = load i32, i32* %y, align 4
  %3 = icmp slt i32 %cast1, %parm.arr.cast2
  br i1 %3, label %while.then, label %while.end

while.then:                                       ; preds = %land.lhs.true
  %cast3 = load i32, i32* %i, align 4
  %4 = add i32 %cast3, 1
  store i32 %4, i32* %i, align 4
  %cast4 = load i32, i32* %ret, align 4
  %parm.arr.cast5 = load i32, i32* %x, align 4
  %5 = add i32 %cast4, %parm.arr.cast5
  %cast6 = load i32, i32* %i, align 4
  %6 = add i32 %5, %cast6
  store i32 %6, i32* %ret, align 4
  br label %while.cond

while.end:                                        ; preds = %land.lhs.true, %while.cond
  %cast7 = load i32, i32* %ret, align 4
  store i32 %cast7, i32* %retval, align 4
  br label %return

return:                                           ; preds = %while.end
  %7 = load i32, i32* %retval, align 4
  ret i32 %7
}

define i32 @swap(i32* %0, i32 %1, i32 %2) {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %arr = alloca i32*, align 8
  store i32* %0, i32** %arr, align 8
  %l = alloca i32, align 4
  store i32 %1, i32* %l, align 4
  %r = alloca i32, align 4
  store i32 %2, i32* %r, align 4
  %curr = alloca i32, align 4
  %parm.arr.cast = load i32, i32* %l, align 4
  %parm.arr.cast1 = load i32*, i32** %arr, align 8
  %3 = getelementptr inbounds i32, i32* %parm.arr.cast1, i32 %parm.arr.cast
  %cast = load i32, i32* %3, align 4
  store i32 %cast, i32* %curr, align 4
  %parm.arr.cast2 = load i32, i32* %l, align 4
  %parm.arr.cast3 = load i32*, i32** %arr, align 8
  %4 = getelementptr inbounds i32, i32* %parm.arr.cast3, i32 %parm.arr.cast2
  %parm.arr.cast4 = load i32, i32* %r, align 4
  %parm.arr.cast5 = load i32*, i32** %arr, align 8
  %5 = getelementptr inbounds i32, i32* %parm.arr.cast5, i32 %parm.arr.cast4
  %cast6 = load i32, i32* %5, align 4
  store i32 %cast6, i32* %4, align 4
  %parm.arr.cast7 = load i32, i32* %r, align 4
  %parm.arr.cast8 = load i32*, i32** %arr, align 8
  %6 = getelementptr inbounds i32, i32* %parm.arr.cast8, i32 %parm.arr.cast7
  %cast9 = load i32, i32* %curr, align 4
  store i32 %cast9, i32* %6, align 4
  %parm.arr.cast10 = load i32, i32* %l, align 4
  %parm.arr.cast11 = load i32, i32* %r, align 4
  %7 = call i32 @meanless_calculation(i32 %parm.arr.cast10, i32 %parm.arr.cast11)
  store i32 %7, i32* %retval, align 4
  br label %return

return:                                           ; preds = %entry
  %8 = load i32, i32* %retval, align 4
  ret i32 %8
}

define i32 @median(i32* %0, i32 %1, i32 %2, i32 %3) {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %arr = alloca i32*, align 8
  store i32* %0, i32** %arr, align 8
  %begin = alloca i32, align 4
  store i32 %1, i32* %begin, align 4
  %end = alloca i32, align 4
  store i32 %2, i32* %end, align 4
  %pos = alloca i32, align 4
  store i32 %3, i32* %pos, align 4
  %pivot = alloca i32, align 4
  %l = alloca i32, align 4
  %r = alloca i32, align 4
  %xx = alloca i32, align 4
  %parm.arr.cast = load i32, i32* %begin, align 4
  %parm.arr.cast1 = load i32*, i32** %arr, align 8
  %4 = getelementptr inbounds i32, i32* %parm.arr.cast1, i32 %parm.arr.cast
  %cast = load i32, i32* %4, align 4
  store i32 %cast, i32* %pivot, align 4
  %parm.arr.cast2 = load i32, i32* %begin, align 4
  store i32 %parm.arr.cast2, i32* %l, align 4
  %parm.arr.cast3 = load i32, i32* %end, align 4
  %5 = add i32 %parm.arr.cast3, 1
  store i32 %5, i32* %r, align 4
  store i32 0, i32* %xx, align 4
  br label %while.cond

while.cond:                                       ; preds = %if.end29, %entry
  br i1 true, label %while.then, label %while.end

while.then:                                       ; preds = %while.cond
  br label %while.cond4

while.cond4:                                      ; preds = %if.end, %while.then
  %cast7 = load i32, i32* %l, align 4
  %cast8 = load i32, i32* %r, align 4
  %6 = icmp slt i32 %cast7, %cast8
  br i1 %6, label %while.then5, label %while.end6

while.then5:                                      ; preds = %while.cond4
  %cast9 = load i32, i32* %r, align 4
  %7 = sub i32 %cast9, 1
  store i32 %7, i32* %r, align 4
  %cast10 = load i32, i32* %r, align 4
  %parm.arr.cast11 = load i32*, i32** %arr, align 8
  %8 = getelementptr inbounds i32, i32* %parm.arr.cast11, i32 %cast10
  %cast12 = load i32, i32* %8, align 4
  %cast13 = load i32, i32* %pivot, align 4
  %9 = icmp slt i32 %cast12, %cast13
  br i1 %9, label %if.then, label %if.end

if.then:                                          ; preds = %while.then5
  br label %while.end6

if.end:                                           ; preds = %while.then5
  %cast14 = load i32, i32* %xx, align 4
  %10 = add i32 %cast14, 1
  store i32 %10, i32* %xx, align 4
  br label %while.cond4

while.end6:                                       ; preds = %if.then, %while.cond4
  br label %while.cond15

while.cond15:                                     ; preds = %if.end22, %while.end6
  %cast18 = load i32, i32* %l, align 4
  %cast19 = load i32, i32* %r, align 4
  %11 = icmp slt i32 %cast18, %cast19
  br i1 %11, label %while.then16, label %while.end17

while.then16:                                     ; preds = %while.cond15
  %cast20 = load i32, i32* %l, align 4
  %12 = add i32 %cast20, 1
  store i32 %12, i32* %l, align 4
  %cast23 = load i32, i32* %l, align 4
  %parm.arr.cast24 = load i32*, i32** %arr, align 8
  %13 = getelementptr inbounds i32, i32* %parm.arr.cast24, i32 %cast23
  %cast25 = load i32, i32* %13, align 4
  %cast26 = load i32, i32* %pivot, align 4
  %14 = icmp sge i32 %cast25, %cast26
  br i1 %14, label %if.then21, label %if.end22

if.then21:                                        ; preds = %while.then16
  br label %while.end17

if.end22:                                         ; preds = %while.then16
  %cast27 = load i32, i32* %xx, align 4
  %15 = sub i32 %cast27, 1
  store i32 %15, i32* %xx, align 4
  br label %while.cond15

while.end17:                                      ; preds = %if.then21, %while.cond15
  %cast30 = load i32, i32* %l, align 4
  %cast31 = load i32, i32* %r, align 4
  %16 = icmp eq i32 %cast30, %cast31
  br i1 %16, label %if.then28, label %if.else

if.then28:                                        ; preds = %while.end17
  br label %while.end

if.else:                                          ; preds = %while.end17
  %parm.arr.cast32 = load i32*, i32** %arr, align 8
  %cast33 = load i32, i32* %l, align 4
  %cast34 = load i32, i32* %r, align 4
  %17 = call i32 @swap(i32* %parm.arr.cast32, i32 %cast33, i32 %cast34)
  br label %if.end29

if.end29:                                         ; preds = %if.else
  br label %while.cond

while.end:                                        ; preds = %if.then28, %while.cond
  %parm.arr.cast35 = load i32, i32* %begin, align 4
  %parm.arr.cast36 = load i32*, i32** %arr, align 8
  %18 = getelementptr inbounds i32, i32* %parm.arr.cast36, i32 %parm.arr.cast35
  %cast37 = load i32, i32* %pivot, align 4
  store i32 %cast37, i32* %18, align 4
  %parm.arr.cast38 = load i32*, i32** %arr, align 8
  %parm.arr.cast39 = load i32, i32* %begin, align 4
  %cast40 = load i32, i32* %l, align 4
  %19 = call i32 @swap(i32* %parm.arr.cast38, i32 %parm.arr.cast39, i32 %cast40)
  %cast43 = load i32, i32* %l, align 4
  %parm.arr.cast44 = load i32, i32* %pos, align 4
  %20 = icmp sgt i32 %cast43, %parm.arr.cast44
  br i1 %20, label %if.then41, label %if.end42

if.then41:                                        ; preds = %while.end
  %parm.arr.cast45 = load i32*, i32** %arr, align 8
  %parm.arr.cast46 = load i32, i32* %begin, align 4
  %cast47 = load i32, i32* %l, align 4
  %parm.arr.cast48 = load i32, i32* %pos, align 4
  %21 = call i32 @median(i32* %parm.arr.cast45, i32 %parm.arr.cast46, i32 %cast47, i32 %parm.arr.cast48)
  store i32 %21, i32* %retval, align 4
  br label %return

if.end42:                                         ; preds = %while.end
  %cast51 = load i32, i32* %l, align 4
  %parm.arr.cast52 = load i32, i32* %pos, align 4
  %22 = icmp slt i32 %cast51, %parm.arr.cast52
  br i1 %22, label %if.then49, label %if.end50

if.then49:                                        ; preds = %if.end42
  %parm.arr.cast53 = load i32*, i32** %arr, align 8
  %cast54 = load i32, i32* %l, align 4
  %23 = add i32 %cast54, 1
  %parm.arr.cast55 = load i32, i32* %end, align 4
  %parm.arr.cast56 = load i32, i32* %pos, align 4
  %24 = call i32 @median(i32* %parm.arr.cast53, i32 %23, i32 %parm.arr.cast55, i32 %parm.arr.cast56)
  store i32 %24, i32* %retval, align 4
  br label %return

if.end50:                                         ; preds = %if.end42
  %cast57 = load i32, i32* %xx, align 4
  store i32 %cast57, i32* %retval, align 4
  br label %return

return:                                           ; preds = %if.end50, %if.then49, %if.then41
  %25 = load i32, i32* %retval, align 4
  ret i32 %25
}

define i32 @main() {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %0 = call i32 @_sysy_getarray(i32* getelementptr inbounds ([10000000 x i32], [10000000 x i32]* @a, i32 0, i32 0))
  store i32 %0, i32* @n, align 4
  call void @_sysy_starttime(i32 60)
  %cast = load i32, i32* @n, align 4
  %1 = sub i32 %cast, 1
  %cast1 = load i32, i32* @n, align 4
  %2 = sdiv i32 %cast1, 2
  %3 = call i32 @median(i32* getelementptr inbounds ([10000000 x i32], [10000000 x i32]* @a, i32 0, i32 0), i32 0, i32 %1, i32 %2)
  call void @_sysy_stoptime(i32 62)
  %cast2 = load i32, i32* @n, align 4
  call void @_sysy_putarray(i32 %cast2, i32* getelementptr inbounds ([10000000 x i32], [10000000 x i32]* @a, i32 0, i32 0))
  %cast3 = load i32, i32* @n, align 4
  %4 = sdiv i32 %cast3, 2
  %5 = getelementptr inbounds [10000000 x i32], [10000000 x i32]* @a, i32 0, i32 %4
  %cast4 = load i32, i32* %5, align 4
  %6 = srem i32 %cast4, 256
  store i32 %6, i32* %retval, align 4
  br label %return

return:                                           ; preds = %entry
  %7 = load i32, i32* %retval, align 4
  ret i32 %7
}
