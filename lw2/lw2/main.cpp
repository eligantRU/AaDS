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
		map<string, int> vars;

		string tmp;
		while (getline(cin, tmp))
		{
			const auto tokens = TokenizeString(tmp);
			if (tokens.empty()) break;
			vars[tokens[0]] = stoi(tokens[1]);
		}

		cout << ConvertRPN(TokenizeString(str)) << endl;
	}
	catch (const exception & ex)
	{
		cerr << ex.what() << endl;
	}
}
