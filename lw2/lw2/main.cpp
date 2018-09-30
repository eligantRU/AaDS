/*
   25. Имеется  выражение  в  постфиксной  форме,   включающее
обращение к функциям SIN, COS, EXP.  Переменные  заданы  одной
строчной латинской буквой.  Запросить  значения  переменных  и
вычислить выражение. Представить  его  в  инфиксной  форме  со
скобками. Лишние скобки присутствовать не должны (11).

Никита Егоров, ПС-21
*/

#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <algorithm>
#include <memory>

using namespace::std;

namespace
{

enum OperationPriority
{
	POW = 4,
	DIV = 3,
	MUL = 3,
	MINUS = 2,
	PLUS = 1,
	NONE = 0,
};

}

bool IsOperator(char ch)
{
	return (ch == '+') || (ch == '-') || (ch == '*') || (ch == '/') || (ch == '^');
}

OperationPriority GetOperationPriority(const string & str)
{
	if (str == "^") return OperationPriority::POW;
	if (str == "/") return OperationPriority::DIV;
	if (str == "*") return OperationPriority::MUL;
	if (str == "-") return OperationPriority::MINUS;
	if (str == "+") return OperationPriority::PLUS;
	return OperationPriority::NONE;
}

class IExpression
{
public:
	virtual string op1() const = 0;
	virtual string oper() const = 0;
	virtual string op2() const = 0;

	virtual string ToString() const = 0;
};

class Expression : public IExpression
{
public:
	Expression() = delete;
	Expression(string op1, string oper, string op2)
		:m_op1(op1)
		,m_oper(oper)
		,m_op2(op2)
	{}

	string op1() const override {return m_op1;}
	string oper() const override {return m_oper;}
	string op2() const override {return m_op2;}

	virtual string ToString() const override
	{
		return m_op1 + m_oper + m_op2;
	}

private:
	string m_op1;
	string m_oper;
	string m_op2;
};

struct BracketedExpression : public Expression
{
public:
	BracketedExpression(const string & op1, const string & operation, const string & op2)
		:Expression(op1, operation, op2)
	{}

	string op1() const override { return "(" + Expression::op1(); }
	string op2() const override { return Expression::op2() + ")"; }

	string ToString() const override
	{
		return "(" + Expression::ToString() + ")";
	}
};

string CharToString(char ch)
{
	char str[2];
	str[0] = ch;
	str[1] = 0;
	return string(str);
}

int main()
{
	string str;
	getline(cin, str);
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());

	stack<shared_ptr<IExpression>> stack;
	string oper;
	for (const auto & ch : str)
	{
		if (!IsOperator(ch))
		{
			const auto expr = isdigit(ch)
				? make_shared<Expression>("", to_string(ch - '0'), "")
				: make_shared<Expression>("", CharToString(ch), "");
			stack.push(expr);
		}
		else
		{
			oper = ch;
			auto op1 = stack.top(); stack.pop();
			auto op2 = stack.top(); stack.pop();

			const auto operation1Priority = GetOperationPriority(op1->oper());
			if ((operation1Priority < GetOperationPriority(oper)) && (operation1Priority != 0))
			{
				op1 = make_shared<BracketedExpression>(op1->op1(), op1->oper(), op1->op2());
			}
			const auto operation2Priority = GetOperationPriority(op2->oper());
			if ((operation2Priority < GetOperationPriority(oper)) && (operation2Priority != 0))
			{
				op2 = make_shared<BracketedExpression>(op2->op1(), op2->oper(), op2->op2());
			}

			stack.push(make_shared<Expression>(
				op2->op1() + op2->oper() + op2->op2(),
				oper,
				op1->op1() + op1->oper() + op1->op2()
			));
		}
	}

	if (stack.size() != 1)
	{
		cerr << "Invalid stack state" << endl;
	}
	cout << stack.top()->ToString() << endl;
}
