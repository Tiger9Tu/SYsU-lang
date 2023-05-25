; ModuleID = '-'
source_filename = "-"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@a = global [10000000 x i32] zeroinitializer, align 16
@n = global i32 0, align 4

; Function Attrs: noinline nounwind optnone
define i32 @meanless_calculation(i32 %x, i32 %y) #0 {
entry:
  %x.addr = alloca i32, align 4
  %y.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %ret = alloca i32, align 4
  store i32 %x, i32* %x.addr, align 4
  store i32 %y, i32* %y.addr, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %ret, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %0 = load i32, i32* %i, align 4
  %1 = load i32, i32* %x.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %land.rhs, label %land.end

land.rhs:                                         ; preds = %while.cond
  %2 = load i32, i32* %i, align 4
  %3 = load i32, i32* %y.addr, align 4
  %cmp1 = icmp slt i32 %2, %3
  br label %land.end

land.end:                                         ; preds = %land.rhs, %while.cond
  %4 = phi i1 [ false, %while.cond ], [ %cmp1, %land.rhs ]
  br i1 %4, label %while.body, label %while.end

while.body:                                       ; preds = %land.end
  %5 = load i32, i32* %i, align 4
  %add = add nsw i32 %5, 1
  store i32 %add, i32* %i, align 4
  %6 = load i32, i32* %ret, align 4
  %7 = load i32, i32* %x.addr, align 4
  %add2 = add nsw i32 %6, %7
  %8 = load i32, i32* %i, align 4
  %add3 = add nsw i32 %add2, %8
  store i32 %add3, i32* %ret, align 4
  br label %while.cond

while.end:                                        ; preds = %land.end
  %9 = load i32, i32* %ret, align 4
  ret i32 %9
}

; Function Attrs: noinline nounwind optnone
define i32 @swap(i32* %arr, i32 %l, i32 %r) #0 {
entry:
  %arr.addr = alloca i32*, align 8
  %l.addr = alloca i32, align 4
  %r.addr = alloca i32, align 4
  %curr = alloca i32, align 4
  store i32* %arr, i32** %arr.addr, align 8
  store i32 %l, i32* %l.addr, align 4
  store i32 %r, i32* %r.addr, align 4
  %0 = load i32*, i32** %arr.addr, align 8
  %1 = load i32, i32* %l.addr, align 4
  %idxprom = sext i32 %1 to i64
  %arrayidx = getelementptr inbounds i32, i32* %0, i64 %idxprom
  %2 = load i32, i32* %arrayidx, align 4
  store i32 %2, i32* %curr, align 4
  %3 = load i32*, i32** %arr.addr, align 8
  %4 = load i32, i32* %r.addr, align 4
  %idxprom1 = sext i32 %4 to i64
  %arrayidx2 = getelementptr inbounds i32, i32* %3, i64 %idxprom1
  %5 = load i32, i32* %arrayidx2, align 4
  %6 = load i32*, i32** %arr.addr, align 8
  %7 = load i32, i32* %l.addr, align 4
  %idxprom3 = sext i32 %7 to i64
  %arrayidx4 = getelementptr inbounds i32, i32* %6, i64 %idxprom3
  store i32 %5, i32* %arrayidx4, align 4
  %8 = load i32, i32* %curr, align 4
  %9 = load i32*, i32** %arr.addr, align 8
  %10 = load i32, i32* %r.addr, align 4
  %idxprom5 = sext i32 %10 to i64
  %arrayidx6 = getelementptr inbounds i32, i32* %9, i64 %idxprom5
  store i32 %8, i32* %arrayidx6, align 4
  %11 = load i32, i32* %l.addr, align 4
  %12 = load i32, i32* %r.addr, align 4
  %call = call i32 @meanless_calculation(i32 %11, i32 %12)
  ret i32 %call
}

; Function Attrs: noinline nounwind optnone
define i32 @median(i32* %arr, i32 %begin, i32 %end, i32 %pos) #0 {
entry:
  %retval = alloca i32, align 4
  %arr.addr = alloca i32*, align 8
  %begin.addr = alloca i32, align 4
  %end.addr = alloca i32, align 4
  %pos.addr = alloca i32, align 4
  %pivot = alloca i32, align 4
  %l = alloca i32, align 4
  %r = alloca i32, align 4
  %xx = alloca i32, align 4
  store i32* %arr, i32** %arr.addr, align 8
  store i32 %begin, i32* %begin.addr, align 4
  store i32 %end, i32* %end.addr, align 4
  store i32 %pos, i32* %pos.addr, align 4
  %0 = load i32*, i32** %arr.addr, align 8
  %1 = load i32, i32* %begin.addr, align 4
  %idxprom = sext i32 %1 to i64
  %arrayidx = getelementptr inbounds i32, i32* %0, i64 %idxprom
  %2 = load i32, i32* %arrayidx, align 4
  store i32 %2, i32* %pivot, align 4
  %3 = load i32, i32* %begin.addr, align 4
  store i32 %3, i32* %l, align 4
  %4 = load i32, i32* %end.addr, align 4
  %add = add nsw i32 %4, 1
  store i32 %add, i32* %r, align 4
  store i32 0, i32* %xx, align 4
  br label %while.body

while.body:                                       ; preds = %entry, %if.end20
  br label %while.cond1

while.cond1:                                      ; preds = %if.end, %while.body
  %5 = load i32, i32* %l, align 4
  %6 = load i32, i32* %r, align 4
  %cmp = icmp slt i32 %5, %6
  br i1 %cmp, label %while.body2, label %while.end

while.body2:                                      ; preds = %while.cond1
  %7 = load i32, i32* %r, align 4
  %sub = sub nsw i32 %7, 1
  store i32 %sub, i32* %r, align 4
  %8 = load i32*, i32** %arr.addr, align 8
  %9 = load i32, i32* %r, align 4
  %idxprom3 = sext i32 %9 to i64
  %arrayidx4 = getelementptr inbounds i32, i32* %8, i64 %idxprom3
  %10 = load i32, i32* %arrayidx4, align 4
  %11 = load i32, i32* %pivot, align 4
  %cmp5 = icmp slt i32 %10, %11
  br i1 %cmp5, label %if.then, label %if.end

if.then:                                          ; preds = %while.body2
  br label %while.end

if.end:                                           ; preds = %while.body2
  %12 = load i32, i32* %xx, align 4
  %add6 = add nsw i32 %12, 1
  store i32 %add6, i32* %xx, align 4
  br label %while.cond1

while.end:                                        ; preds = %if.then, %while.cond1
  br label %while.cond7

while.cond7:                                      ; preds = %if.end15, %while.end
  %13 = load i32, i32* %l, align 4
  %14 = load i32, i32* %r, align 4
  %cmp8 = icmp slt i32 %13, %14
  br i1 %cmp8, label %while.body9, label %while.end17

while.body9:                                      ; preds = %while.cond7
  %15 = load i32, i32* %l, align 4
  %add10 = add nsw i32 %15, 1
  store i32 %add10, i32* %l, align 4
  %16 = load i32*, i32** %arr.addr, align 8
  %17 = load i32, i32* %l, align 4
  %idxprom11 = sext i32 %17 to i64
  %arrayidx12 = getelementptr inbounds i32, i32* %16, i64 %idxprom11
  %18 = load i32, i32* %arrayidx12, align 4
  %19 = load i32, i32* %pivot, align 4
  %cmp13 = icmp sge i32 %18, %19
  br i1 %cmp13, label %if.then14, label %if.end15

if.then14:                                        ; preds = %while.body9
  br label %while.end17

if.end15:                                         ; preds = %while.body9
  %20 = load i32, i32* %xx, align 4
  %sub16 = sub nsw i32 %20, 1
  store i32 %sub16, i32* %xx, align 4
  br label %while.cond7

while.end17:                                      ; preds = %if.then14, %while.cond7
  %21 = load i32, i32* %l, align 4
  %22 = load i32, i32* %r, align 4
  %cmp18 = icmp eq i32 %21, %22
  br i1 %cmp18, label %if.then19, label %if.else

if.then19:                                        ; preds = %while.end17
  br label %while.end21

if.else:                                          ; preds = %while.end17
  %23 = load i32*, i32** %arr.addr, align 8
  %24 = load i32, i32* %l, align 4
  %25 = load i32, i32* %r, align 4
  %call = call i32 @swap(i32* %23, i32 %24, i32 %25)
  br label %if.end20

if.end20:                                         ; preds = %if.else
  br label %while.body

while.end21:                                      ; preds = %if.then19
  %26 = load i32, i32* %pivot, align 4
  %27 = load i32*, i32** %arr.addr, align 8
  %28 = load i32, i32* %begin.addr, align 4
  %idxprom22 = sext i32 %28 to i64
  %arrayidx23 = getelementptr inbounds i32, i32* %27, i64 %idxprom22
  store i32 %26, i32* %arrayidx23, align 4
  %29 = load i32*, i32** %arr.addr, align 8
  %30 = load i32, i32* %begin.addr, align 4
  %31 = load i32, i32* %l, align 4
  %call24 = call i32 @swap(i32* %29, i32 %30, i32 %31)
  %32 = load i32, i32* %l, align 4
  %33 = load i32, i32* %pos.addr, align 4
  %cmp25 = icmp sgt i32 %32, %33
  br i1 %cmp25, label %if.then26, label %if.end28

if.then26:                                        ; preds = %while.end21
  %34 = load i32*, i32** %arr.addr, align 8
  %35 = load i32, i32* %begin.addr, align 4
  %36 = load i32, i32* %l, align 4
  %37 = load i32, i32* %pos.addr, align 4
  %call27 = call i32 @median(i32* %34, i32 %35, i32 %36, i32 %37)
  store i32 %call27, i32* %retval, align 4
  br label %return

if.end28:                                         ; preds = %while.end21
  %38 = load i32, i32* %l, align 4
  %39 = load i32, i32* %pos.addr, align 4
  %cmp29 = icmp slt i32 %38, %39
  br i1 %cmp29, label %if.then30, label %if.end33

if.then30:                                        ; preds = %if.end28
  %40 = load i32*, i32** %arr.addr, align 8
  %41 = load i32, i32* %l, align 4
  %add31 = add nsw i32 %41, 1
  %42 = load i32, i32* %end.addr, align 4
  %43 = load i32, i32* %pos.addr, align 4
  %call32 = call i32 @median(i32* %40, i32 %add31, i32 %42, i32 %43)
  store i32 %call32, i32* %retval, align 4
  br label %return

if.end33:                                         ; preds = %if.end28
  %44 = load i32, i32* %xx, align 4
  store i32 %44, i32* %retval, align 4
  br label %return

return:                                           ; preds = %if.end33, %if.then30, %if.then26
  %45 = load i32, i32* %retval, align 4
  ret i32 %45
}

; Function Attrs: noinline nounwind optnone
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %call = call i32 @_sysy_getarray(i32* getelementptr inbounds ([10000000 x i32], [10000000 x i32]* @a, i64 0, i64 0))
  store i32 %call, i32* @n, align 4
  call void @_sysy_starttime(i32 60)
  %0 = load i32, i32* @n, align 4
  %sub = sub nsw i32 %0, 1
  %1 = load i32, i32* @n, align 4
  %div = sdiv i32 %1, 2
  %call1 = call i32 @median(i32* getelementptr inbounds ([10000000 x i32], [10000000 x i32]* @a, i64 0, i64 0), i32 0, i32 %sub, i32 %div)
  call void @_sysy_stoptime(i32 62)
  %2 = load i32, i32* @n, align 4
  call void @_sysy_putarray(i32 %2, i32* getelementptr inbounds ([10000000 x i32], [10000000 x i32]* @a, i64 0, i64 0))
  %3 = load i32, i32* @n, align 4
  %div2 = sdiv i32 %3, 2
  %idxprom = sext i32 %div2 to i64
  %arrayidx = getelementptr inbounds [10000000 x i32], [10000000 x i32]* @a, i64 0, i64 %idxprom
  %4 = load i32, i32* %arrayidx, align 4
  %rem = srem i32 %4, 256
  ret i32 %rem
}

declare i32 @_sysy_getarray(i32*) #1

declare void @_sysy_starttime(i32) #1

declare void @_sysy_stoptime(i32) #1

declare void @_sysy_putarray(i32, i32*) #1

attributes #0 = { noinline nounwind optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Debian clang version 11.0.1-2"}
