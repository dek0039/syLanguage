#include "fwdec.hpp"

namespace sy
{
	std::string removeSpacesBetweenPlusAndQuotes(const std::string& str) {
		std::string temp{};
		int i = 0;

	
		bool quoteStart = false;

		for (char c : str)
		{
			if (c == QUOTE)
			{
				quoteStart = !quoteStart;
			}

			if (!quoteStart && c != ' ' && c != '+' && c != QUOTE)
				temp += c;
		}

		return temp;
	}

	void parseFunctions(std::string line, std::string functionName)
	{
		bool isFunction = UserFunctions.contains(functionName);
	
		const auto delims = findLocation(line, DELIM);
		int last = 0;
		for (int i = 0; i < delims.size(); i++)
		{
			auto complete = line.substr(last, delims[i] + 1);
			auto completeWithSpaces = complete;
			completeWithSpaces = removeChar(completeWithSpaces, DELIM);

			complete = removeSpacing(complete);
			complete = removeChar(complete, DELIM);

			if (complete.contains(PRINT))
			{
				auto findLeftBracket = completeWithSpaces.find("(");
				auto findRightBracket = completeWithSpaces.find(")");
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

				auto innerVar = completeWithSpaces.substr(findLeftBracket + 1);
				innerVar.erase(innerVar.size() - 1);

				
				if (isFunction)
				{
					UserFunctions[functionName]->Stack.push_back([innerVar, functionName]
						{
							if (UserFunctions[functionName]->Variables.contains(innerVar))
							{
								std::cout << UserFunctions[functionName]->Variables[innerVar]->asFloat() << std::endl;
							}
							else if(Variables.contains(innerVar))
							{
								std::cout << Variables[innerVar]->asFloat() << std::endl;
							}
						});

					if (UserFunctions[functionName]->Variables.contains(innerVar))
					{
						last += delims[i];
						continue;
					}
					else if (Variables.contains(innerVar))
					{
						last += delims[i];
						continue;
					}
				}
				else
				{
					Stack.push_back([innerVar]
						{
							if (Variables.contains(innerVar))
							{
								std::cout << Variables[innerVar]->asFloat() << std::endl;
							}
						});

					if (Variables.contains(innerVar))
					{
						last += delims[i];
						continue;
					}
				}
			

				std::vector<std::string> stringsInsideQuote{};
				std::string concatenatedString{};
				if (innerVar.contains(QUOTE))
				{
					auto pluses = findLocation(innerVar, '+');
					if (pluses.size() == 0)
					{
						innerVar = removeChar(innerVar, QUOTE);
						if (isFunction)
						{
							UserFunctions[functionName]->Stack.push_back([innerVar]
								{
									int lastPos = 0;
									const auto newLines = findLocationString(innerVar, "-n");
									if (newLines.size())
									{
										for (auto& pos : newLines) {
											std::cout << removeString(innerVar.substr(lastPos, pos - lastPos), "-n") << std::endl;
											lastPos = pos;
										}
										std::cout << removeString(innerVar.substr(lastPos), "-n") << std::endl;
									}
									else
									{
										std::cout << innerVar;
									}
								});
						}
						else
						{
							Stack.push_back([innerVar]
								{
									int lastPos = 0;
									const auto newLines = findLocationString(innerVar, "-n");
									if (newLines.size())
									{
										for (auto& pos : newLines) {
											std::cout << removeString(innerVar.substr(lastPos, pos - lastPos), "-n") << std::endl;
											lastPos = pos;
										}
										std::cout << removeString(innerVar.substr(lastPos), "-n") << std::endl;
									}
									else
									{
										std::cout << innerVar;
									}
								});
						}
						
						last += delims[i];
						continue;
					}
					else
					{
					}			
				}


				if (isFunction)
				{
					if (!UserFunctions[functionName]->Variables.contains(innerVar))
					{
						last += delims[i];
						continue;
					}

					UserFunctions[functionName]->Stack.push_back([innerVar, line, functionName]
						{
							if (!UserFunctions[functionName]->Variables.contains(innerVar))
							{
								std::cerr << "Syntax error! Unidentified variable: |" << innerVar << "|" << std::endl << "Line: " << line << std::endl;
							}
							std::cout << UserFunctions[functionName]->Variables[innerVar]->asFloat() << std::endl;
						});
				}
				else
				{
					if (!Variables.contains(innerVar))
					{
						last += delims[i];
						continue;
					}

					Stack.push_back([innerVar, line]
						{
							if (!Variables.contains(innerVar))
							{
								std::cerr << "Syntax error! Unidentified variable: |" << innerVar << "|" << std::endl << "Line: " << line << std::endl;
							}
							std::cout << Variables[innerVar]->asFloat() << std::endl;
						});
				}
			}

			if (complete.contains(DUMP_VAR))
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

				if (isFunction)
				{
					UserFunctions[functionName]->Stack.push_back([&]
						{
							for (const auto& [n, c] : UserFunctions[functionName]->Variables)
								std::cout << n << "=" << c->m_val << std::endl;
						});
				}
				else
				{
					Stack.push_back([&]
						{
							for (const auto& [n, c] : sy::Variables)
								std::cout << n << "=" << c->m_val << std::endl;
						});
				}
				
			}

			last += delims[i];
		}
	}
}