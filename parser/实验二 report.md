# 实验二 report

涂越20337238




## 完成度

1.（基本）完成parser1

2.（挑战）完成parser2

3.（挑战）添加valueCatagory的识别

4.（挑战-较简单）使用vscode插件进行代码风格整理

## 实验过程

1.使用顾宇浩TA提供的模板，学习了visit模式
2.学习了attach模式进行debug
3.编写代码，遇到问题时查看llvm、cppreferece等文档，然后重构

## 难点及解决办法

1.ImplicitCast

要进行implicit cast的节点只有表达式（expr），因此我首先在每个表达式的结构体内实现相应的转换函数。然后在嵌套expr的顶部执行递归地执行转换函数即可。

2.InitListExpr

根据cppreference对数组初始化的定义，编写递归处理的函数即可。

3.作用域:作用域的存储很简单，使用嵌套哈希即可（书上也是这么说的）。但是作用域的识别较为困难，我的解决方法是根据L_PAREN,R_PAREN,L_BRACE,R_BRACE,SEMI这五个token构建状态机栈，从而进行作用域层的压入与弹出。这样做的优势在于不会有多余的开销（如果是空作用域就不会新建哈希表）。

## 结果截图

parser1/2都是一样的截图

![image-20230409205913482](C:\Users\tuyue\AppData\Roaming\Typora\typora-user-images\image-20230409205913482.png)
