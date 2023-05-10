@s = global i32 0, align 4

; Function Attrs: noinline nounwind optnone
define i32 @get_ans_se(i32 %ans, i32 %v0, i32 %v1) #0 {
entry:
  %ans.addr = alloca i32, align 4
  %v0.addr = alloca i32, align 4
  %v1.addr = alloca i32, align 4
  %v = alloca i32, align 4
  store i32 %ans, i32* %ans.addr, align 4
  store i32 %v0, i32* %v0.addr, align 4
  store i32 %v1, i32* %v1.addr, align 4
  store i32 0, i32* %v, align 4
  %0 = load i32, i32* %v0.addr, align 4
  %1 = load i32, i32* %v1.addr, align 4
  %cmp = icmp eq i32 %0, %1
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 1, i32* %v, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %2 = load i32, i32* %ans.addr, align 4
  %mul = mul nsw i32 %2, 2
  store i32 %mul, i32* %ans.addr, align 4
  %3 = load i32, i32* %ans.addr, align 4
  %4 = load i32, i32* %v, align 4
  %add = add nsw i32 %3, %4
  store i32 %add, i32* %ans.addr, align 4
  %5 = load i32, i32* @s, align 4
  %6 = load i32, i32* %ans.addr, align 4
  %add1 = add nsw i32 %5, %6
  store i32 %add1, i32* @s, align 4
  %7 = load i32, i32* %ans.addr, align 4
  ret i32 %7
}

; Function Attrs: noinline nounwind optnone
define i32 @get_ans(i32 %ans, i32 %v0, i32 %v1) #0 {
entry:
  %ans.addr = alloca i32, align 4
  %v0.addr = alloca i32, align 4
  %v1.addr = alloca i32, align 4
  %v = alloca i32, align 4
  store i32 %ans, i32* %ans.addr, align 4
  store i32 %v0, i32* %v0.addr, align 4
  store i32 %v1, i32* %v1.addr, align 4
  store i32 0, i32* %v, align 4
  %0 = load i32, i32* %v0.addr, align 4
  %1 = load i32, i32* %v1.addr, align 4
  %cmp = icmp eq i32 %0, %1
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 1, i32* %v, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %2 = load i32, i32* %ans.addr, align 4
  %mul = mul nsw i32 %2, 2
  store i32 %mul, i32* %ans.addr, align 4
  %3 = load i32, i32* %ans.addr, align 4
  %4 = load i32, i32* %v, align 4
  %add = add nsw i32 %3, %4
  store i32 %add, i32* %ans.addr, align 4
  %5 = load i32, i32* %ans.addr, align 4
  ret i32 %5
}

; Function Attrs: noinline nounwind optnone
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %k0 = alloca i32, align 4
  %k1 = alloca i32, align 4
  %k2 = alloca i32, align 4
  %k3 = alloca i32, align 4
  %k4 = alloca i32, align 4
  %a1 = alloca i32, align 4
  %a2 = alloca i32, align 4
  %a3 = alloca i32, align 4
  %a4 = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 -2147483648, i32* %k0, align 4
  store i32 -2147483648, i32* %k1, align 4
  store i32 -2147483647, i32* %k2, align 4
  store i32 2147483647, i32* %k3, align 4
  store i32 2147483646, i32* %k4, align 4
  %call = call i32 @get_ans(i32 0, i32 -2147483648, i32 -2147483648)
  store i32 %call, i32* %a1, align 4
  %0 = load i32, i32* %a1, align 4
  %call1 = call i32 @get_ans(i32 %0, i32 -2147483647, i32 -2147483647)
  store i32 %call1, i32* %a1, align 4
  %1 = load i32, i32* %a1, align 4
  %call2 = call i32 @get_ans(i32 %1, i32 -2147483648, i32 -2147483648)
  store i32 %call2, i32* %a1, align 4
  %2 = load i32, i32* %a1, align 4
  %call3 = call i32 @get_ans(i32 %2, i32 -2147483648, i32 2147483647)
  store i32 %call3, i32* %a1, align 4
  %3 = load i32, i32* %a1, align 4
  %call4 = call i32 @get_ans(i32 %3, i32 -1073741824, i32 -1073741823)
  store i32 %call4, i32* %a1, align 4
  %4 = load i32, i32* %a1, align 4
  %call5 = call i32 @get_ans(i32 %4, i32 -2147483648, i32 -2147483648)
  store i32 %call5, i32* %a1, align 4
  %5 = load i32, i32* %a1, align 4
  %call6 = call i32 @get_ans(i32 %5, i32 -2147483648, i32 2147483647)
  store i32 %call6, i32* %a1, align 4
  %call7 = call i32 @get_ans(i32 0, i32 -2147483647, i32 2147483647)
  store i32 %call7, i32* %a2, align 4
  %6 = load i32, i32* %a2, align 4
  %call8 = call i32 @get_ans(i32 %6, i32 -2147483647, i32 2147483646)
  store i32 %call8, i32* %a2, align 4
  %7 = load i32, i32* %a2, align 4
  %call9 = call i32 @get_ans(i32 %7, i32 2147483647, i32 2147483646)
  store i32 %call9, i32* %a2, align 4
  %8 = load i32, i32* %a2, align 4
  %call10 = call i32 @get_ans(i32 %8, i32 -1073741824, i32 -1073741824)
  store i32 %call10, i32* %a2, align 4
  %call11 = call i32 @get_ans_se(i32 0, i32 -2147483648, i32 -2147483648)
  store i32 %call11, i32* %a3, align 4
  %9 = load i32, i32* %a3, align 4
  %call12 = call i32 @get_ans_se(i32 %9, i32 -2147483647, i32 -2147483647)
  store i32 %call12, i32* %a3, align 4
  %10 = load i32, i32* %a3, align 4
  %call13 = call i32 @get_ans_se(i32 %10, i32 -2147483648, i32 -2147483648)
  store i32 %call13, i32* %a3, align 4
  %11 = load i32, i32* %a3, align 4
  %call14 = call i32 @get_ans_se(i32 %11, i32 -2147483648, i32 2147483647)
  store i32 %call14, i32* %a3, align 4
  %12 = load i32, i32* %a3, align 4
  %call15 = call i32 @get_ans_se(i32 %12, i32 -1073741824, i32 -1073741823)
  store i32 %call15, i32* %a3, align 4
  %13 = load i32, i32* %a3, align 4
  %call16 = call i32 @get_ans_se(i32 %13, i32 -2147483648, i32 -2147483648)
  store i32 %call16, i32* %a3, align 4
  %14 = load i32, i32* %a3, align 4
  %call17 = call i32 @get_ans_se(i32 %14, i32 -2147483648, i32 2147483647)
  store i32 %call17, i32* %a3, align 4
  %call18 = call i32 @get_ans_se(i32 0, i32 -2147483647, i32 2147483647)
  store i32 %call18, i32* %a4, align 4
  %15 = load i32, i32* %a4, align 4
  %call19 = call i32 @get_ans_se(i32 %15, i32 -2147483647, i32 2147483646)
  store i32 %call19, i32* %a4, align 4
  %16 = load i32, i32* %a4, align 4
  %call20 = call i32 @get_ans_se(i32 %16, i32 2147483647, i32 2147483646)
  store i32 %call20, i32* %a4, align 4
  %17 = load i32, i32* %a4, align 4
  %call21 = call i32 @get_ans_se(i32 %17, i32 -1073741824, i32 -1073741824)
  store i32 %call21, i32* %a4, align 4
  %18 = load i32, i32* %a1, align 4
  call void @_sysy_putint(i32 %18)
  call void @_sysy_putch(i32 10)
  %19 = load i32, i32* %a2, align 4
  call void @_sysy_putint(i32 %19)
  call void @_sysy_putch(i32 10)
  %20 = load i32, i32* %a3, align 4
  call void @_sysy_putint(i32 %20)
  call void @_sysy_putch(i32 10)
  %21 = load i32, i32* %a4, align 4
  call void @_sysy_putint(i32 %21)
  %22 = load i32, i32* %a1, align 4
  %23 = load i32, i32* %a2, align 4
  %add = add nsw i32 %22, %23
  %24 = load i32, i32* %a3, align 4
  %add22 = add nsw i32 %add, %24
  %25 = load i32, i32* %a4, align 4
  %add23 = add nsw i32 %add22, %25
  ret i32 %add23
}

declare void @_sysy_putint(i32) #1

declare void @_sysy_putch(i32) #1

attributes #0 = { noinline nounwind optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Debian clang version 11.0.1-2"}