# Compiler

&emsp;&emsp;在《编译原理》课程的学习下，逐渐的掌握一个编译器的结构、作用和实现方法。同时，希望自己在不断的努力下写出一个简单的C语言编译器。


### 主要功能

1. 支持`一元运算符`（++，--，!，-），其中++和--有前缀和后缀两种功能
2. 支持`逻辑运算符`：||和&&
3. 支持`算术运算符`：+，-，*，/，%
4. 支持`关系运算符`：>，>=，<，<=，==，！=
5. 控制流：`for`循环，`while`循环
6. 支持`判断语句`if
7. `赋值`操作及`一维数组`的部分操作

### 上下文无关文法

#### 一、基本框架

    Program -> Type main() Block
    Type -> int | bool
    Block -> { Stmts return Num ; }
    Decl -> Type Array ;
    Array -> Identifier [ Num ] | Identifier [ Identifier ] | Identifier
    Stmts -> Stmts M Stmt | Stmt




#### 二、标识符和常数

    Letter -> A | B | C | D | E | F | G | H | I | J | K | L | M | N 
        | O | P | Q | R | S | T | U | V | W | X | Y | Z | a | b
        | c | d | e | f | g | h | i | j | k | l | m | n | o | p
        | q | r | s | t | u | v | w | x | y | z | _
    Digit -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
    Non_digit -> 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
    Num -> Num Digit | Non_digit
    Identifier -> Identifier Digit | Identifier Letter | Letter
    Bool_value -> true | false

#### 三、运算符

    Self_op -> ++ | --
	HLogic_op -> &&
    LLogic_op -> ||
	HMath_op -> * | / | %
    LMath_op -> + | - 
    Judge_op -> == | != | >= | <= | > | <


#### 四、语句块框架

    Stmt -> Assignment ;
		| Decl
        | if ( Bool ) M Stmt
        | if ( Bool ) M Stmt N else M Stmt
        | while M ( Bool ) M Stmt
        | for ( Fora ; M Forb ; Forc ) M Stmt
        | { Stmts }
    Fora -> Assignment | ε
    Forb -> Bool | ε
    Forc -> ForAssignment | ε


#### 五、赋值语句


    Assignment -> LArray = Bool ; | LArray Self_op ; | Self_op LArray ; 

    ForAssignment -> LArray = Bool ; | LArray Self_op ; | Self_op LArray ; 

#### 六、条件语句

    Factor -> ( Bool ) | Array | Num | Bool_value | ! ( Bool )
    HExpr -> HExpr HMath_op Factor | Factor
	LExpr -> LExpr LMath_op HExpr | HExpr
    Rel -> Rel Judge_op LExpr | LExpr
	HRel -> HRel HLogic_op M Rel | Rel
    Bool -> Bool LLogic_op M HRel | HRel


#### 七、辅助

	M -> ε 
	N -> ε	
