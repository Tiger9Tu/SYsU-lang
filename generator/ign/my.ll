; ModuleID = '-'
source_filename = "-"

@s = global i32 0

declare void @_sysy_starttime(i32)

declare void @_sysy_stoptime(i32)

declare i32 @_sysy_getch()

declare void @_sysy_putch(i32)

declare i32 @_sysy_getint()

declare void @_sysy_putint(i32)

declare i32 @_sysy_getarray(i32*)

declare void @_sysy_putarray(i32, i32*)

declare float @_sysy_getfloat()

declare void @_sysy_putfloat(float)

declare i32 @_sysy_getfarray(float*)

declare void @_sysy_putfarray(i32, float*)

declare void @_sysy_putf(i8*)

define i32 @get_ans_se(i32 %0, i32 %1, i32 %2) {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %ans = alloca i32, align 4
  store i32 %0, i32* %ans, align 4
  %v0 = alloca i32, align 4
  store i32 %1, i32* %v0, align 4
  %v1 = alloca i32, align 4
  store i32 %2, i32* %v1, align 4
  %v = alloca i32, align 4
  store i32 0, i32* %v, align 4
  %parm.arr.cast = load i32, i32* %v0, align 4
  %parm.arr.cast1 = load i32, i32* %v1, align 4
  %3 = icmp eq i32 %parm.arr.cast, %parm.arr.cast1
  br i1 %3, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 1, i32* %v, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %parm.arr.cast2 = load i32, i32* %ans, align 4
  %4 = mul i32 %parm.arr.cast2, 2
  store i32 %4, i32* %ans, align 4
  %parm.arr.cast3 = load i32, i32* %ans, align 4
  %cast = load i32, i32* %v, align 4
  %5 = add i32 %parm.arr.cast3, %cast
  store i32 %5, i32* %ans, align 4
  %cast4 = load i32, i32* @s, align 4
  %parm.arr.cast5 = load i32, i32* %ans, align 4
  %6 = add i32 %cast4, %parm.arr.cast5
  store i32 %6, i32* @s, align 4
  %parm.arr.cast6 = load i32, i32* %ans, align 4
  store i32 %parm.arr.cast6, i32* %retval, align 4
  br label %return

return:                                           ; preds = %if.end
  %7 = load i32, i32* %retval, align 4
  ret i32 %7
}

define i32 @get_ans(i32 %0, i32 %1, i32 %2) {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %ans = alloca i32, align 4
  store i32 %0, i32* %ans, align 4
  %v0 = alloca i32, align 4
  store i32 %1, i32* %v0, align 4
  %v1 = alloca i32, align 4
  store i32 %2, i32* %v1, align 4
  %v = alloca i32, align 4
  store i32 0, i32* %v, align 4
  %parm.arr.cast = load i32, i32* %v0, align 4
  %parm.arr.cast1 = load i32, i32* %v1, align 4
  %3 = icmp eq i32 %parm.arr.cast, %parm.arr.cast1
  br i1 %3, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 1, i32* %v, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %parm.arr.cast2 = load i32, i32* %ans, align 4
  %4 = mul i32 %parm.arr.cast2, 2
  store i32 %4, i32* %ans, align 4
  %parm.arr.cast3 = load i32, i32* %ans, align 4
  %cast = load i32, i32* %v, align 4
  %5 = add i32 %parm.arr.cast3, %cast
  store i32 %5, i32* %ans, align 4
  %parm.arr.cast4 = load i32, i32* %ans, align 4
  store i32 %parm.arr.cast4, i32* %retval, align 4
  br label %return

return:                                           ; preds = %if.end
  %6 = load i32, i32* %retval, align 4
  ret i32 %6
}

define i32 @main() {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %k0 = alloca i32, align 4
  %k1 = alloca i32, align 4
  %k2 = alloca i32, align 4
  %k3 = alloca i32, align 4
  %k4 = alloca i32, align 4
  %a1 = alloca i32, align 4
  %a2 = alloca i32, align 4
  %a3 = alloca i32, align 4
  %a4 = alloca i32, align 4
  store i32 0, i32* %k0, align 4
  store i32 0, i32* %k1, align 4
  store i32 1, i32* %k2, align 4
  store i32 2147483647, i32* %k3, align 4
  store i32 2147483646, i32* %k4, align 4
  %0 = call i32 @get_ans(i32 0, i32 0, i32 0)
  store i32 %0, i32* %a1, align 4
  %cast = load i32, i32* %a1, align 4
  %1 = call i32 @get_ans(i32 %cast, i32 1, i32 1)
  store i32 %1, i32* %a1, align 4
  %cast1 = load i32, i32* %a1, align 4
  %2 = call i32 @get_ans(i32 %cast1, i32 0, i32 -2147483648)
  store i32 %2, i32* %a1, align 4
  %cast2 = load i32, i32* %a1, align 4
  %3 = call i32 @get_ans(i32 %cast2, i32 0, i32 2147483647)
  store i32 %3, i32* %a1, align 4
  %cast3 = load i32, i32* %a1, align 4
  %4 = call i32 @get_ans(i32 %cast3, i32 0, i32 0)
  store i32 %4, i32* %a1, align 4
  %cast4 = load i32, i32* %a1, align 4
  %5 = call i32 @get_ans(i32 %cast4, i32 0, i32 -2147483648)
  store i32 %5, i32* %a1, align 4
  %cast5 = load i32, i32* %a1, align 4
  %6 = call i32 @get_ans(i32 %cast5, i32 0, i32 2147483647)
  store i32 %6, i32* %a1, align 4
  %7 = call i32 @get_ans(i32 0, i32 1, i32 2147483647)
  store i32 %7, i32* %a2, align 4
  %cast6 = load i32, i32* %a2, align 4
  %8 = call i32 @get_ans(i32 %cast6, i32 1, i32 2147483646)
  store i32 %8, i32* %a2, align 4
  %cast7 = load i32, i32* %a2, align 4
  %9 = call i32 @get_ans(i32 %cast7, i32 2147483647, i32 2147483646)
  store i32 %9, i32* %a2, align 4
  %cast8 = load i32, i32* %a2, align 4
  %10 = call i32 @get_ans(i32 %cast8, i32 0, i32 0)
  store i32 %10, i32* %a2, align 4
  %11 = call i32 @get_ans_se(i32 0, i32 0, i32 0)
  store i32 %11, i32* %a3, align 4
  %cast9 = load i32, i32* %a3, align 4
  %12 = call i32 @get_ans_se(i32 %cast9, i32 1, i32 1)
  store i32 %12, i32* %a3, align 4
  %cast10 = load i32, i32* %a3, align 4
  %13 = call i32 @get_ans_se(i32 %cast10, i32 0, i32 -2147483648)
  store i32 %13, i32* %a3, align 4
  %cast11 = load i32, i32* %a3, align 4
  %14 = call i32 @get_ans_se(i32 %cast11, i32 0, i32 2147483647)
  store i32 %14, i32* %a3, align 4
  %cast12 = load i32, i32* %a3, align 4
  %15 = call i32 @get_ans_se(i32 %cast12, i32 0, i32 0)
  store i32 %15, i32* %a3, align 4
  %cast13 = load i32, i32* %a3, align 4
  %16 = call i32 @get_ans_se(i32 %cast13, i32 0, i32 -2147483648)
  store i32 %16, i32* %a3, align 4
  %cast14 = load i32, i32* %a3, align 4
  %17 = call i32 @get_ans_se(i32 %cast14, i32 0, i32 2147483647)
  store i32 %17, i32* %a3, align 4
  %18 = call i32 @get_ans_se(i32 0, i32 1, i32 2147483647)
  store i32 %18, i32* %a4, align 4
  %cast15 = load i32, i32* %a4, align 4
  %19 = call i32 @get_ans_se(i32 %cast15, i32 1, i32 2147483646)
  store i32 %19, i32* %a4, align 4
  %cast16 = load i32, i32* %a4, align 4
  %20 = call i32 @get_ans_se(i32 %cast16, i32 2147483647, i32 2147483646)
  store i32 %20, i32* %a4, align 4
  %cast17 = load i32, i32* %a4, align 4
  %21 = call i32 @get_ans_se(i32 %cast17, i32 0, i32 0)
  store i32 %21, i32* %a4, align 4
  %cast18 = load i32, i32* %a1, align 4
  call void @_sysy_putint(i32 %cast18)
  call void @_sysy_putch(i32 10)
  %cast19 = load i32, i32* %a2, align 4
  call void @_sysy_putint(i32 %cast19)
  call void @_sysy_putch(i32 10)
  %cast20 = load i32, i32* %a3, align 4
  call void @_sysy_putint(i32 %cast20)
  call void @_sysy_putch(i32 10)
  %cast21 = load i32, i32* %a4, align 4
  call void @_sysy_putint(i32 %cast21)
  %cast22 = load i32, i32* %a1, align 4
  %cast23 = load i32, i32* %a2, align 4
  %22 = add i32 %cast22, %cast23
  %cast24 = load i32, i32* %a3, align 4
  %23 = add i32 %22, %cast24
  %cast25 = load i32, i32* %a4, align 4
  %24 = add i32 %23, %cast25
  store i32 %24, i32* %retval, align 4
  br label %return

return:                                           ; preds = %entry
  %25 = load i32, i32* %retval, align 4
  ret i32 %25
}



