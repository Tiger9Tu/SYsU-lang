; ModuleID = '-'
source_filename = "-"

@a = global i32 0
@b = global i32 0

define i32 @main() {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %c = alloca i32, align 4
  store i32 10, i32* @a, align 4
  store i32 5, i32* @b, align 4
  %cast = load i32, i32* @a, align 4
  %0 = mul i32 %cast, 2
  %cast1 = load i32, i32* @b, align 4
  %1 = add i32 %0, %cast1
  %2 = add i32 %1, 3
  store i32 %2, i32* %c, align 4
  %cast2 = load i32, i32* %c, align 4
  store i32 %cast2, i32* %retval, align 4
  br label %return

return:                                           ; preds = %entry
  %3 = load i32, i32* %retval, align 4
  ret i32 %3
}
