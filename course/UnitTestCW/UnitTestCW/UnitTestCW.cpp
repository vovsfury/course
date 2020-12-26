#include "pch.h"
#include "CppUnitTest.h"
#include "../../Coursework/Coursework/Coursework.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestCW
{
	TEST_CLASS(UnitTestCW)
	{
	public:
		
		TEST_METHOD(split1)
		{
			string infix;
			string expression;
			double result;

			infix = "cos 0";
			result = 1;
			expression = "cos 0";
			inf_to_pref* prefix = new inf_to_pref(infix);

			Assert::AreEqual(expression, prefix->prefix());
			Assert::AreEqual(result, prefix->calculate());
		}

		TEST_METHOD(split3)
		{
			string infix;
			string expression;
			double result;

			infix = "3^2-sqrt(9)+cos0*3+(33/11)-1";
			result = 11;
			expression = "- + + - ^ 3 2 sqrt 9 * cos 0 3 / 33 11 1";
			inf_to_pref* prefix = new inf_to_pref(infix);

			Assert::AreEqual(expression, prefix->prefix());
			Assert::AreEqual(result, prefix->calculate());
		}

		TEST_METHOD(split3)
		{
			string infix;
			string expression;
			double result;

			infix = "cos(0)*(32+21*2)+7";
			result = 81;
			expression = "+ * cos 0 + 32 * 21 2 7";
			inf_to_pref* prefix = new inf_to_pref(infix);

			Assert::AreEqual(expression, prefix->prefix());
			Assert::AreEqual(result, prefix->calculate());
		}
		TEST_METHOD(delete_spaces)
		{
			string infix;
			string expression;
			double result;

			infix = "      (  (3+3)  )  *  sqrt (9)    ";
			result = 18;
			expression = "* + 3 3 sqrt 9";
			inf_to_pref* prefix = new inf_to_pref(infix);

			Assert::AreEqual(expression, prefix->prefix());
			Assert::AreEqual(result, prefix->calculate());
		}

		TEST_METHOD(delete_brackets)
		{
			string infix;
			string expression;
			double result;

			infix = "(((3+3))*sqrt (9))";
			result = 18;
			expression = "* + 3 3 sqrt 9";
			inf_to_pref* prefix = new inf_to_pref(infix);

			Assert::AreEqual(expression, prefix->prefix());
			Assert::AreEqual(result, prefix->calculate());
		}

		TEST_METHOD(delete_brackets_and_spaces)
		{
			string infix;
			string expression;
			double result;

			infix = "      ((  (  (  3+3  )   )   *sqrt (9 )  )    ) ";
			result = 18;
			expression = "* + 3 3 sqrt 9";
			inf_to_pref* prefix = new inf_to_pref(infix);

			Assert::AreEqual(expression, prefix->prefix());
			Assert::AreEqual(result, prefix->calculate());
		}

		TEST_METHOD(calculate_infinite)
		{
			string infix;
			string expression;
			double result;

			infix = "0/3";
			expression = "/ 3 0";
			inf_to_pref* prefix = new inf_to_pref(infix);

			try
			{
				prefix->prefix();
			}
			catch (runtime_error & exception)
			{
				Assert::AreEqual(expression, prefix->prefix());
				Assert::AreEqual("enter the infix notation expression\n", exception.what());
			}
		}

	};
}
