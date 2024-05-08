/***************
*
*				The Shunting Yard Algorithm implementation!
*				Developed by Mertcan Zafer
*				Influenced by javidx9				
*/


#include <iostream>
#include <queue>
#include <algorithm>
#include <string>
#include <array>
#include <deque>
#include<unordered_map>

struct sOperator
{
	uint8_t presedence = 0;
	uint8_t	arguments = 0;
};

int main()
{
	std::unordered_map<char, sOperator> mapOps
	{
		{'/',{4,2}},
		{'*',{3,2}},
		{'+',{2,2}},
		{'-',{1,2}}
	};

	std::string expression = "1+2*4-3";

	struct sSymbols
	{
		std::string symbol = "";

		enum class Type :uint8_t
		{
			Unknwown,
			LiteralNumeric,
			Opeartor
		};

		Type type = Type::Unknwown;
		sOperator op;
	};

	std::deque<sSymbols> sHolding;
	std::deque<sSymbols>sOutput;

	for (const auto c : expression)
	{
		// Push literals straight to output, They are already in order
		if(c >= '0' && c <= '9')
		  sOutput.push_back({std::string(1, c ), sSymbols::Type::LiteralNumeric});
		else if(mapOps.find(c) != mapOps.end())
		{
			// Symbol is operator
			const auto& new_op = mapOps[c];

			while (!sHolding.empty())
			{
				// Ensure holding stack front is an operator (it might not be later...)
				if (sHolding.front().type == sSymbols::Type::Opeartor)
				{
					const auto& holding_stack_op = sHolding.front().op;

					if (holding_stack_op.presedence >= new_op.presedence)
					{
						sOutput.push_back(sHolding.front());
						sHolding.pop_front();
					}
					else
						break;
				}
			}
			// Push the new operator onto the holding stack
			sHolding.push_back({std::string(1,c),sSymbols::Type::Opeartor ,new_op});
		}
	}

	while (!sHolding.empty())
	{
		sOutput.push_back(sHolding.front());
		sHolding.pop_front();
	}

	std::cin.get();
}