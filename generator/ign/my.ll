; ModuleID = '-'
source_filename = "-"

@width = global i32 8
@height = global i32 4
@image_in = global [32 x i32] zeroinitializer
@image_out = global [32 x i32] zeroinitializer

declare void @_sysy_starttime(i32)

declare void @_sysy_stoptime(i32)

declare i32 @_sysy_getch()

declare void @_sysy_putch(i32)

declare i32 @_sysy_getint()

declare void @_sysy_putint(i32)

declare i32 @_sysy_getarray(i32*)

declare void @_sysy_putarray(i32, i32*)

define i32 @main() {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %num = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %j, align 4
  %0 = call i32 @_sysy_getarray(i32* getelementptr inbounds ([32 x i32], [32 x i32]* @image_in, i32 0, i32 0))
  store i32 %0, i32* %num, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %cast = load i32, i32* %j, align 4
  %1 = icmp slt i32 %cast, 8
  br i1 %1, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %cast1 = load i32, i32* %j, align 4
  %2 = add i32 24, %cast1
  %3 = getelementptr inbounds [32 x i32], [32 x i32]* @image_out, i32 0, i32 %2
  %cast2 = load i32, i32* %j, align 4
  %4 = add i32 24, %cast2
  %5 = getelementptr inbounds [32 x i32], [32 x i32]* @image_in, i32 0, i32 %4
  %cast3 = load i32, i32* %5, align 4
  store i32 %cast3, i32* %3, align 4
  %cast4 = load i32, i32* %j, align 4
  %6 = add i32 %cast4, 1
  store i32 %6, i32* %j, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  call void @_sysy_stoptime(i32 20)
  store i32 1, i32* %retval, align 4
  br label %return

return:                                           ; preds = %while.end
  %7 = load i32, i32* %retval, align 4
  ret i32 %7
}
