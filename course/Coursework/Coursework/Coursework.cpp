#include <iostream>
#include <cmath>
#include <string.h>
#include <stdexcept>
#include <algorithm>
#include <locale>
#include <sstream>
using namespace std;

class inf_to_pref
{

private:
	const char* math_formulas_list[15] = { "abs","sqrt","lg","ln","exp","sin","cos","tg","ctg","asin","acos","atg","actg","pi","e" };
	string infix;
	double value;
	char math_formula;
	inf_to_pref* left;
	inf_to_pref* right;

public:

	inf_to_pref(string input_infix)
	{
		left = NULL;
		right = NULL;
		math_formula = 'Z';
		infix = input_infix;
		infix.erase(remove(infix.begin(), infix.end(), ' '), infix.end());
		split();
		
	}

	// returns expression using depth first search
	string dfs(inf_to_pref* root)
	{
		string infix;
		if (root->math_formula == '-' || root->math_formula == '+' || root->math_formula == '*' || root->math_formula == '/' || root->math_formula == '^' || root->math_formula == '~' || root->math_formula == 'x')
			infix = infix + root->math_formula;
		else
			if (root->math_formula == 'C')
			{
				stringstream new_string;
				new_string << root->value;
				infix += new_string.str();
			}
			else
				infix += math_formulas_list[root->math_formula - 'a'];
		infix += ' ';
		if (root->left) infix += dfs(root->left);
		if (root->right) infix += dfs(root->right);
		return infix;
	}

	
	bool brackets()
	{
		int brackets = 0;
		for (size_t i = 0; i < infix.length(); i++)
		{
			if (infix[i] == '(')
				brackets++;
			else if (infix[i] == ')')
				brackets--;
			if (brackets < 0)
				return false;
		}
		return (brackets == 0);
	}

	//deletes brackets
	void delete_brackets()
	{
		if (!infix.length() || infix[0] != '(' || infix[infix.length() - 1] != ')')
			return;
		infix.erase(infix.begin());
		infix.pop_back();
		if (brackets())
			delete_brackets();
		else
		{
			infix.insert(infix.begin(), '(');
			infix.push_back(')');
		}
	}
	
	//splits string and makes tree, scanning algorythm from the end of string
	void split()
	{
		delete_brackets();
		int brackets = 0;
		int minimal_priority = 3;
		size_t split = string::npos;
		for (int i = infix.length() - 1; i != string::npos; i--)
			switch (infix[i])
			{
			case '(':brackets++; break;
			case ')':brackets--; break;
			default:if (!brackets)
			{
				int cur_priority;	

				switch (infix[i])
				{
				case '-':cur_priority = i && infix[i - 1] == 'e' ? 3 : 0; break;
				case '+':cur_priority = i && infix[i - 1] == 'e' ? 3 : 0; break;
				case '*':cur_priority = 1; break;
				case '/':cur_priority = 1; break;
				case '^':cur_priority = 2; break;
				default:cur_priority = 3; break;
				}
				if (cur_priority < minimal_priority)
				{
					minimal_priority = cur_priority;
					split = i;
				}
			}
				   break;
			}

		//unairy minus 
		if (split != string::npos)
		{
			math_formula = infix[split];
			right = new inf_to_pref(string(infix, split + 1, infix.length() - split - 1));
			if (math_formula == '-' && !split)
				math_formula = '~';
			else
				left = new inf_to_pref(string(infix, 0, split));
			return;
		}

		//for functions
		for (int i = 0; i <= 12; i++)
		{
			if (!infix.find(math_formulas_list[i]))
			{
				math_formula = 'a' + (char)i;
				size_t left = strlen(math_formulas_list[i]);
				right = new inf_to_pref(string(infix, left, infix.length() - left));
				return;
			}
		}

		if (infix == "pi") //for constants
		{
			math_formula = 'C';
			value = 3.14;
		}
		else if (infix == "e")
		{
			math_formula = 'C';
			value = 2.71;
		}

		else
		{
			char* tmp;
			value = strtod(infix.c_str(), &tmp);
			if (*tmp == '\0')
				math_formula = 'C';
		}
	}

	//counts result using depth first search
	double result_count(inf_to_pref* root)
	{
		switch (root->math_formula)
		{
		case '-':return result_count(root->left) - result_count(root->right);
		case '+':return result_count(root->left) + result_count(root->right);
		case '*':return result_count(root->left) * result_count(root->right);
		case '/':return result_count(root->left) / result_count(root->right);
		case '^':return pow(result_count(root->left), result_count(root->right));
		case '~':return -result_count(root->right);
		case 'a':return fabs(result_count(root->right));
		case 'b':return sqrt(result_count(root->right));
		case 'c':return log10(result_count(root->right));
		case 'd':return log(result_count(root->right));
		case 'e':return exp(result_count(root->right));
		case 'f':return sin(result_count(root->right));
		case 'g':return cos(result_count(root->right));
		case 'h':return tan(result_count(root->right));
		case 'i':return 1 / tan(result_count(root->right));
		case 'j':return asin(result_count(root->right));
		case 'k':return acos(result_count(root->right));
		case 'l':return atan(result_count(root->right));
		case 'm':return atan(-result_count(root->right)) + 3.14 / 2;
		case 'C':return root->value;
		default:return 0;
		}
	}

	double calculate()
	{
		double result = result_count(this);
		if (!isfinite(result))
			throw runtime_error("entered expression is infinite\n");
		return result;
	}

	string prefix()
	{
		string infix;
		infix = dfs(this);
		infix.pop_back();
		return infix;
	}
};

int main()
{
	try
	{
		cout << "enter the infix notation expression\n";
		string infix;
		double result;
		getline(cin, infix);
		inf_to_pref* prefix = new inf_to_pref(infix);
		infix = prefix->prefix();
		cout << "prefix form: " << infix << endl;
		cout << "result: ";
		result = prefix->calculate();
		cout << result << endl;
	}
	catch (exception & exception)
	{
		cout << exception.what();
	}
}