@width = constant i32 8, align 4
@height = constant i32 4, align 4
@image_in = global [32 x i32] zeroinitializer, align 16
@image_out = global [32 x i32] zeroinitializer, align 16

; Function Attrs: noinline nounwind optnone
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %num = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %j, align 4
  %call = call i32 @_sysy_getarray(i32* getelementptr inbounds ([32 x i32], [32 x i32]* @image_in, i64 0, i64 0))
  store i32 %call, i32* %num, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %0 = load i32, i32* %j, align 4
  %cmp = icmp slt i32 %0, 8
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %1 = load i32, i32* %j, align 4
  %add = add nsw i32 24, %1
  %idxprom = sext i32 %add to i64
  %arrayidx = getelementptr inbounds [32 x i32], [32 x i32]* @image_in, i64 0, i64 %idxprom
  %2 = load i32, i32* %arrayidx, align 4
  %3 = load i32, i32* %j, align 4
  %add1 = add nsw i32 24, %3
  %idxprom2 = sext i32 %add1 to i64
  %arrayidx3 = getelementptr inbounds [32 x i32], [32 x i32]* @image_out, i64 0, i64 %idxprom2
  store i32 %2, i32* %arrayidx3, align 4
  %4 = load i32, i32* %j, align 4
  %add4 = add nsw i32 %4, 1
  store i32 %add4, i32* %j, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  call void @_sysy_stoptime(i32 20)
  ret i32 1
}

declare i32 @_sysy_getarray(i32*) #1

declare void @_sysy_stoptime(i32) #1

attributes #0 = { noinline nounwind optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Debian clang version 11.0.1-2"}
