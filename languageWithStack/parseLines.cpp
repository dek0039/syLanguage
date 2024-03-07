#include "fwdec.hpp"

namespace sy
{
	std::string userFunctionName{};
	void parseLines(std::string file)
	{
		auto lines = getLines(file);

		for (auto& line : lines)
		{
			if (line.empty())
				continue;
			if (line.contains("//")) {
				if (line.find("//") == 0)
					continue;
				else
					line = line.substr(0, line.find("//"));
			}

			parseFunctions(line, userFunctionName);

			if (line.contains("functionend"))
			{
				userFunctionName.clear();

				continue;
			}

			if (line.contains(FUNCTION) && userFunctionName.empty()) {
				userFunctionName.clear();
				auto pos = line.find_first_of(FUNCTION);
				line = line.erase(pos, pos + 8);
				line = removeChar(line, DELIM);
				if ("end" != removeString(removeSpacing(line), "()"))
				{
					userFunction function;
					UserFunctions.insert({ removeString(removeSpacing(line), "()"),  function });
					userFunctionName = removeString(removeSpacing(line), "()");
				}

				continue;
			}

			if (line.contains(VAR))
				parseVariables(line, userFunctionName);

			// + = += - etc
			for (const auto& [OP, OPSTR] : OPERATOR_TO_STRING) {
				if (line.contains(OPSTR))
					getOperations(line, userFunctionName);
				break;
			}

			parseUserFunctions(line);
		}
	}
}
