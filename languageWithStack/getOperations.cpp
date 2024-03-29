#include "fwdec.hpp"

namespace sy
{
    inline float doDoubleVar(std::string complete, std::map<std::string, std::shared_ptr<Variable>>& VariablesMap, bool isFunc)
    {
		std::string firstVar;
		std::string secondVar;
		OPERATORS OP;
		if (complete.contains("+="))
		{
			firstVar = complete.substr(0, complete.find("+="));
			secondVar = complete.substr(firstVar.length() + 2, complete.length());
			OP = OPERATORS::ADD;
		}
		else if (complete.contains("-="))
		{
			firstVar = complete.substr(0, complete.find("-="));
			secondVar = complete.substr(firstVar.length() + 2, complete.length());
			OP = OPERATORS::SUB;
		}
		else if (complete.contains("*="))
		{
			firstVar = complete.substr(0, complete.find("*="));
			secondVar = complete.substr(firstVar.length() + 2, complete.length());
			OP = OPERATORS::MULT;
		}
		else if (complete.contains("/="))
		{   
			firstVar = complete.substr(0, complete.find("/="));
			secondVar = complete.substr(firstVar.length() + 2, complete.length());
			OP = OPERATORS::DIV;
		}
		else if (complete.contains("="))
		{
			firstVar = complete.substr(0, complete.find("="));
			secondVar = complete.substr(firstVar.length() + 1, complete.length());
			OP = OPERATORS::EQUALS;
		}

		float val = NAN;
		int which = 0;
		bool containsBoth = true;

		if (VariablesMap.contains(firstVar))
		{
			val = VariablesMap[firstVar]->m_val;
		}
		else
		{
			containsBoth = false;
		}
		if (VariablesMap.contains(secondVar))
		{
			which = 1;
			val = VariablesMap[secondVar]->m_val;
		}
		else
		{
			containsBoth = false;
		}

		if (isNumaric(secondVar))
		{
			val = std::atoi(secondVar.c_str());
		}

		if (isFloat(secondVar))
		{
			val = std::atof(secondVar.c_str());
		}

		if (isnan(val))
		{
			std::cerr << "Syntax error! Unidentified variable(s)" << firstVar << ":" << secondVar;
			exit(0);
		}

		if (isFunc)
		{
			if (which == 0)
			{
				if (!VariablesMap[firstVar])
				{
					VariablesMap = Variables;
				}
			}

			if (which == 1)
			{
				if (!VariablesMap[secondVar])
				{
					VariablesMap = Variables;
				}
			}
		}

		if (OP == OPERATORS::ADD && containsBoth) {
			VariablesMap[firstVar]->m_val += VariablesMap[secondVar]->m_val;
		}
		else if (OP == OPERATORS::SUB && containsBoth) {
			VariablesMap[firstVar]->m_val -= VariablesMap[secondVar]->m_val;
		}
		else if (OP == OPERATORS::MULT && containsBoth) {
			VariablesMap[firstVar]->m_val *= VariablesMap[secondVar]->m_val;
		}
		else if (OP == OPERATORS::DIV && containsBoth) {
			VariablesMap[firstVar]->m_val /= VariablesMap[secondVar]->m_val;
		}
		else if (OP == OPERATORS::ADD) {
			if (which)
				VariablesMap[secondVar]->m_val += val;
			else
				VariablesMap[firstVar]->m_val += val;
		}
		else if (OP == OPERATORS::SUB) {
			if (which)
				VariablesMap[secondVar]->m_val -= val;
			else
				VariablesMap[firstVar]->m_val -= val;
		}
		else if (OP == OPERATORS::MULT) {
			if (which)
				VariablesMap[secondVar]->m_val *= val;
			else
				VariablesMap[firstVar]->m_val *= val;
		}
		else if (OP == OPERATORS::DIV) {
			if (which)
				VariablesMap[secondVar]->m_val /= val;
			else
				VariablesMap[firstVar]->m_val /= val;
		}
		else if (OP == OPERATORS::EQUALS) {
				VariablesMap[firstVar]->m_val = VariablesMap[secondVar]->m_val;
		}

        return 0.0;
    }

	void getOperations(std::string line, std::string functionName)
	{
	    const auto delims = findLocation(line, DELIM);
	    int last = 0;
	    for (int i = 0; i < delims.size(); i++)
	    {
	    	auto complete = line.substr(last, delims[i] + 1);
	    	complete = removeSpacing(complete);
	    	complete = removeChar(complete, DELIM);
			if (!complete.contains(VAR))
			{
				if (functionName.empty())
				{
					Stack.push_back([complete]
						{
							doDoubleVar(complete, Variables, false);
						});
				}
				else
				{
					UserFunctions[functionName]->Stack.push_back([complete, functionName]
						{
						    doDoubleVar(complete, UserFunctions[functionName]->Variables, true);				
						});
				}
			}
				
			last += delims[i];
	    }
	}
}