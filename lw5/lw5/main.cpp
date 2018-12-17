/*
   1. ������� ������ ���������,  ��������������� �� ����������
����. ������� ��������� ������ ����������:
   1) ���������� ��������� ��������;
   2) ������� ��������;
   3) �������� �������� (8).

������ ������, ��-21
MSVC C++17
*/

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <optional>

using namespace::std;

extern vector<int> values;

using ReturnedValue = pair<optional<size_t>, pair<size_t, size_t>>;

namespace
{

ReturnedValue Impl(int value)
{
	auto [lastLower, lastUpper] = make_pair(size_t(), size_t());
	auto [lower, upper] = make_pair(size_t(), values.size());

	while (lower != upper)
	{
		auto compared_value = lower + (upper - lower) / 2;
		if (value == values[compared_value])
			return make_pair(compared_value + 1, make_pair(lower, upper));
		else if (value < values[compared_value])
			upper = compared_value;
		else
			lower = compared_value + 1;
		if ((lastLower == lower) && (lastUpper == upper))
			return make_pair(lower, make_pair(lower, upper));
		lastLower = lower;
		lastUpper = upper;
	}
	return make_pair(nullopt, make_pair(lower, upper));
}

size_t FindInsertPosition(int value)
{
	const auto [result, range] = Impl(value);
	const auto [lower, upper] = range;

	if (result) return result.value();
	if (upper == 0) return 0;
	if (lower == values.size()) return values.size();
	throw exception("Unexpected exception");
}

}

vector<int> values;

void Insert(int value)
{
	values.insert(values.begin() + FindInsertPosition(value), value);
}

size_t Find(int value)
{
	const auto [result, _] = Impl(value);
	if (!result) throw exception("Not found");
	return result.value() - 1;
}

void Delete(int value)
{
	values.erase(values.begin() + Find(value));
}

void Print()
{
	for (const auto v : values) cout << v << " ";
	cout << endl;
}

int main()
{
	ifstream strm("1.dat");

	int value;
	while (strm >> value)
	{
		values.push_back(value);
	}
	Print();

	string command;
	cout << " >> ";
	while (cin >> command)
	{
		if (command.empty()) continue;
		
		try
		{
			if (command == "insert")
			{
				cin >> value;
				Insert(value);
			}
			else if (command == "find")
			{
				cin >> value;
				cout << Find(value) << endl;
			}
			else if (command == "delete")
			{
				cin >> value;
				Delete(value);
			}
			else if (command == "exit")
			{
				break;
			}
			else
			{
				throw exception("Unknown command");
			}
			Print();
		}
		catch (const exception & ex)
		{
			cout << ex.what() << endl;
		}
		cout << " >> ";
	}
}
