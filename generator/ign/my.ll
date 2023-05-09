; ModuleID = '-'
source_filename = "-"

@RADIUS = global float 5.500000e+00
@PI = global float 0x400921FB60000000
@EPS = global float 0x3EB0C6F7A0000000
@PI_HEX = global float 0x400921FB60000000
@HEX2 = global float 7.812500e-02
@FACT = global float 3.300000e+04
@EVAL1 = global float 0.000000e+00
@EVAL2 = global float 0.000000e+00
@EVAL3 = global float 0.000000e+00
@CONV1 = global float 2.330000e+02
@CONV2 = global float 4.095000e+03
@MAX = global i32 1000000000
@TWO = global i32 2
@THREE = global i32 3
@FIVE = global i32 5

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

define float @float_abs(float %0) {
entry:
  %retval = alloca float, align 4
  store float 0.000000e+00, float* %retval, align 4
  %x = alloca float, align 4
  store float %0, float* %x, align 4
  %parm.arr.cast = load float, float* %x, align 4
  %1 = fcmp ult float %parm.arr.cast, 0.000000e+00
  br i1 %1, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %parm.arr.cast1 = load float, float* %x, align 4
  %fneg = fneg float %parm.arr.cast1
  store float %fneg, float* %retval, align 4
  br label %return

if.end:                                           ; preds = %entry
  %parm.arr.cast2 = load float, float* %x, align 4
  store float %parm.arr.cast2, float* %retval, align 4
  br label %return

return:                                           ; preds = %if.end, %if.then
  %2 = load float, float* %retval, align 4
  ret float %2
}

define float @circle_area(i32 %0) {
entry:
  %retval = alloca float, align 4
  store float 0.000000e+00, float* %retval, align 4
  %radius = alloca i32, align 4
  store i32 %0, i32* %radius, align 4
  %cast = load float, float* @PI, align 4
  %parm.arr.cast = load i32, i32* %radius, align 4
  %1 = sitofp i32 %parm.arr.cast to float
  %2 = fmul float %cast, %1
  %parm.arr.cast1 = load i32, i32* %radius, align 4
  %3 = sitofp i32 %parm.arr.cast1 to float
  %4 = fmul float %2, %3
  %parm.arr.cast2 = load i32, i32* %radius, align 4
  %parm.arr.cast3 = load i32, i32* %radius, align 4
  %5 = mul i32 %parm.arr.cast2, %parm.arr.cast3
  %6 = sitofp i32 %5 to float
  %cast4 = load float, float* @PI, align 4
  %7 = fmul float %6, %cast4
  %8 = fadd float %4, %7
  %9 = fdiv float %8, 2.000000e+00
  store float %9, float* %retval, align 4
  br label %return

return:                                           ; preds = %entry
  %10 = load float, float* %retval, align 4
  ret float %10
}

define i32 @float_eq(float %0, float %1) {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %a = alloca float, align 4
  store float %0, float* %a, align 4
  %b = alloca float, align 4
  store float %1, float* %b, align 4
  %parm.arr.cast = load float, float* %a, align 4
  %parm.arr.cast1 = load float, float* %b, align 4
  %2 = fsub float %parm.arr.cast, %parm.arr.cast1
  %3 = call float @float_abs(float %2)
  %cast = load float, float* @EPS, align 4
  %4 = fcmp ult float %3, %cast
  br i1 %4, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  store i32 0, i32* %retval, align 4
  br label %return

if.else:                                          ; preds = %entry
  store i32 0, i32* %retval, align 4
  br label %return

return:                                           ; preds = %if.else, %if.then
  %5 = load i32, i32* %retval, align 4
  ret i32 %5
}

define void @error() {
entry:
  call void @_sysy_putch(i32 101)
  call void @_sysy_putch(i32 114)
  call void @_sysy_putch(i32 114)
  call void @_sysy_putch(i32 111)
  call void @_sysy_putch(i32 114)
  call void @_sysy_putch(i32 10)
  br label %return

return:                                           ; preds = %entry
  ret void
}

define void @ok() {
entry:
  call void @_sysy_putch(i32 111)
  call void @_sysy_putch(i32 107)
  call void @_sysy_putch(i32 10)
  br label %return

return:                                           ; preds = %entry
  ret void
}

define void @assert(i32 %0) {
entry:
  %cond = alloca i32, align 4
  store i32 %0, i32* %cond, align 4
  %parm.arr.cast = load i32, i32* %cond, align 4
  %1 = icmp ne i32 %parm.arr.cast, 0
  %2 = xor i1 %1, true
  br i1 %2, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  call void @error()
  br label %if.end

if.else:                                          ; preds = %entry
  call void @ok()
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  br label %return

return:                                           ; preds = %if.end
  ret void
}

define void @assert_not(i32 %0) {
entry:
  %cond = alloca i32, align 4
  store i32 %0, i32* %cond, align 4
  %parm.arr.cast = load i32, i32* %cond, align 4
  %tobool = icmp ne i32 %parm.arr.cast, 0
  br i1 %tobool, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  call void @error()
  br label %if.end

if.else:                                          ; preds = %entry
  call void @ok()
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  br label %return

return:                                           ; preds = %if.end
  ret void
}

define i32 @main() {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %i = alloca i32, align 4
  %p = alloca i32, align 4
  %arr = alloca [10 x float], align 4
  %len = alloca i32, align 4
  %input = alloca float, align 4
  %area = alloca float, align 4
  %area_trunc = alloca float, align 4
  %cast = load float, float* @HEX2, align 4
  %cast1 = load float, float* @FACT, align 4
  %0 = call i32 @float_eq(float %cast, float %cast1)
  call void @assert_not(i32 %0)
  %cast2 = load float, float* @EVAL1, align 4
  %cast3 = load float, float* @EVAL2, align 4
  %1 = call i32 @float_eq(float %cast2, float %cast3)
  call void @assert_not(i32 %1)
  %cast4 = load float, float* @EVAL2, align 4
  %cast5 = load float, float* @EVAL3, align 4
  %2 = call i32 @float_eq(float %cast4, float %cast5)
  call void @assert(i32 %2)
  %cast6 = load float, float* @RADIUS, align 4
  %3 = fptosi float %cast6 to i32
  %4 = call float @circle_area(i32 %3)
  %cast7 = load i32, i32* @FIVE, align 4
  %5 = call float @circle_area(i32 %cast7)
  %6 = call i32 @float_eq(float %4, float %5)
  call void @assert(i32 %6)
  %cast8 = load float, float* @CONV1, align 4
  %cast9 = load float, float* @CONV2, align 4
  %7 = call i32 @float_eq(float %cast8, float %cast9)
  call void @assert_not(i32 %7)
  br i1 false, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  call void @ok()
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  br i1 false, label %if.then10, label %if.end11

if.then10:                                        ; preds = %if.end
  call void @ok()
  br label %if.end11

if.end11:                                         ; preds = %if.then10, %if.end
  br i1 false, label %if.then12, label %if.end13

if.then12:                                        ; preds = %if.end11
  call void @error()
  br label %if.end13

if.end13:                                         ; preds = %if.then12, %if.end11
  br i1 true, label %if.then14, label %if.end15

if.then14:                                        ; preds = %if.end13
  call void @ok()
  br label %if.end15

if.end15:                                         ; preds = %if.then14, %if.end13
  store i32 1, i32* %i, align 4
  store i32 0, i32* %p, align 4
  %8 = bitcast [10 x float]* %arr to i8*
  call void @llvm.memset.p0i8.i32(i8* align 8 %8, i8 0, i32 40, i1 false)
  %9 = getelementptr inbounds [10 x float], [10 x float]* %arr, i32 0, i32 0
  store float 1.000000e+00, float* %9, align 4
  %10 = getelementptr inbounds [10 x float], [10 x float]* %arr, i32 0, i32 1
  store float 2.000000e+00, float* %10, align 4
  %11 = getelementptr inbounds [10 x float], [10 x float]* %arr, i32 0, i32 0
  %12 = call i32 @_sysy_getfarray(float* %11)
  store i32 %12, i32* %len, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %if.end15
  %cast16 = load i32, i32* %i, align 4
  %cast17 = load i32, i32* @MAX, align 4
  %13 = icmp slt i32 %cast16, %cast17
  br i1 %13, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %14 = call float @_sysy_getfloat()
  store float %14, float* %input, align 4
  %cast18 = load float, float* @PI, align 4
  %cast19 = load float, float* %input, align 4
  %15 = fmul float %cast18, %cast19
  %cast20 = load float, float* %input, align 4
  %16 = fmul float %15, %cast20
  store float %16, float* %area, align 4
  %cast21 = load float, float* %input, align 4
  %17 = fptosi float %cast21 to i32
  %18 = call float @circle_area(i32 %17)
  store float %18, float* %area_trunc, align 4
  %cast22 = load i32, i32* %p, align 4
  %19 = getelementptr inbounds [10 x float], [10 x float]* %arr, i32 0, i32 %cast22
  %cast23 = load i32, i32* %p, align 4
  %20 = getelementptr inbounds [10 x float], [10 x float]* %arr, i32 0, i32 %cast23
  %cast24 = load float, float* %20, align 4
  %cast25 = load float, float* %input, align 4
  %21 = fadd float %cast24, %cast25
  store float %21, float* %19, align 4
  %cast26 = load float, float* %area, align 4
  call void @_sysy_putfloat(float %cast26)
  call void @_sysy_putch(i32 32)
  %cast27 = load float, float* %area_trunc, align 4
  %22 = fptosi float %cast27 to i32
  call void @_sysy_putint(i32 %22)
  call void @_sysy_putch(i32 10)
  %cast28 = load i32, i32* %i, align 4
  %23 = sitofp i32 %cast28 to double
  %24 = fmul double %23, 0x41200000
  %25 = fptosi double %24 to i32
  store i32 %25, i32* %i, align 4
  %cast29 = load i32, i32* %p, align 4
  %26 = add i32 %cast29, 1
  store i32 %26, i32* %p, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %cast30 = load i32, i32* %len, align 4
  %27 = getelementptr inbounds [10 x float], [10 x float]* %arr, i32 0, i32 0
  call void @_sysy_putfarray(i32 %cast30, float* %27)
  store i32 0, i32* %retval, align 4
  br label %return

return:                                           ; preds = %while.end
  %28 = load i32, i32* %retval, align 4
  ret i32 %28
}

; Function Attrs: argmemonly nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i32(i8* nocapture writeonly, i8, i32, i1 immarg) #0

attributes #0 = { argmemonly nounwind willreturn writeonly }
