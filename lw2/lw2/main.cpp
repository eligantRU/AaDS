/*
   25. Имеется  выражение  в  постфиксной  форме,   включающее
обращение к функциям SIN, COS, EXP.  Переменные  заданы  одной
строчной латинской буквой.  Запросить  значения  переменных  и
вычислить выражение. Представить  его  в  инфиксной  форме  со
скобками. Лишние скобки присутствовать не должны (11).

Никита Егоров, ПС-21
*/

#include "main.hpp"



int main()
{
	string str;
	getline(cin, str);

	try
	{
		map<string, string> vars;

		string tmp;
		while (getline(cin, tmp))
		{
			const auto tokens = TokenizeString(tmp);
			if (tokens.empty()) break;
			vars[tokens[0]] = tokens[1];
		}

		auto tokens = TokenizeString(str);
		cout << ConvertRPN(tokens) << " = ";
		for (const auto & var : vars) replace(tokens.begin(), tokens.end(), var.first, var.second);
		cout << ConvertRPN(tokens) << " = ";
	}
	catch (const exception & ex)
	{
		cerr << ex.what() << endl;
	}
}
