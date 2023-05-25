; ModuleID = '-'
source_filename = "-"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@a = global [10000000 x i32] zeroinitializer, align 16
@n = local_unnamed_addr global i32 0, align 4

; Function Attrs: norecurse nounwind readnone
define i32 @meanless_calculation(i32 %x, i32 %y) local_unnamed_addr #0 {
entry:
  %cmp11 = icmp sgt i32 %x, 0
  %cmp112 = icmp sgt i32 %y, 0
  %0 = and i1 %cmp11, %cmp112
  br i1 %0, label %while.body.preheader, label %while.end

while.body.preheader:                             ; preds = %entry
  %1 = add i32 %y, -1
  %2 = add i32 %x, -1
  %3 = icmp ult i32 %1, %2
  %umin = select i1 %3, i32 %1, i32 %2
  %4 = add i32 %x, 2
  %5 = mul i32 %umin, %4
  %6 = add i32 %5, %x
  %7 = zext i32 %umin to i33
  %8 = add i32 %umin, -1
  %9 = zext i32 %8 to i33
  %10 = mul i33 %7, %9
  %11 = lshr i33 %10, 1
  %12 = trunc i33 %11 to i32
  %13 = add i32 %6, %12
  %14 = add i32 %13, 1
  br label %while.end

while.end:                                        ; preds = %while.body.preheader, %entry
  %ret.0.lcssa = phi i32 [ 0, %entry ], [ %14, %while.body.preheader ]
  ret i32 %ret.0.lcssa
}

; Function Attrs: nofree norecurse nounwind
define i32 @swap(i32* nocapture %arr, i32 %l, i32 %r) local_unnamed_addr #1 {
entry:
  %idxprom = sext i32 %l to i64
  %arrayidx = getelementptr inbounds i32, i32* %arr, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4, !tbaa !2
  %idxprom1 = sext i32 %r to i64
  %arrayidx2 = getelementptr inbounds i32, i32* %arr, i64 %idxprom1
  %1 = load i32, i32* %arrayidx2, align 4, !tbaa !2
  store i32 %1, i32* %arrayidx, align 4, !tbaa !2
  store i32 %0, i32* %arrayidx2, align 4, !tbaa !2
  %call = call i32 @meanless_calculation(i32 %l, i32 %r)
  ret i32 %call
}

; Function Attrs: nofree nounwind
define i32 @median(i32* %arr, i32 %begin, i32 %end, i32 %pos) local_unnamed_addr #2 {
entry:
  %idxprom = sext i32 %begin to i64
  %arrayidx = getelementptr inbounds i32, i32* %arr, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4, !tbaa !2
  %add = add nsw i32 %end, 1
  br label %while.cond

while.cond:                                       ; preds = %if.else, %entry
  %l.0 = phi i32 [ %begin, %entry ], [ %l.2, %if.else ]
  %r.0 = phi i32 [ %add, %entry ], [ %r.2, %if.else ]
  %xx.0 = phi i32 [ 0, %entry ], [ %xx.2.lcssa, %if.else ]
  %cmp75 = icmp slt i32 %l.0, %r.0
  br i1 %cmp75, label %while.body2.preheader, label %while.end

while.body2.preheader:                            ; preds = %while.cond
  %1 = sext i32 %r.0 to i64
  %2 = sext i32 %l.0 to i64
  %3 = add i32 %r.0, %xx.0
  %4 = sub i32 %3, %l.0
  br label %while.body2

while.body2:                                      ; preds = %while.body2.preheader, %if.end
  %indvars.iv = phi i64 [ %1, %while.body2.preheader ], [ %indvars.iv.next, %if.end ]
  %xx.177 = phi i32 [ %xx.0, %while.body2.preheader ], [ %add6, %if.end ]
  %indvars.iv.next = add nsw i64 %indvars.iv, -1
  %arrayidx4 = getelementptr inbounds i32, i32* %arr, i64 %indvars.iv.next
  %5 = load i32, i32* %arrayidx4, align 4, !tbaa !2
  %cmp5 = icmp slt i32 %5, %0
  %6 = trunc i64 %indvars.iv.next to i32
  br i1 %cmp5, label %while.end, label %if.end

if.end:                                           ; preds = %while.body2
  %add6 = add nsw i32 %xx.177, 1
  %cmp = icmp sgt i64 %indvars.iv.next, %2
  br i1 %cmp, label %while.body2, label %while.end, !llvm.loop !6

while.end:                                        ; preds = %if.end, %while.body2, %while.cond
  %xx.1.lcssa = phi i32 [ %xx.0, %while.cond ], [ %xx.177, %while.body2 ], [ %4, %if.end ]
  %r.2 = phi i32 [ %r.0, %while.cond ], [ %6, %while.body2 ], [ %l.0, %if.end ]
  %cmp881 = icmp slt i32 %l.0, %r.2
  br i1 %cmp881, label %while.body9.preheader, label %while.end17

while.body9.preheader:                            ; preds = %while.end
  %7 = sext i32 %l.0 to i64
  %8 = sext i32 %r.2 to i64
  %9 = add i32 %xx.1.lcssa, %l.0
  %10 = sub i32 %9, %r.2
  br label %while.body9

while.body9:                                      ; preds = %if.end15, %while.body9.preheader
  %indvars.iv88 = phi i64 [ %7, %while.body9.preheader ], [ %indvars.iv.next89, %if.end15 ]
  %xx.283 = phi i32 [ %xx.1.lcssa, %while.body9.preheader ], [ %sub16, %if.end15 ]
  %indvars.iv.next89 = add nsw i64 %indvars.iv88, 1
  %arrayidx12 = getelementptr inbounds i32, i32* %arr, i64 %indvars.iv.next89
  %11 = load i32, i32* %arrayidx12, align 4, !tbaa !2
  %cmp13.not = icmp slt i32 %11, %0
  %12 = trunc i64 %indvars.iv.next89 to i32
  br i1 %cmp13.not, label %if.end15, label %while.end17

if.end15:                                         ; preds = %while.body9
  %sub16 = add nsw i32 %xx.283, -1
  %exitcond.not = icmp eq i64 %indvars.iv.next89, %8
  br i1 %exitcond.not, label %while.end17, label %while.body9, !llvm.loop !8

while.end17:                                      ; preds = %while.body9, %if.end15, %while.end
  %xx.2.lcssa = phi i32 [ %xx.1.lcssa, %while.end ], [ %xx.283, %while.body9 ], [ %10, %if.end15 ]
  %l.2 = phi i32 [ %l.0, %while.end ], [ %12, %while.body9 ], [ %r.2, %if.end15 ]
  %cmp18 = icmp eq i32 %l.2, %r.2
  br i1 %cmp18, label %while.end21, label %if.else

if.else:                                          ; preds = %while.end17
  %call = call i32 @swap(i32* %arr, i32 %l.2, i32 %r.2)
  br label %while.cond, !llvm.loop !9

while.end21:                                      ; preds = %while.end17
  store i32 %0, i32* %arrayidx, align 4, !tbaa !2
  %call24 = call i32 @swap(i32* %arr, i32 %begin, i32 %l.2)
  %cmp25 = icmp sgt i32 %l.2, %pos
  br i1 %cmp25, label %if.then26, label %if.end28

if.then26:                                        ; preds = %while.end21
  %call27 = call i32 @median(i32* %arr, i32 %begin, i32 %l.2, i32 %pos)
  br label %cleanup

if.end28:                                         ; preds = %while.end21
  %cmp29 = icmp slt i32 %l.2, %pos
  br i1 %cmp29, label %if.then30, label %cleanup

if.then30:                                        ; preds = %if.end28
  %add31 = add nsw i32 %l.2, 1
  %call32 = call i32 @median(i32* %arr, i32 %add31, i32 %end, i32 %pos)
  br label %cleanup

cleanup:                                          ; preds = %if.end28, %if.then30, %if.then26
  %retval.0 = phi i32 [ %call27, %if.then26 ], [ %call32, %if.then30 ], [ %xx.2.lcssa, %if.end28 ]
  ret i32 %retval.0
}

; Function Attrs: nounwind
define i32 @main() local_unnamed_addr #3 {
entry:
  %call = call i32 @_sysy_getarray(i32* getelementptr inbounds ([10000000 x i32], [10000000 x i32]* @a, i64 0, i64 0)) #5
  store i32 %call, i32* @n, align 4, !tbaa !2
  call void @_sysy_starttime(i32 60) #5
  %0 = load i32, i32* @n, align 4, !tbaa !2
  %sub = add nsw i32 %0, -1
  %div = sdiv i32 %0, 2
  %call1 = call i32 @median(i32* getelementptr inbounds ([10000000 x i32], [10000000 x i32]* @a, i64 0, i64 0), i32 0, i32 %sub, i32 %div)
  call void @_sysy_stoptime(i32 62) #5
  %1 = load i32, i32* @n, align 4, !tbaa !2
  call void @_sysy_putarray(i32 %1, i32* getelementptr inbounds ([10000000 x i32], [10000000 x i32]* @a, i64 0, i64 0)) #5
  %2 = load i32, i32* @n, align 4, !tbaa !2
  %div2 = sdiv i32 %2, 2
  %idxprom = sext i32 %div2 to i64
  %arrayidx = getelementptr inbounds [10000000 x i32], [10000000 x i32]* @a, i64 0, i64 %idxprom
  %3 = load i32, i32* %arrayidx, align 4, !tbaa !2
  %rem = srem i32 %3, 256
  ret i32 %rem
}

declare i32 @_sysy_getarray(i32*) local_unnamed_addr #4

declare void @_sysy_starttime(i32) local_unnamed_addr #4

declare void @_sysy_stoptime(i32) local_unnamed_addr #4

declare void @_sysy_putarray(i32, i32*) local_unnamed_addr #4

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nofree norecurse nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nofree nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Debian clang version 11.0.1-2"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.unroll.disable"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
