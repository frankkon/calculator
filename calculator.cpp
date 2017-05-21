//试题描述：
//实现一个进行整数综合运算的计算函数。
//只要使得main函数可以成功执行，输出正确结果就行。

//如下是代码框架，请在此基础上添加代码实现。
//不允许删除或修改现有的代码，否则考试得0分。
//注意不要出现内存泄露，否则倒扣10分。
//对试题有任何疑问，请联系孔俊

//重要提醒：
//请认真阅读现有代码，在充分理解的基础上增加代码
//满足评分的入口是“编译通过”，如果编译不通过以0分计算
//所以请时刻保证编译通过。
//程序的目标就是使得main函数可以成功执行，输出正确结果。

//评分标准
//1. 程序符合编程规范，严格按照编码规范文档执行(20分)
//2. 程序编译通过,没有错误和告警(10分)
//3. 程序符合题目要求，运行结果正确(40分)
//4. 程序对异常分支的处理全面合理(15分)
//5. C++通过PCLint检查，JAVA通过FindBugs检查（5分）
//6. 代码圈复杂度满足要求（5分）
//7. 进阶要求：函数、算法、数据结构、类设计合理。（5分）

#include "stdafx.h"
//*************************************************************************
//如果需要，请在这里包含额外的头文件

//*************************************************************************

using namespace std;

//*************************************************************************
//如果需要，请在这里添加你的常量定义

//*************************************************************************

//*************************************************************************
//如果需要，请在这里添加枚举、类、函数等定义
static int level[128] = {0};

static stack<int> values;
static stack<char> operators;

static int bufIndex = 0;
static char numBuf[100] = {0};

void initLevel()
{
    level['!'] = 1; 
    level[')'] = 2;
    level['('] = 3;
    level['+'] = level['-'] = 4;
    level['*'] = level['/'] = 5;
}

inline bool isOper(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

inline bool isLeftInc(char c)
{
    return c == '(';
}

inline bool isRightInc(char c)
{
    return c == ')' ;
}

inline bool isNumber(char c)
{
    return '0' <= c && c <= '9';
}

inline bool isBlank(char c)
{
    return c == ' ';
}

inline bool isSingleOper(char pre, char curr)
{
    return (pre == '(' && (curr == '+' || curr == '-'));
}

inline void cleanBuf()
{
    memset(numBuf, 0, 100);
    bufIndex = 0;
}

void calcByOper(const char c)
{
    int lastNum = 0;
    int currNum = 0;
    int rst = 0;

    currNum = values.top();
    values.pop();
    lastNum = values.top();
    values.pop();

    switch(c) 
    {
    case '+':
        rst = lastNum + currNum;
    	break;
    case '-':
        rst = lastNum - currNum;
    	break;
    case '*':
        rst = lastNum * currNum;
    	break;
    case '/':
        rst = lastNum / currNum;
    	break;
    default:
        return;
    }

    values.push(rst);
}

void calc(char currOper)
{
    char oper = operators.top();

    switch(oper) 
    {
    case '+':
    case '-':
    case '*':
    case '/':
        calcByOper(oper);
    	break;
    case '(':
        operators.pop();
    	return;
    default:
        return;
    }

    cleanBuf();
    operators.pop();

    if (level[operators.top()] >= level[currOper])
    {
        calc(currOper);
    }
    else
    {
        operators.push(currOper);
    }
}

inline void setNumber(char c)
{
    numBuf[bufIndex++] = c; 
}

void calcEnd()
{
    calc('!');
}

bool IsIncPair(const char* formula)
{
    int count = 0;
    while (formula != NULL && *formula != '\0')
    {
        if (*formula == '(')
        {
            if (count < 0)
            {
                return false;
            }
            count++;
        }
        else if (*formula == ')')
        {
            count--;
        }

        formula++;
    }

    return (count == 0);
}

bool isBeginValid(const char* formula)
{
    if (!isLeftInc(formula[0]) && !isNumber(formula[0]) && !isBlank(formula[0])
        && '-' != formula[0] && '+' != formula[0])
    {
        return false;
    }

    return true;
}

bool isEndValid(const char* formula)
{
    unsigned int formulaLen = strlen(formula);

    if (!isRightInc(formula[formulaLen-1]) && !isNumber(formula[formulaLen-1])
        && !isBlank(formula[formulaLen-1]))
    {
        return false;
    }

    return true;
}

bool checkLeftInc(const char* formula)
{
    char c = 0;
    while (formula != NULL && *formula != '\0')
    {
        if (*formula == '(' && isNumber(c))
        {
            return false;
        }

        if ( (*formula == '*' || *formula == '/') && c == '(')
        {
            return false;
        }

        c = *formula;
        formula++;
    }

    return true;
}

bool checkRightInc(const char* formula)
{
    char c = 0;
    while (formula != NULL && *formula != '\0')
    {
        if (*formula == ')' && isOper(c))
        {
            return false;
        }

        if (isNumber(*formula) && c == ')')
        {
            return false;
        }

        c = *formula;
        formula++;
    }

    return true;

}

bool checkDoubleOper(const char* formula)
{
    char c = 0;
    while (formula != NULL && *formula != '\0')
    {
        if (isOper(*formula) && isOper(c))
        {
            return false;
        }

        c = *formula;
        formula++;
    }

    return true;
}

void removeBlank(char* formula)
{
    char buf[100] = {0};
    int i = 0;
    int j = 0;

    while (NULL != formula && formula[i] != '\0')
    {
        if (isBlank(formula[i]))
        {
            i++;
        }
        else
        {
            buf[j] = formula[i];
            i++, j++;
        }
    }

    if (NULL != formula && formula[0] != '\0')
    {
        strcpy(formula, buf);
    }
}

bool IsValidFormula(const char* formula)
{
    //首字符校验
    bool beginValid = isBeginValid(formula);

    //末字符校验
    bool endValid = isEndValid(formula);

    //校验刮号的配对
    bool incPair = IsIncPair(formula);

    //校验左刮号左侧不能是数字,右侧字符不能为*和/
    bool leftInc = checkLeftInc(formula);

    //校验右刮号左侧不能是+-*/,右侧不能是数字
    bool rightInc = checkRightInc(formula);

    //校验是否存在连续2个运算符的情况
    bool doubleOper = checkDoubleOper(formula);

    //不能出现数字和运算符和空格以外的字符
    //计算时已经处理。
    
    return beginValid && endValid && incPair && leftInc && rightInc && doubleOper;
}

void dealOper(bool & preCharIsNum, char currChar)
{
    if (level[ operators.top() ] >= level[ currChar ])
    {
        if (preCharIsNum)
        {
            values.push(atoi(numBuf));
        }
        calc(currChar);
        preCharIsNum = false;
    }
    else
    {
        operators.push(currChar);
        if (preCharIsNum)
        {
            values.push(atoi(numBuf));
        }
        preCharIsNum = false;
        cleanBuf();
    }
}

//*************************************************************************

int calculator(const char* strFormula)
{
    char formula[100] = {0};
    strcpy(formula, strFormula);
    removeBlank(formula);

    if (!IsValidFormula(formula))
    {
        return 0;
    }

    operators.push('!');
    values.push(0);
    cleanBuf();
    initLevel();
    
    bool preCharIsNum = false;
    char preChar = 0;
    unsigned int formulaLen = strlen(formula);

    for (unsigned int i = 0; i < formulaLen; ++i)
    {
        if (isNumber(formula[i]) || isSingleOper(preChar, formula[i]))
        {
            setNumber(formula[i]);
            preCharIsNum = true;
        }
        else if (isOper(formula[i]) || isRightInc(formula[i]))
        {
            dealOper(preCharIsNum, formula[i]);
        }
        else if (isLeftInc(formula[i]))
        {
            operators.push(formula[i]);
            preCharIsNum = false;
        }
        else 
        {
            return 0;
        }
        
        preChar = formula[i];
    }

    if (preCharIsNum)
    {
        values.push(atoi(numBuf));
    }
    
    calcEnd();
    int rst = values.top();
    values.pop();
    
    return rst;
}

#ifdef _DEBUG
//*****请不要修改main函数中的任何内容**********
//*****请不要修改main函数中的任何内容**********
//*****请不要修改main函数中的任何内容**********
int main(int argc, char* argv[])
{
    cout<<"begin testing ..."<<endl;

    assert(calculator("1+2+2") == 5);
    assert(calculator("1-3-2") == -4);
    assert(calculator("2*3*2") == 12);
    assert(calculator("18/3/2") == 3);

    assert(calculator("1-3+2-7+9") == 2);
    assert(calculator("1+5-3+8") == 11);
    assert(calculator("1*8/2") == 4);
    assert(calculator("12/3*2") == 8);

    assert(calculator("1+5*3+8/2-1") == 19);
    assert(calculator("5*4-6/2+8") == 25);

    assert(calculator("(1+1)") == 2);
    assert(calculator("-6/2+8") == 5);

    assert(calculator("(1+5)*3+8/(3-1)") == 22);
    assert(calculator("1+5*(3+8)/(6-1)") == 12);

    assert(calculator("1+a*(3+8)/(6-1)") == 0);
    assert(calculator("1+abc)") == 0);

    assert(calculator("-1") == -1);
    assert(calculator("+1") == 1);
    assert(calculator("1") == 1);

    assert(calculator("()") == 0);
    assert(calculator("(1)") == 1);
    assert(calculator(" (1 + 5) * 3 + 8 / (3 - 1) ") == 22);

    assert(calculator("1+") == 0);
    assert(calculator("*1") == 0);

    assert(calculator("(1+1") == 0);
    assert(calculator("1+1)") == 0);
    assert(calculator("(1+5)3+8/(3-1)") == 0);
    assert(calculator("(1+5)*3+8(3-1)") == 0);
    assert(calculator(")1+1(") == 0);
    assert(calculator("(1+1*)") == 0);
    assert(calculator("(*1+1)") == 0);
    assert(calculator("2 * -1") == 0);

    assert(calculator("2 * (-1)") == -2);
    assert(calculator("4 / (+2)") == 2);
    assert(calculator("1+ (-2)") == -1);
    assert(calculator("4 - (-2)") == 6);

    cout<<"end testing ..."<<endl;
	return 0;
}

#else

int main(int /*argc*/, char* /*argv[]*/)
{
    char formula[100];

    cout<<"请输入综合运算表达式,退出请输入quit:"<<endl;
    
    while (cin>>formula)
    {
        if (0 == strcmp(formula, "quit"))
        {
            break;
        }
        cout<<formula<<" = "<<calculator(formula)<<endl;
        cout<<"请输入综合运算表达式,退出请输入quit:"<<endl;
    }

    return 0;
}

#endif

