; ModuleID = '<stdin>'
source_filename = "-"

@n = global i32 0
@a = global [10000000 x i32] zeroinitializer

declare void @_sysy_starttime(i32)

declare void @_sysy_stoptime(i32)

declare i32 @_sysy_getarray(i32*)

declare void @_sysy_putarray(i32, i32*)

define i32 @meanless_calculation(i32 %0, i32 %1) {
entry:
  br label %while.cond

while.cond:                                       ; preds = %while.then, %entry
  %ret.0 = phi i32 [ 0, %entry ], [ %6, %while.then ]
  %i.0 = phi i32 [ 0, %entry ], [ %4, %while.then ]
  %2 = icmp slt i32 %i.0, %0
  br i1 %2, label %land.lhs.true, label %while.end

land.lhs.true:                                    ; preds = %while.cond
  %3 = icmp slt i32 %i.0, %1
  br i1 %3, label %while.then, label %while.end

while.then:                                       ; preds = %land.lhs.true
  %4 = add i32 %i.0, 1
  %5 = add i32 %ret.0, %0
  %6 = add i32 %5, %4
  br label %while.cond

while.end:                                        ; preds = %land.lhs.true, %while.cond
  br label %return

return:                                           ; preds = %while.end
  ret i32 %ret.0
}

define i32 @swap(i32* %0, i32 %1, i32 %2) {
entry:
  %3 = getelementptr inbounds i32, i32* %0, i32 %1
  %cast = load i32, i32* %3, align 4
  %4 = getelementptr inbounds i32, i32* %0, i32 %1
  %5 = getelementptr inbounds i32, i32* %0, i32 %2
  %cast6 = load i32, i32* %5, align 4
  store i32 %cast6, i32* %4, align 4
  %6 = getelementptr inbounds i32, i32* %0, i32 %2
  store i32 %cast, i32* %6, align 4
  %7 = call i32 @meanless_calculation(i32 %1, i32 %2)
  br label %return

return:                                           ; preds = %entry
  ret i32 %7
}

define i32 @median(i32* %0, i32 %1, i32 %2, i32 %3) {
entry:
  %4 = getelementptr inbounds i32, i32* %0, i32 %1
  %cast = load i32, i32* %4, align 4
  %5 = add i32 %2, 1
  br label %while.cond

while.cond:                                       ; preds = %if.end29, %entry
  %xx.1 = phi i32 [ 0, %entry ], [ %xx.0, %if.end29 ]
  %r.2 = phi i32 [ %5, %entry ], [ %r.1, %if.end29 ]
  %l.2 = phi i32 [ %1, %entry ], [ %l.1, %if.end29 ]
  br i1 true, label %while.then, label %while.end

while.then:                                       ; preds = %while.cond
  br label %while.cond4

while.cond4:                                      ; preds = %if.end, %while.then
  %xx.3 = phi i32 [ %xx.1, %while.then ], [ %10, %if.end ]
  %r.0 = phi i32 [ %r.2, %while.then ], [ %7, %if.end ]
  %6 = icmp slt i32 %l.2, %r.0
  br i1 %6, label %while.then5, label %while.end6

while.then5:                                      ; preds = %while.cond4
  %7 = sub i32 %r.0, 1
  %8 = getelementptr inbounds i32, i32* %0, i32 %7
  %cast12 = load i32, i32* %8, align 4
  %9 = icmp slt i32 %cast12, %cast
  br i1 %9, label %if.then, label %if.end

if.then:                                          ; preds = %while.then5
  br label %while.end6

if.end:                                           ; preds = %while.then5
  %10 = add i32 %xx.3, 1
  br label %while.cond4

while.end6:                                       ; preds = %if.then, %while.cond4
  %r.1 = phi i32 [ %r.0, %while.cond4 ], [ %7, %if.then ]
  br label %while.cond15

while.cond15:                                     ; preds = %if.end22, %while.end6
  %xx.0 = phi i32 [ %xx.3, %while.end6 ], [ %15, %if.end22 ]
  %l.0 = phi i32 [ %l.2, %while.end6 ], [ %12, %if.end22 ]
  %11 = icmp slt i32 %l.0, %r.1
  br i1 %11, label %while.then16, label %while.end17

while.then16:                                     ; preds = %while.cond15
  %12 = add i32 %l.0, 1
  %13 = getelementptr inbounds i32, i32* %0, i32 %12
  %cast25 = load i32, i32* %13, align 4
  %14 = icmp sge i32 %cast25, %cast
  br i1 %14, label %if.then21, label %if.end22

if.then21:                                        ; preds = %while.then16
  br label %while.end17

if.end22:                                         ; preds = %while.then16
  %15 = sub i32 %xx.0, 1
  br label %while.cond15

while.end17:                                      ; preds = %if.then21, %while.cond15
  %l.1 = phi i32 [ %l.0, %while.cond15 ], [ %12, %if.then21 ]
  %16 = icmp eq i32 %l.1, %r.1
  br i1 %16, label %if.then28, label %if.else

if.then28:                                        ; preds = %while.end17
  br label %while.end

if.else:                                          ; preds = %while.end17
  %17 = call i32 @swap(i32* %0, i32 %l.1, i32 %r.1)
  br label %if.end29

if.end29:                                         ; preds = %if.else
  br label %while.cond

while.end:                                        ; preds = %if.then28, %while.cond
  %xx.2 = phi i32 [ %xx.1, %while.cond ], [ %xx.0, %if.then28 ]
  %l.3 = phi i32 [ %l.2, %while.cond ], [ %l.1, %if.then28 ]
  %18 = getelementptr inbounds i32, i32* %0, i32 %1
  store i32 %cast, i32* %18, align 4
  %19 = call i32 @swap(i32* %0, i32 %1, i32 %l.3)
  %20 = icmp sgt i32 %l.3, %3
  br i1 %20, label %if.then41, label %if.end42

if.then41:                                        ; preds = %while.end
  %21 = call i32 @median(i32* %0, i32 %1, i32 %l.3, i32 %3)
  br label %return

if.end42:                                         ; preds = %while.end
  %22 = icmp slt i32 %l.3, %3
  br i1 %22, label %if.then49, label %if.end50

if.then49:                                        ; preds = %if.end42
  %23 = add i32 %l.3, 1
  %24 = call i32 @median(i32* %0, i32 %23, i32 %2, i32 %3)
  br label %return

if.end50:                                         ; preds = %if.end42
  br label %return

return:                                           ; preds = %if.end50, %if.then49, %if.then41
  %retval.0 = phi i32 [ %21, %if.then41 ], [ %24, %if.then49 ], [ %xx.2, %if.end50 ]
  ret i32 %retval.0
}

define i32 @main() {
entry:
  %0 = call i32 @_sysy_getarray(i32* getelementptr inbounds ([10000000 x i32], [10000000 x i32]* @a, i32 0, i32 0))
  store i32 %0, i32* @n, align 4
  call void @_sysy_starttime(i32 60)
  %cast = load i32, i32* @n, align 4
  %1 = sub i32 %cast, 1
  %2 = sdiv i32 %cast, 2
  %3 = call i32 @median(i32* getelementptr inbounds ([10000000 x i32], [10000000 x i32]* @a, i32 0, i32 0), i32 0, i32 %1, i32 %2)
  call void @_sysy_stoptime(i32 62)
  call void @_sysy_putarray(i32 %cast, i32* getelementptr inbounds ([10000000 x i32], [10000000 x i32]* @a, i32 0, i32 0))
  %4 = getelementptr inbounds [10000000 x i32], [10000000 x i32]* @a, i32 0, i32 %2
  %cast4 = load i32, i32* %4, align 4
  %5 = srem i32 %cast4, 256
  br label %return

return:                                           ; preds = %entry
  ret i32 %5
}
