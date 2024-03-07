#include "fwdec.hpp"

namespace sy
{
	void parseUserFunctions(std::string line)
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
			complete = removeString(complete, FUNCTION);

			if (UserFunctions.contains(removeString(complete, "()")))
			{
				Stack.push_back([complete]
					{
						auto& userFunc = UserFunctions[removeString(complete, "()")];
						for (const auto& func : userFunc.Stack)
						{
							if (func)
								func();
						}
					});			
			}

			last += delims[i];
		}
	}
}