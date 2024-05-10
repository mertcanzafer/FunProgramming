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
	typedef
	sSymbols::Type		sT;

	for (const auto c : expression)
	{
		// Push literals straight to output, They are already in order
		if(c >= '0' && c <= '9')
		  sOutput.push_back({std::string(1, c ), sT::LiteralNumeric});
		else if(mapOps.find(c) != mapOps.end())
		{
			// Symbol is operator
			const auto& new_op = mapOps[c];

			while (!sHolding.empty())
			{
				// Ensure holding stack front is an operator (it might not be later...)
				if (sHolding.front().type == sT::Opeartor)
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
			sHolding.push_back({std::string(1,c),sT::Opeartor ,new_op});
		}
		else
		{
			std::cout << "Bad symbol: " << std::string(1, c) << "\n";
			return 0;
		}
	}

	while (!sHolding.empty())
	{
		sOutput.push_back(sHolding.front());
		sHolding.pop_front();
	}

	std::cout << "Expression:= " << expression << "\n";
	std::cout << "RPN       := ";
	for (const auto& s : sOutput)
	{
		std::cout << s.symbol;
	}
	std::cout << "\n";

	// Solver
	std::deque<double> Solver;

	for (const auto& inst : sOutput)
	{
		switch (inst.type)
		{
		case sT::Unknwown:

		break;
		
		case sT::Opeartor:
		{
			std::vector<double> mem(inst.op.arguments);	
			for (uint8_t i = 0; i < inst.op.arguments; i++)
			{
				if (!Solver.empty())
				{
					mem[i] = Solver[0];
					Solver.pop_front();
				}
				else {
					std::cout << "!!!				ERROR!  BAD Expression\n";
				}
			}

			double result{ 0.0 };

			if (inst.op.arguments == 2)
			{
				switch (inst.symbol[0])
				{
				case '/':
					result = mem[1] / mem[0];
					break;
				case '*':
					result = mem[1] * mem[0];
					break;
				case '+':
					result = mem[1] + mem[0];
					break;
				case '-':
					result = mem[1] - mem[0];
					break;
				}
			}
			Solver.push_front(result);
		}
		break;

		case sT::LiteralNumeric:
			Solver.push_front(std::stod(inst.symbol));
			break;
		default:
			std::cout << "Incorrect type!!\n";
		}
	}

	std::cout << "Result	:=" << Solver[0] << "\n";

	std::cin.get();
}
