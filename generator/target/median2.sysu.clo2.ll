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
  %1 = add nsw i32 %y, -1
  %2 = add nsw i32 %x, -1
  %3 = icmp ult i32 %1, %2
  %umin = select i1 %3, i32 %1, i32 %2
  %4 = add nuw i32 %x, 2
  %5 = mul i32 %umin, %4
  %6 = add i32 %5, %x
  %7 = zext i32 %umin to i33
  %8 = add nsw i32 %umin, -1
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
  %cmp11.i = icmp sgt i32 %l, 0
  %cmp112.i = icmp sgt i32 %r, 0
  %2 = and i1 %cmp11.i, %cmp112.i
  br i1 %2, label %while.body.preheader.i, label %meanless_calculation.exit

while.body.preheader.i:                           ; preds = %entry
  %3 = add nsw i32 %r, -1
  %4 = add nsw i32 %l, -1
  %5 = icmp ult i32 %3, %4
  %umin.i = select i1 %5, i32 %3, i32 %4
  %6 = add nuw i32 %l, 2
  %7 = mul i32 %umin.i, %6
  %8 = zext i32 %umin.i to i33
  %9 = add nsw i32 %umin.i, -1
  %10 = zext i32 %9 to i33
  %11 = mul i33 %10, %8
  %12 = lshr i33 %11, 1
  %13 = trunc i33 %12 to i32
  %14 = add nuw i32 %l, 1
  %15 = add i32 %14, %7
  %16 = add i32 %15, %13
  br label %meanless_calculation.exit

meanless_calculation.exit:                        ; preds = %entry, %while.body.preheader.i
  %ret.0.lcssa.i = phi i32 [ 0, %entry ], [ %16, %while.body.preheader.i ]
  ret i32 %ret.0.lcssa.i
}

; Function Attrs: nofree nounwind
define i32 @median(i32* %arr, i32 %begin, i32 %end, i32 %pos) local_unnamed_addr #2 {
entry:
  br label %tailrecurse.outer

tailrecurse.outer:                                ; preds = %if.end28, %entry
  %begin.tr.ph = phi i32 [ %add31, %if.end28 ], [ %begin, %entry ]
  %end.tr.ph = phi i32 [ %end.tr, %if.end28 ], [ %end, %entry ]
  %idxprom = sext i32 %begin.tr.ph to i64
  %arrayidx = getelementptr inbounds i32, i32* %arr, i64 %idxprom
  br label %tailrecurse

tailrecurse:                                      ; preds = %tailrecurse.outer, %while.end21
  %end.tr = phi i32 [ %r.2, %while.end21 ], [ %end.tr.ph, %tailrecurse.outer ]
  %0 = load i32, i32* %arrayidx, align 4, !tbaa !2
  %add = add nsw i32 %end.tr, 1
  br label %while.cond

while.cond:                                       ; preds = %if.else, %tailrecurse
  %l.0 = phi i32 [ %begin.tr.ph, %tailrecurse ], [ %l.2, %if.else ]
  %r.0 = phi i32 [ %add, %tailrecurse ], [ %r.2, %if.else ]
  %xx.0 = phi i32 [ 0, %tailrecurse ], [ %xx.2.lcssa, %if.else ]
  %cmp85 = icmp slt i32 %l.0, %r.0
  br i1 %cmp85, label %while.body2.preheader, label %while.end

while.body2.preheader:                            ; preds = %while.cond
  %1 = sext i32 %r.0 to i64
  %2 = sext i32 %l.0 to i64
  %3 = add i32 %r.0, %xx.0
  %4 = sub i32 %3, %l.0
  br label %while.body2

while.body2:                                      ; preds = %while.body2.preheader, %if.end
  %indvars.iv = phi i64 [ %1, %while.body2.preheader ], [ %indvars.iv.next, %if.end ]
  %xx.187 = phi i32 [ %xx.0, %while.body2.preheader ], [ %add6, %if.end ]
  %indvars.iv.next = add nsw i64 %indvars.iv, -1
  %arrayidx4 = getelementptr inbounds i32, i32* %arr, i64 %indvars.iv.next
  %5 = load i32, i32* %arrayidx4, align 4, !tbaa !2
  %cmp5 = icmp slt i32 %5, %0
  br i1 %cmp5, label %while.end.loopexit.split.loop.exit, label %if.end

if.end:                                           ; preds = %while.body2
  %add6 = add nsw i32 %xx.187, 1
  %cmp = icmp sgt i64 %indvars.iv.next, %2
  br i1 %cmp, label %while.body2, label %while.end

while.end.loopexit.split.loop.exit:               ; preds = %while.body2
  %6 = trunc i64 %indvars.iv.next to i32
  br label %while.end

while.end:                                        ; preds = %if.end, %while.end.loopexit.split.loop.exit, %while.cond
  %xx.1.lcssa = phi i32 [ %xx.0, %while.cond ], [ %xx.187, %while.end.loopexit.split.loop.exit ], [ %4, %if.end ]
  %r.2 = phi i32 [ %r.0, %while.cond ], [ %6, %while.end.loopexit.split.loop.exit ], [ %l.0, %if.end ]
  %cmp891 = icmp slt i32 %l.0, %r.2
  br i1 %cmp891, label %while.body9.preheader, label %while.end17

while.body9.preheader:                            ; preds = %while.end
  %7 = sext i32 %l.0 to i64
  %8 = sext i32 %r.2 to i64
  br label %while.body9

while.body9:                                      ; preds = %if.end15, %while.body9.preheader
  %indvars.iv98 = phi i64 [ %7, %while.body9.preheader ], [ %indvars.iv.next99, %if.end15 ]
  %xx.293 = phi i32 [ %xx.1.lcssa, %while.body9.preheader ], [ %sub16, %if.end15 ]
  %indvars.iv.next99 = add nsw i64 %indvars.iv98, 1
  %arrayidx12 = getelementptr inbounds i32, i32* %arr, i64 %indvars.iv.next99
  %9 = load i32, i32* %arrayidx12, align 4, !tbaa !2
  %cmp13.not = icmp slt i32 %9, %0
  br i1 %cmp13.not, label %if.end15, label %while.end17.loopexit

if.end15:                                         ; preds = %while.body9
  %sub16 = add nsw i32 %xx.293, -1
  %exitcond.not = icmp eq i64 %indvars.iv.next99, %8
  br i1 %exitcond.not, label %while.end21.loopexit, label %while.body9

while.end17.loopexit:                             ; preds = %while.body9
  %10 = trunc i64 %indvars.iv.next99 to i32
  br label %while.end17

while.end17:                                      ; preds = %while.end17.loopexit, %while.end
  %xx.2.lcssa = phi i32 [ %xx.1.lcssa, %while.end ], [ %xx.293, %while.end17.loopexit ]
  %l.2 = phi i32 [ %l.0, %while.end ], [ %10, %while.end17.loopexit ]
  %cmp18 = icmp eq i32 %l.2, %r.2
  br i1 %cmp18, label %while.end21, label %if.else

if.else:                                          ; preds = %while.end17
  %idxprom.i = sext i32 %l.2 to i64
  %arrayidx.i = getelementptr inbounds i32, i32* %arr, i64 %idxprom.i
  %11 = load i32, i32* %arrayidx.i, align 4, !tbaa !2
  %idxprom1.i = sext i32 %r.2 to i64
  %arrayidx2.i = getelementptr inbounds i32, i32* %arr, i64 %idxprom1.i
  %12 = load i32, i32* %arrayidx2.i, align 4, !tbaa !2
  store i32 %12, i32* %arrayidx.i, align 4, !tbaa !2
  store i32 %11, i32* %arrayidx2.i, align 4, !tbaa !2
  br label %while.cond

while.end21.loopexit:                             ; preds = %if.end15
  %13 = add i32 %xx.1.lcssa, %l.0
  %14 = sub i32 %13, %r.2
  br label %while.end21

while.end21:                                      ; preds = %while.end17, %while.end21.loopexit
  %xx.2.lcssa103 = phi i32 [ %14, %while.end21.loopexit ], [ %xx.2.lcssa, %while.end17 ]
  store i32 %0, i32* %arrayidx, align 4, !tbaa !2
  %idxprom1.i77 = sext i32 %r.2 to i64
  %arrayidx2.i78 = getelementptr inbounds i32, i32* %arr, i64 %idxprom1.i77
  %15 = load i32, i32* %arrayidx2.i78, align 4, !tbaa !2
  store i32 %15, i32* %arrayidx, align 4, !tbaa !2
  store i32 %0, i32* %arrayidx2.i78, align 4, !tbaa !2
  %cmp25 = icmp sgt i32 %r.2, %pos
  br i1 %cmp25, label %tailrecurse, label %if.end28

if.end28:                                         ; preds = %while.end21
  %cmp29 = icmp slt i32 %r.2, %pos
  %add31 = add nsw i32 %r.2, 1
  br i1 %cmp29, label %tailrecurse.outer, label %cleanup

cleanup:                                          ; preds = %if.end28
  ret i32 %xx.2.lcssa103
}

; Function Attrs: nounwind
define i32 @main() local_unnamed_addr #3 {
entry:
  %call = tail call i32 @_sysy_getarray(i32* getelementptr inbounds ([10000000 x i32], [10000000 x i32]* @a, i64 0, i64 0)) #5
  store i32 %call, i32* @n, align 4, !tbaa !2
  tail call void @_sysy_starttime(i32 60) #5
  %0 = load i32, i32* @n, align 4, !tbaa !2
  %sub = add nsw i32 %0, -1
  %div = sdiv i32 %0, 2
  br label %tailrecurse.outer.i

tailrecurse.outer.i:                              ; preds = %if.end28.i, %entry
  %begin.tr.ph.i = phi i32 [ %add31.i, %if.end28.i ], [ 0, %entry ]
  %end.tr.ph.i = phi i32 [ %end.tr.i, %if.end28.i ], [ %sub, %entry ]
  %idxprom.i = sext i32 %begin.tr.ph.i to i64
  %arrayidx.i = getelementptr inbounds [10000000 x i32], [10000000 x i32]* @a, i64 0, i64 %idxprom.i
  br label %tailrecurse.i

tailrecurse.i:                                    ; preds = %while.end21.i, %tailrecurse.outer.i
  %end.tr.i = phi i32 [ %r.2.i, %while.end21.i ], [ %end.tr.ph.i, %tailrecurse.outer.i ]
  %1 = load i32, i32* %arrayidx.i, align 4, !tbaa !2
  %add.i = add nsw i32 %end.tr.i, 1
  br label %while.cond.i

while.cond.i:                                     ; preds = %if.else.i, %tailrecurse.i
  %l.0.i = phi i32 [ %begin.tr.ph.i, %tailrecurse.i ], [ %l.2.i, %if.else.i ]
  %r.0.i = phi i32 [ %add.i, %tailrecurse.i ], [ %r.2.i, %if.else.i ]
  %cmp85.i = icmp sgt i32 %r.0.i, %l.0.i
  br i1 %cmp85.i, label %while.body2.preheader.i, label %while.end.i

while.body2.preheader.i:                          ; preds = %while.cond.i
  %2 = sext i32 %r.0.i to i64
  %3 = sext i32 %l.0.i to i64
  br label %while.body2.i

while.body2.i:                                    ; preds = %if.end.i, %while.body2.preheader.i
  %indvars.iv.i = phi i64 [ %2, %while.body2.preheader.i ], [ %indvars.iv.next.i, %if.end.i ]
  %indvars.iv.next.i = add nsw i64 %indvars.iv.i, -1
  %arrayidx4.i = getelementptr inbounds [10000000 x i32], [10000000 x i32]* @a, i64 0, i64 %indvars.iv.next.i
  %4 = load i32, i32* %arrayidx4.i, align 4, !tbaa !2
  %cmp5.i = icmp slt i32 %4, %1
  br i1 %cmp5.i, label %while.end.loopexit.split.loop.exit.i, label %if.end.i

if.end.i:                                         ; preds = %while.body2.i
  %cmp.i = icmp sgt i64 %indvars.iv.next.i, %3
  br i1 %cmp.i, label %while.body2.i, label %while.end.i

while.end.loopexit.split.loop.exit.i:             ; preds = %while.body2.i
  %5 = trunc i64 %indvars.iv.next.i to i32
  br label %while.end.i

while.end.i:                                      ; preds = %if.end.i, %while.end.loopexit.split.loop.exit.i, %while.cond.i
  %r.2.i = phi i32 [ %r.0.i, %while.cond.i ], [ %5, %while.end.loopexit.split.loop.exit.i ], [ %l.0.i, %if.end.i ]
  %cmp891.i = icmp slt i32 %l.0.i, %r.2.i
  br i1 %cmp891.i, label %while.body9.preheader.i, label %while.end17.i

while.body9.preheader.i:                          ; preds = %while.end.i
  %6 = sext i32 %l.0.i to i64
  %7 = sext i32 %r.2.i to i64
  br label %while.body9.i

while.body9.i:                                    ; preds = %if.end15.i, %while.body9.preheader.i
  %indvars.iv98.i = phi i64 [ %6, %while.body9.preheader.i ], [ %indvars.iv.next99.i, %if.end15.i ]
  %indvars.iv.next99.i = add nsw i64 %indvars.iv98.i, 1
  %arrayidx12.i = getelementptr inbounds [10000000 x i32], [10000000 x i32]* @a, i64 0, i64 %indvars.iv.next99.i
  %8 = load i32, i32* %arrayidx12.i, align 4, !tbaa !2
  %cmp13.not.i = icmp slt i32 %8, %1
  br i1 %cmp13.not.i, label %if.end15.i, label %while.end17.loopexit.i

if.end15.i:                                       ; preds = %while.body9.i
  %exitcond.not.i = icmp eq i64 %indvars.iv.next99.i, %7
  br i1 %exitcond.not.i, label %while.end21.i, label %while.body9.i

while.end17.loopexit.i:                           ; preds = %while.body9.i
  %9 = trunc i64 %indvars.iv.next99.i to i32
  br label %while.end17.i

while.end17.i:                                    ; preds = %while.end17.loopexit.i, %while.end.i
  %l.2.i = phi i32 [ %l.0.i, %while.end.i ], [ %9, %while.end17.loopexit.i ]
  %cmp18.i = icmp eq i32 %l.2.i, %r.2.i
  br i1 %cmp18.i, label %while.end21.i.loopexit10, label %if.else.i

if.else.i:                                        ; preds = %while.end17.i
  %idxprom.i.i = sext i32 %l.2.i to i64
  %arrayidx.i.i = getelementptr inbounds [10000000 x i32], [10000000 x i32]* @a, i64 0, i64 %idxprom.i.i
  %10 = load i32, i32* %arrayidx.i.i, align 4, !tbaa !2
  %idxprom1.i.i = sext i32 %r.2.i to i64
  %arrayidx2.i.i = getelementptr inbounds [10000000 x i32], [10000000 x i32]* @a, i64 0, i64 %idxprom1.i.i
  %11 = load i32, i32* %arrayidx2.i.i, align 4, !tbaa !2
  store i32 %11, i32* %arrayidx.i.i, align 4, !tbaa !2
  store i32 %10, i32* %arrayidx2.i.i, align 4, !tbaa !2
  br label %while.cond.i

while.end21.i.loopexit10:                         ; preds = %while.end17.i
  %.pre = sext i32 %r.2.i to i64
  br label %while.end21.i

while.end21.i:                                    ; preds = %if.end15.i, %while.end21.i.loopexit10
  %idxprom1.i77.i.pre-phi = phi i64 [ %.pre, %while.end21.i.loopexit10 ], [ %7, %if.end15.i ]
  store i32 %1, i32* %arrayidx.i, align 4, !tbaa !2
  %arrayidx2.i78.i = getelementptr inbounds [10000000 x i32], [10000000 x i32]* @a, i64 0, i64 %idxprom1.i77.i.pre-phi
  %12 = load i32, i32* %arrayidx2.i78.i, align 4, !tbaa !2
  store i32 %12, i32* %arrayidx.i, align 4, !tbaa !2
  store i32 %1, i32* %arrayidx2.i78.i, align 4, !tbaa !2
  %cmp25.i = icmp sgt i32 %r.2.i, %div
  br i1 %cmp25.i, label %tailrecurse.i, label %if.end28.i

if.end28.i:                                       ; preds = %while.end21.i
  %cmp29.i = icmp slt i32 %r.2.i, %div
  %add31.i = add nsw i32 %r.2.i, 1
  br i1 %cmp29.i, label %tailrecurse.outer.i, label %median.exit

median.exit:                                      ; preds = %if.end28.i
  tail call void @_sysy_stoptime(i32 62) #5
  %13 = load i32, i32* @n, align 4, !tbaa !2
  tail call void @_sysy_putarray(i32 %13, i32* getelementptr inbounds ([10000000 x i32], [10000000 x i32]* @a, i64 0, i64 0)) #5
  %14 = load i32, i32* @n, align 4, !tbaa !2
  %div2 = sdiv i32 %14, 2
  %idxprom = sext i32 %div2 to i64
  %arrayidx = getelementptr inbounds [10000000 x i32], [10000000 x i32]* @a, i64 0, i64 %idxprom
  %15 = load i32, i32* %arrayidx, align 4, !tbaa !2
  %rem = srem i32 %15, 256
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
