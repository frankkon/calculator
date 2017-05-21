//����������
//ʵ��һ�����������ۺ�����ļ��㺯����
//ֻҪʹ��main�������Գɹ�ִ�У������ȷ������С�

//�����Ǵ����ܣ����ڴ˻�������Ӵ���ʵ�֡�
//������ɾ�����޸����еĴ��룬�����Ե�0�֡�
//ע�ⲻҪ�����ڴ�й¶�����򵹿�10�֡�
//���������κ����ʣ�����ϵ�׿�

//��Ҫ���ѣ�
//�������Ķ����д��룬�ڳ�����Ļ��������Ӵ���
//�������ֵ�����ǡ�����ͨ������������벻ͨ����0�ּ���
//������ʱ�̱�֤����ͨ����
//�����Ŀ�����ʹ��main�������Գɹ�ִ�У������ȷ�����

//���ֱ�׼
//1. ������ϱ�̹淶���ϸ��ձ���淶�ĵ�ִ��(20��)
//2. �������ͨ��,û�д���͸澯(10��)
//3. ���������ĿҪ�����н����ȷ(40��)
//4. ������쳣��֧�Ĵ���ȫ�����(15��)
//5. C++ͨ��PCLint��飬JAVAͨ��FindBugs��飨5�֣�
//6. ����Ȧ���Ӷ�����Ҫ��5�֣�
//7. ����Ҫ�󣺺������㷨�����ݽṹ������ƺ�����5�֣�

#include "stdafx.h"
//*************************************************************************
//�����Ҫ������������������ͷ�ļ�

//*************************************************************************

using namespace std;

//*************************************************************************
//�����Ҫ���������������ĳ�������

//*************************************************************************

//*************************************************************************
//�����Ҫ�������������ö�١��ࡢ�����ȶ���
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
    //���ַ�У��
    bool beginValid = isBeginValid(formula);

    //ĩ�ַ�У��
    bool endValid = isEndValid(formula);

    //У��κŵ����
    bool incPair = IsIncPair(formula);

    //У����κ���಻��������,�Ҳ��ַ�����Ϊ*��/
    bool leftInc = checkLeftInc(formula);

    //У���ҹκ���಻����+-*/,�Ҳ಻��������
    bool rightInc = checkRightInc(formula);

    //У���Ƿ��������2������������
    bool doubleOper = checkDoubleOper(formula);

    //���ܳ������ֺ�������Ϳո�������ַ�
    //����ʱ�Ѿ�����
    
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
//*****�벻Ҫ�޸�main�����е��κ�����**********
//*****�벻Ҫ�޸�main�����е��κ�����**********
//*****�벻Ҫ�޸�main�����е��κ�����**********
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

    cout<<"�������ۺ�������ʽ,�˳�������quit:"<<endl;
    
    while (cin>>formula)
    {
        if (0 == strcmp(formula, "quit"))
        {
            break;
        }
        cout<<formula<<" = "<<calculator(formula)<<endl;
        cout<<"�������ۺ�������ʽ,�˳�������quit:"<<endl;
    }

    return 0;
}

#endif

