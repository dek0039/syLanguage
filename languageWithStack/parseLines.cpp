#include "fwdec.hpp"

namespace sy
{
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
				
			// var
			if (line.contains(VAR))
				parseVariables(line);

			// + = += - etc
			for (const auto& [OP, OPSTR] : OPERATOR_TO_STRING) {
				if (line.contains(OPSTR))
					getOperations(line);
				break;
			}

			parseFunctions(line);
		}
	}
}
