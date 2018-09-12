#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace::std;

namespace
{

struct ExamInfo
{
	string firstName;
	string lastName;
	int mark = 0;
};

bool IsInRange(int value, int lowerBound, int upperBound)
{
	return (lowerBound <= value) && (value <= upperBound);
}

bool IsValidMark(int value)
{
	return IsInRange(value, 1, 5);
}

vector<ExamInfo> ParseFile(ifstream & input)
{
	vector<ExamInfo> infos;

	string line;
	while (getline(input, line))
	{
		if (line.empty())
		{
			continue;
		}

		stringstream stream(line);

		string token;
		vector<string> tokens;
		while (getline(stream, token, ' '))
		{
			if (!token.empty())
			{
				tokens.push_back(token);
			}
		}

		if (tokens.size() != 5)
		{
			throw range_error("Invalid ExamInfo, valid template: <firstName> <lastName> <mark1> <mark2> <mark3>");
		}

		const auto firstName = tokens[0];
		const auto lastName = tokens[1];
		const auto mark1 = stoi(tokens[2]);
		const auto mark2 = stoi(tokens[3]);
		const auto mark3 = stoi(tokens[4]);

		if (!(IsValidMark(mark1) && IsValidMark(mark2) && IsValidMark(mark3)))
		{
			throw range_error("Mark should be in range 1..5");
		}
		infos.push_back({ firstName, lastName, mark1 + mark2 + mark3 });
	}

	return infos;
}

int GetThirdTopMark(const vector<ExamInfo> & infos)
{
	vector<int> marks;
	for (const auto & info : infos)
	{
		marks.push_back(info.mark);
	}
	sort(marks.begin(), marks.end(), greater<int>());
	marks.erase(unique(marks.begin(), marks.end()), marks.end());
	
	return (marks.size() >= 2) ? marks[2] : marks.back();
}

void PrintTop3Students(vector<ExamInfo> infos)
{
	if (infos.empty())
	{
		return;
	}
	sort(infos.begin(), infos.end(), [](const auto & lhs, const auto & rhs) {
		return lhs.mark > rhs.mark;
	});

	const auto thirdTopMark = GetThirdTopMark(infos);
	const auto count = count_if(infos.begin(), infos.end(), [thirdTopMark](const auto & info) {
		return info.mark >= thirdTopMark;
	});

	for_each(infos.begin(), infos.begin() + count, [](const auto & info) {
		cout << info.firstName << " " << info.lastName << " " << info.mark << endl;
	});
}

}

int main(int argc, char * argv[])
{
	if (argc != 2)
	{
		cerr << "Invalid params. Use this:" << endl;
		cerr << "lw1 <input file>" << endl;
		return 1;
	}

	const string inputFilePath = argv[1];
	
	try
	{
		ifstream input(inputFilePath);
		if (!input.is_open())
		{
			throw runtime_error("Failed to open " + inputFilePath + " for reading");
		}

		PrintTop3Students(ParseFile(input));
	}
	catch (const exception & ex)
	{
		cerr << ex.what() << endl;
		return 1;
	}
	return 0;
}
