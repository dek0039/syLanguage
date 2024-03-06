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

	void parseFunctions(std::string line)
	{
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

				std::vector<std::string> stringsInsideQuote{};
				std::string concatenatedString{};
				if (innerVar.contains(QUOTE))
				{
					auto pluses = findLocation(innerVar, '+');
					if (pluses.size() == 0)
					{
						innerVar = removeChar(innerVar, QUOTE);
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
						last += delims[i];
						continue;
					}
					else
					{
							/*
							* std::cout << "Found plus count: " << pluses.size() << std::endl;

							std::string totalString{};
							int lastPlusLocation = 0;
							int currentPlusIter{};
							for (const auto& plusLocation : pluses)
							{
								std::cout << "Last Plus Location: " << lastPlusLocation << " current location: " << plusLocation << " first location: " << pluses[0] << std::endl;
				
								{
									auto leftToPlus = innerVar.substr(lastPlusLocation, plusLocation - lastPlusLocation);
									auto rightToPlus = innerVar.substr(plusLocation, innerVar.size());


									totalString += removeChar(removeChar(leftToPlus, QUOTE), '+');
								//	std::cout << "Left: " << removeChar(removeChar(leftToPlus, QUOTE), '+') << std::endl;

									if (findLocation(rightToPlus, QUOTE).size() == 0)
									{
										if (Variables.contains(removeSpacing(rightToPlus)))
										{
											totalString += Variables[removeSpacing(rightToPlus)]->asFloat();
										}
									}

									if (findLocation(rightToPlus, QUOTE).size() == 2)
									{		
										totalString += removeChar(removeChar(rightToPlus, QUOTE), '+');
										//std::cout << "Right: " << removeChar(removeChar(rightToPlus, QUOTE), '+') << std::endl;
									}
								
								}


								lastPlusLocation = pluses[currentPlusIter];
								currentPlusIter++;
							}

							std::cout << totalString << std::endl;
							*/
						/*int lastPlusLocation = 0;
						for (int plusLocation = 0; plusLocation < pluses.size(); plusLocation++)
						{
							std::cout << innerVar << std::endl;
							auto quotes = findLocation(innerVar, QUOTE);
							std::cout << "Quote count: " << quotes.size() << std::endl;
							for (const auto& quoteLocation : quotes)
							{
								auto leftToPlus = innerVar.substr(quoteLocation, pluses[plusLocation]);
								auto rightToPlus = innerVar.substr(pluses[plusLocation], innerVar.size());
								std::cout << "Right to plus: " << rightToPlus << std::endl;
								std::cout << "Left to plus: " << leftToPlus << std::endl;
								std::cout << "Quote location: " << quoteLocation << std::endl;
								
								if (quotes.size() % 2 == 0) {
									for (int i = 0; i < quotes.size(); i += 2) {
										auto stringInsideQuote = leftToPlus.substr(quotes[i] + 1, quotes[i + 1] - quotes[i] - 1);
										std::cout << stringInsideQuote << std::endl;
									}
								}
								else
								{
									std::cerr << "Syntax error! Missing quote: " << line << std::endl;
									last += delims[i];
									continue;
								}

								lastPlusLocation = plusLocation;
							}
						}
						*/
					}			
				}

			

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

				Stack.push_back([&]
				{
						for (const auto& [n, c] : sy::Variables)
							std::cout << n << "=" << c->m_val << std::endl;
				});
			}

			last += delims[i];
		}
	}
}