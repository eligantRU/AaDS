#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <sstream>
#include <iomanip>

using namespace::std;

bool g_needCalc = false;

enum OperationPriority
{
	EXP = 5,
	COS = 5,
	SIN = 5,
	POW = 4,
	DIV = 3,
	MUL = 3,
	MINUS = 2,
	PLUS = 1,
	NONE = 0,
};

bool IsUnaryOperation(const string & str)
{
	return (str == "cos") || (str == "sin") || (str == "exp");
}

bool IsOperator(const string & str)
{
	return (str == "+") || (str == "-")
		|| (str == "*") || (str == "/") || (str == "^")
		|| (str == "cos") || (str == "sin") || (str == "exp");
}

bool IsFloat(const string & str)
{
	istringstream stream(str);
	float value;
	stream >> noskipws >> value;
	return stream.eof() && !stream.fail();
}

OperationPriority GetOperationPriority(const string & str)
{
	if (str == "cos") return OperationPriority::COS;
	if (str == "sin") return OperationPriority::SIN;
	if (str == "exp") return OperationPriority::EXP;
	if (str == "^") return OperationPriority::POW;
	if (str == "/") return OperationPriority::DIV;
	if (str == "*") return OperationPriority::MUL;
	if (str == "-") return OperationPriority::MINUS;
	if (str == "+") return OperationPriority::PLUS;
	return OperationPriority::NONE;
}

float DoOperation(float op1, float op2, const string & op)
{
	if (op == "+") return op1 + op2;
	if (op == "-") return op1 - op2;
	if (op == "*") return op1 * op2;
	if (op == "/") return op1 / op2;
	if (op == "^") return powf(op1, op2);
	if (op == "exp") return expf(op2);
	if (op == "sin") return sinf(op2);
	if (op == "cos") return cosf(op2);
	throw invalid_argument("Unknown operator '" + op + "'");
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
	{
		m_op1 = op1;
		m_op2 = op2;
		m_oper = oper;
		
		float o1 = 0;
		float o2 = 0;
		if (!oper.empty())
		{
			try
			{
				if (!op1.empty() && !op2.empty())
				{
					o1 = stof(op1);
					o2 = stof(op2);
				}
				else if (!op1.empty())
				{
					o1 = stof(op1);
				}
				else if (!op2.empty())
				{
					o2 = stof(op2);
				}
			}
			catch (...) {}
		}
		else
		{
			try
			{
				o2 = stof(op2);
			}
			catch (...) {}
		}

		try
		{
			if (g_needCalc)
			{
				m_result = (!oper.empty())
					? to_string(DoOperation(o1, o2, oper))
					: to_string(o2);
				m_op1 = "";
				m_oper = "";
				m_op2 = m_result;
			}
		}
		catch (...) {}
	}

	string op1() const override { return m_op1; }
	string oper() const override { return m_oper; }
	string op2() const override { return m_op2; }

	virtual string ToString() const override
	{
		return m_op1 + m_oper + m_op2;
	}

private:
	string m_op1;
	string m_oper;
	string m_op2;

	string m_result = "[NotCalculated]";
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

vector<string> TokenizeString(const string & str, char delimeter = ' ')
{
	vector<string> tokens;

	stringstream stream(str);
	string tmp;

	while (getline(stream, tmp, delimeter))
	{
		tokens.push_back(tmp);
	}
	return tokens;
}

void ProcessBinaryOperation(stack<shared_ptr<IExpression>> & stack, const string & oper)
{
	if (stack.size() < 2)
	{
		throw runtime_error("Invalid stack size: cannot get operands");
	}
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
		(op2->oper() == "^")
			? "(" + op2->op1() + op2->oper() + op2->op2() + ")"
			: op2->op1() + op2->oper() + op2->op2(),
		oper,
		op1->op1() + op1->oper() + op1->op2()
	));
}

void ProcessUnaryOperation(stack<shared_ptr<IExpression>> & stack, const string & oper)
{
	if (stack.empty())
	{
		throw runtime_error("Invalid stack size: cannot get operands");
	}
	auto op = stack.top(); stack.pop();

	const auto operationPriority = GetOperationPriority(op->oper());
	if ((operationPriority < GetOperationPriority(oper)) && (operationPriority != 0))
	{
		op = make_shared<BracketedExpression>(op->op1(), op->oper(), op->op2());
	}

	stack.push(make_shared<Expression>(
		"",
		oper,
		op->op1() + op->oper() + op->op2()
	));
}

string ConvertRPN(const vector<string> & tokens, bool needCalc = false)
{
	g_needCalc = needCalc;
	stack<shared_ptr<IExpression>> stack;
	string oper;
	for (const auto & token : tokens)
	{
		if (!IsOperator(token))
		{
			stack.push(make_shared<Expression>("", "", token));
		}
		else
		{
			oper = token;

			if (IsUnaryOperation(oper))
			{
				ProcessUnaryOperation(stack, oper);
			}
			else
			{
				ProcessBinaryOperation(stack, oper);
			}
		}
	}
	if (stack.size() != 1)
	{
		throw runtime_error("Invalid stack state: should contains only one element");
	}
	return stack.top()->ToString();
}

float CalculateRPN(const vector<string> & tokens)
{
	return stof(ConvertRPN(tokens, true));
}
