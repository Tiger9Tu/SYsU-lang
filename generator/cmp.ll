my output:
; ModuleID = '-'
source_filename = "-"

@RADIUS = global float 5.500000e+00
@PI = global float 0x400921FB60000000
@EPS = global float 0x3EB0C6F7A0000000
@PI_HEX = global float 0x400921FB60000000
@HEX2 = global float 7.812500e-02
@FACT = global float -3.300000e+04
@EVAL1 = global float 0.000000e+00
@EVAL2 = global float 0.000000e+00
@EVAL3 = global float 0.000000e+00
@CONV1 = global float 2.330000e+02
@CONV2 = global float 4.095000e+03
@MAX = global i32 1000000000
@TWO = global i32 2
@THREE = global i32 3
@FIVE = global i32 0

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
  store i32 1, i32* %retval, align 4
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
  %3 = call float @circle_area(i32 0)
  %cast7 = load i32, i32* @FIVE, align 4
  %4 = call float @circle_area(i32 %cast7)
  %5 = call i32 @float_eq(float %3, float %4)
  call void @assert(i32 %5)
  %cast8 = load float, float* @CONV1, align 4
  %cast9 = load float, float* @CONV2, align 4
  %6 = call i32 @float_eq(float %cast8, float %cast9)
  call void @assert_not(i32 %6)
  store i32 0, i32* %retval, align 4
  br label %return

return:                                           ; preds = %entry
  %7 = load i32, i32* %retval, align 4
  ret i32 %7
}



clang output:
@RADIUS = constant float 5.500000e+00, align 4
@PI = constant float 0x400921FB60000000, align 4
@EPS = constant float 0x3EB0C6F7A0000000, align 4
@PI_HEX = constant float 0x400921FB60000000, align 4
@HEX2 = constant float 7.812500e-02, align 4
@FACT = constant float -3.300000e+04, align 4
@EVAL1 = constant float 0x4057C21FC0000000, align 4
@EVAL2 = constant float 0x4041475CE0000000, align 4
@EVAL3 = constant float 0x4041475CE0000000, align 4
@CONV1 = constant float 2.330000e+02, align 4
@CONV2 = constant float 4.095000e+03, align 4
@MAX = constant i32 1000000000, align 4
@TWO = constant i32 2, align 4
@THREE = constant i32 3, align 4
@FIVE = constant i32 5, align 4

; Function Attrs: noinline nounwind optnone
define float @float_abs(float %x) #0 {
entry:
  %retval = alloca float, align 4
  %x.addr = alloca float, align 4
  store float %x, float* %x.addr, align 4
  %0 = load float, float* %x.addr, align 4
  %cmp = fcmp olt float %0, 0.000000e+00
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %1 = load float, float* %x.addr, align 4
  %fneg = fneg float %1
  store float %fneg, float* %retval, align 4
  br label %return

if.end:                                           ; preds = %entry
  %2 = load float, float* %x.addr, align 4
  store float %2, float* %retval, align 4
  br label %return

return:                                           ; preds = %if.end, %if.then
  %3 = load float, float* %retval, align 4
  ret float %3
}

; Function Attrs: noinline nounwind optnone
define float @circle_area(i32 %radius) #0 {
entry:
  %radius.addr = alloca i32, align 4
  store i32 %radius, i32* %radius.addr, align 4
  %0 = load i32, i32* %radius.addr, align 4
  %conv = sitofp i32 %0 to float
  %mul = fmul float 0x400921FB60000000, %conv
  %1 = load i32, i32* %radius.addr, align 4
  %conv1 = sitofp i32 %1 to float
  %mul2 = fmul float %mul, %conv1
  %2 = load i32, i32* %radius.addr, align 4
  %3 = load i32, i32* %radius.addr, align 4
  %mul3 = mul nsw i32 %2, %3
  %conv4 = sitofp i32 %mul3 to float
  %mul5 = fmul float %conv4, 0x400921FB60000000
  %add = fadd float %mul2, %mul5
  %div = fdiv float %add, 2.000000e+00
  ret float %div
}

; Function Attrs: noinline nounwind optnone
define i32 @float_eq(float %a, float %b) #0 {
entry:
  %retval = alloca i32, align 4
  %a.addr = alloca float, align 4
  %b.addr = alloca float, align 4
  store float %a, float* %a.addr, align 4
  store float %b, float* %b.addr, align 4
  %0 = load float, float* %a.addr, align 4
  %1 = load float, float* %b.addr, align 4
  %sub = fsub float %0, %1
  %call = call float @float_abs(float %sub)
  %cmp = fcmp olt float %call, 0x3EB0C6F7A0000000
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  store i32 1, i32* %retval, align 4
  br label %return

if.else:                                          ; preds = %entry
  store i32 0, i32* %retval, align 4
  br label %return

return:                                           ; preds = %if.else, %if.then
  %2 = load i32, i32* %retval, align 4
  ret i32 %2
}

; Function Attrs: noinline nounwind optnone
define void @error() #0 {
entry:
  call void @_sysy_putch(i32 101)
  call void @_sysy_putch(i32 114)
  call void @_sysy_putch(i32 114)
  call void @_sysy_putch(i32 111)
  call void @_sysy_putch(i32 114)
  call void @_sysy_putch(i32 10)
  ret void
}

declare void @_sysy_putch(i32) #1

; Function Attrs: noinline nounwind optnone
define void @ok() #0 {
entry:
  call void @_sysy_putch(i32 111)
  call void @_sysy_putch(i32 107)
  call void @_sysy_putch(i32 10)
  ret void
}

; Function Attrs: noinline nounwind optnone
define void @assert(i32 %cond) #0 {
entry:
  %cond.addr = alloca i32, align 4
  store i32 %cond, i32* %cond.addr, align 4
  %0 = load i32, i32* %cond.addr, align 4
  %tobool = icmp ne i32 %0, 0
  br i1 %tobool, label %if.else, label %if.then

if.then:                                          ; preds = %entry
  call void @error()
  br label %if.end

if.else:                                          ; preds = %entry
  call void @ok()
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  ret void
}

; Function Attrs: noinline nounwind optnone
define void @assert_not(i32 %cond) #0 {
entry:
  %cond.addr = alloca i32, align 4
  store i32 %cond, i32* %cond.addr, align 4
  %0 = load i32, i32* %cond.addr, align 4
  %tobool = icmp ne i32 %0, 0
  br i1 %tobool, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  call void @error()
  br label %if.end

if.else:                                          ; preds = %entry
  call void @ok()
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  ret void
}

; Function Attrs: noinline nounwind optnone
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %call = call i32 @float_eq(float 7.812500e-02, float -3.300000e+04)
  call void @assert_not(i32 %call)
  %call1 = call i32 @float_eq(float 0x4057C21FC0000000, float 0x4041475CE0000000)
  call void @assert_not(i32 %call1)
  %call2 = call i32 @float_eq(float 0x4041475CE0000000, float 0x4041475CE0000000)
  call void @assert(i32 %call2)
  %call3 = call float @circle_area(i32 5)
  %call4 = call float @circle_area(i32 5)
  %call5 = call i32 @float_eq(float %call3, float %call4)
  call void @assert(i32 %call5)
  %call6 = call i32 @float_eq(float 2.330000e+02, float 4.095000e+03)
  call void @assert_not(i32 %call6)
  ret i32 0
}

attributes #0 = { noinline nounwind optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Debian clang version 11.0.1-2"}
