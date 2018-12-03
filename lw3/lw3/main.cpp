#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <memory>
#include <map>

using namespace::std;

vector<string> Tokenize(const string & str, char delimeter = ' ')
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

struct Node
{
	enum class Type
	{
		AND,
		OR,
		NORMAL,
	};

	Node(const string & name, size_t depth, Type type)
		:name(name)
		, depth(depth)
		, type(type)
	{}

	string name;
	size_t depth;
	Type type;
	shared_ptr<Node> parent = nullptr;

	vector<shared_ptr<Node>> childs;

	vector<string> gen;
};

void GenerateCombinations(const vector<vector<string>> & all, size_t index, string str, vector<string> & result)
{
	if (index >= all.size())
	{
		result.push_back(str);
		return;
	}
	for (const auto & bla : all[index]) GenerateCombinations(all, index + 1, str + bla, result);
}

int main()
{
	stack<shared_ptr<Node>> s;
	size_t rootChecker = 0;
	string str;

	shared_ptr<Node> root = nullptr;
	shared_ptr<Node> parent = nullptr;

	vector<shared_ptr<Node>> nodes;

	while (getline(cin, str))
	{
		auto tokens = Tokenize(str);
		auto depth = tokens[0].size();
		auto name = tokens[1];
		auto type = name[0] == '&' ? Node::Type::AND : name[0] == '|' ? Node::Type::OR : Node::Type::NORMAL;
		if (type != Node::Type::NORMAL) name.erase(name.begin());

		shared_ptr<Node> node = make_shared<Node>(name, depth, type);
		nodes.push_back(node);

		if (!rootChecker) { root = node; ++rootChecker; s.emplace(node); continue; }
		++rootChecker;

		auto top = s.top();
		if (depth > top->depth)
		{
			parent = top;
			parent->childs.push_back(node);
		}
		else if (depth == top->depth)
		{
			parent->childs.push_back(node);
		}
		else
		{
			while (s.top()->depth != depth - 1) { s.pop(); }
			parent = s.top();
			parent->childs.push_back(node);
		}
		node->parent = parent;
		s.emplace(node);
	}

	for (const auto & node : nodes)
	{
		string type = node->type == Node::Type::AND ? "&" : node->type == Node::Type::OR ? "|" : "n";
		string parent = (node->parent ? "[" + node->parent->name + "]" : "");
		auto info = "{" + type + "} " + node->name + parent + " (" + to_string(node->depth) + ") : ";
		for (const auto & child : node->childs)
		{
			info += child->name + "(" + to_string(child->depth) + ") ";
		}
		//cout << info << endl;
	}

	vector<shared_ptr<Node>> filteredNodes;
	for (const auto & node : nodes)
	{
		if (node->type != Node::Type::NORMAL)
		{
			filteredNodes.push_back(node);
		}
		else
		{
			node->gen.push_back(node->name);
		}
	}
	reverse(filteredNodes.begin(), filteredNodes.end());

	for (const auto & node : filteredNodes)
	{
		vector<string> res;
		if (node->type == Node::Type::OR)
		{
			vector<string> bla;
			for (const auto & child : node->childs)
			{
				bla.insert(bla.end(), child->gen.begin(), child->gen.end());
			}
			res.insert(res.end(), bla.begin(), bla.end());
		}
		else
		{
			vector<vector<string>> gens;

			for (const auto & child : node->childs)
			{
				gens.push_back(child->gen);
			}
			GenerateCombinations(gens, 0, "", res);
		}
		node->gen = res;
	}
	cout << root->gen.size() << ":" << endl;
	for (const auto & bla : root->gen)
	{
		cout << " * " << bla << endl;
	}
}
