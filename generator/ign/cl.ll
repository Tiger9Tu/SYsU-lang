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
  %i = alloca i32, align 4
  %p = alloca i32, align 4
  %arr = alloca [10 x float], align 16
  %len = alloca i32, align 4
  %input = alloca float, align 4
  %area = alloca float, align 4
  %area_trunc = alloca float, align 4
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
  br i1 true, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  call void @ok()
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  call void @ok()
  call void @ok()
  store i32 1, i32* %i, align 4
  store i32 0, i32* %p, align 4
  %0 = bitcast [10 x float]* %arr to i8*
  call void @llvm.memset.p0i8.i64(i8* align 16 %0, i8 0, i64 40, i1 false)
  %1 = bitcast i8* %0 to <{ float, float, [8 x float] }>*
  %2 = getelementptr inbounds <{ float, float, [8 x float] }>, <{ float, float, [8 x float] }>* %1, i32 0, i32 0
  store float 1.000000e+00, float* %2, align 16
  %3 = getelementptr inbounds <{ float, float, [8 x float] }>, <{ float, float, [8 x float] }>* %1, i32 0, i32 1
  store float 2.000000e+00, float* %3, align 4
  %arraydecay = getelementptr inbounds [10 x float], [10 x float]* %arr, i64 0, i64 0
  %call7 = call i32 @_sysy_getfarray(float* %arraydecay)
  store i32 %call7, i32* %len, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %if.end
  %4 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %4, 1000000000
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %call8 = call float (...) @_sysy_getfloat()
  store float %call8, float* %input, align 4
  %5 = load float, float* %input, align 4
  %mul = fmul float 0x400921FB60000000, %5
  %6 = load float, float* %input, align 4
  %mul9 = fmul float %mul, %6
  store float %mul9, float* %area, align 4
  %7 = load float, float* %input, align 4
  %conv = fptosi float %7 to i32
  %call10 = call float @circle_area(i32 %conv)
  store float %call10, float* %area_trunc, align 4
  %8 = load i32, i32* %p, align 4
  %idxprom = sext i32 %8 to i64
  %arrayidx = getelementptr inbounds [10 x float], [10 x float]* %arr, i64 0, i64 %idxprom
  %9 = load float, float* %arrayidx, align 4
  %10 = load float, float* %input, align 4
  %add = fadd float %9, %10
  %11 = load i32, i32* %p, align 4
  %idxprom11 = sext i32 %11 to i64
  %arrayidx12 = getelementptr inbounds [10 x float], [10 x float]* %arr, i64 0, i64 %idxprom11
  store float %add, float* %arrayidx12, align 4
  %12 = load float, float* %area, align 4
  call void @_sysy_putfloat(float %12)
  call void @_sysy_putch(i32 32)
  %13 = load float, float* %area_trunc, align 4
  %conv13 = fptosi float %13 to i32
  call void @_sysy_putint(i32 %conv13)
  call void @_sysy_putch(i32 10)
  %14 = load i32, i32* %i, align 4
  %conv14 = sitofp i32 %14 to double
  %mul15 = fmul double %conv14, 1.000000e+01
  %conv16 = fptosi double %mul15 to i32
  store i32 %conv16, i32* %i, align 4
  %15 = load i32, i32* %p, align 4
  %add17 = add nsw i32 %15, 1
  store i32 %add17, i32* %p, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %16 = load i32, i32* %len, align 4
  %arraydecay18 = getelementptr inbounds [10 x float], [10 x float]* %arr, i64 0, i64 0
  call void @_sysy_putfarray(i32 %16, float* %arraydecay18)
  ret i32 0
}

; Function Attrs: argmemonly nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #2

declare i32 @_sysy_getfarray(float*) #1

declare float @_sysy_getfloat(...) #1

declare void @_sysy_putfloat(float) #1

declare void @_sysy_putint(i32) #1

declare void @_sysy_putfarray(i32, float*) #1

attributes #0 = { noinline nounwind optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { argmemonly nounwind willreturn writeonly }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
5!1 = !{!"Debian clang version 11.0.1-2"}