# U-Language
c++实现解释性U Language
U language 解释器
基本功能:
    1.支持基础变量数据类型 int,string
    2.支持全局变量的声明，赋值，取值，取址，计算等操作
    3.int型变量支持加减乘除等计算
    4.string型变量支持+(实际使用字符串拼接)，size，|按位取大(按ASCII码)，^按位取小
    5.支持if else end if
    6.支持temp变量，临时存储(temp无类型，不用声明)
    7.支持打印mem和符号表

提高要求:+5-8分
 1.支持函数定义，二元函数
 2.支持函数变量类型和语句的存储(内存中)
 3.支持函数调用，实现函数激活(不需要控制链和访问链)，符号表
 4.支持局部变量
 5.支持while语句，不做了
句法:
    1.基本句法:回车后处理
    声明:int a,string s
    赋值:a=5,s=abcd
    取值:a然后回车显示出a的值
    取址:&a
    计算二元运算a+3,s1^s2
    一元运算:size(s)
   list mem/symboltable
 2.复合句法:
   1.if(exp) statement else statement2 endif
   2.exp a==b, a>b
   3.可选while (exp) statements endwhile
函数定义
 define FunName
returnType type
arguType typevar varname
语句
return 变量
end define
函数调用
FunName var1 var2
或者FunName var1，var2
clear在函数定义中用，擦除正文段中的内容
