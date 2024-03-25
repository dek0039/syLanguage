#include "fwdec.hpp"

namespace sy
{
	void parseFunctionReturn(std::string line, std::string functionName)
	{
		const auto delims = findLocation(line, DELIM);
		int last = 0;
		for (int i = 0; i < delims.size(); i++)
		{
			auto complete = line.substr(last, delims[i] + 1);
			complete = removeSpacing(complete);
			complete = removeChar(complete, DELIM);
			auto firstPos = complete.find_first_of(RETURN);
			complete = complete.erase(firstPos, std::string(RETURN).size());

			UserFunctions[functionName]->Stack.push_back([complete, functionName]
				{
					if (UserFunctions[functionName]->Variables.contains(complete))
					{
						Variables[functionName] = UserFunctions[functionName]->Variables[complete];
					}
					else if (isNumaric(complete) || isFloat(complete))
					{
						try {
							Variables[functionName] =  std::make_shared<Variable>(std::atof(complete.c_str()));
						}
						catch (...)
						{
							std::cerr << "Invalid syntax by: " << complete << std::endl;
						}
					}
					else
					{
						std::cerr << "Error while returning from function: " << functionName << " undefined variable or value!" << std::endl;
					}
				});

			last += delims[i];
		}
	}
}