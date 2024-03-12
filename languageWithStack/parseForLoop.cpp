#include "fwdec.hpp"

namespace sy
{
	void parseForLoop(std::string line, std::string functionName)
	{
		const auto delims = findLocation(line, DELIM);
		int last = 0;
		for (int i = 0; i < delims.size(); i++)
		{
			auto complete = line.substr(last, delims[i] + 1);
			complete = removeSpacing(complete);
			complete = removeChar(complete, DELIM);
			
			if (complete.contains(FOR))
			{
				auto findLeftBracket = complete.find("(");
				auto findRightBracket = complete.find(")");
				if (findLeftBracket == std::string::npos)
				{
					std::cerr << "Syntax error! Missing bracket: " << line << std::endl;
					last += delims[i];
					continue;
				}
				if (findRightBracket == std::string::npos)
				{
					std::cerr << "Syntax error! Missing bracket: " << line << std::endl;
					last += delims[i];
					continue;
				}

				auto innerVar = complete.substr(findLeftBracket + 1);
				innerVar.erase(innerVar.size() - 1);
				innerVar = removeString(innerVar, ")");
				innerVar = removeString(innerVar, "for(");
				const auto comma = findLocation(innerVar, COMMA);
				int lastComma = comma[0];

				auto firstValue = innerVar.substr(0, comma[0]);
				auto secondStatement = innerVar.substr(comma[0], comma[1]);
				auto lastIteration = innerVar.substr(comma[1], innerVar.length());
				
				auto firstValuePos = firstValue.find(OPERATOR_TO_STRING[OPERATORS::EQUALS]);
				auto firstValueVariableName = firstValue.substr(0, firstValuePos);
				auto firstValueValue = firstValue.substr(firstValuePos + 1);
				std::string secondStatementValue{};

				auto commaPos = comma[0];
				auto valueStatementName = innerVar.substr(commaPos + 1, comma[1] - commaPos - 1);
				valueStatementName = removeString(valueStatementName, firstValueVariableName);
				valueStatementName = removeChar(valueStatementName, COMMA);
		
				forLoopCTX ctx;

				if (innerVar.contains("<="))
				{
					ctx.OP = OPERATORS::SMALLEREQL;
					valueStatementName = removeString(valueStatementName, "<=");
				}
				else if (innerVar.contains(">="))
				{
					ctx.OP = OPERATORS::BIGGEREQL;
					valueStatementName = removeString(valueStatementName, ">=");
				}
				else if (innerVar.contains("<"))
				{
					ctx.OP = OPERATORS::SMALLER;
					valueStatementName = removeString(valueStatementName, "<");
				}
				else if (innerVar.contains(">"))
				{
					ctx.OP = OPERATORS::BIGGER;
					valueStatementName = removeString(valueStatementName, ">");
				}

				secondStatementValue = valueStatementName;

				if (innerVar.contains("+"))
				{
					ctx.inc = true;
				}
				else if (innerVar.contains("-"))
				{
					ctx.inc = false;
				}

				ctx.functionName = innerVar.substr(comma[2] + 1);
				ctx.value = std::atoi(firstValueValue.c_str());
				ctx.till = std::atoi(secondStatementValue.c_str());
		
				if (functionName.empty())
				{
					Stack.push_back([ctx]
						{
							if (ctx.inc)
							{
								if (ctx.OP == OPERATORS::SMALLER)
								{
									for (int i = ctx.value; i < ctx.till; i++)
									{
										if (UserFunctions.contains(ctx.functionName))
										{
										    UserFunctions[ctx.functionName]->execute();
										}
									}
								}
							}
						});
				}


				ForLoopContext[firstValueVariableName] = std::make_shared<forLoopCTX>(ctx);
			}

			last += delims[i];
		}
	}
}