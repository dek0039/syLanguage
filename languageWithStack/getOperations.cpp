#include "fwdec.hpp"

namespace sy
{
	bool isFloat(std::string myString) {
		std::istringstream iss(myString);
		float f;
		iss >> f; // noskipws considers leading whitespace invalid
		// Check the entire string was consumed and if either failbit or badbit is set
		return iss.eof() && !iss.fail();
	}

	bool isNumaric(std::string myString) {
		bool numaric = true;

		for (const char c : myString)
		{
			if (!isdigit(c))
			{
				numaric = false;
			}
		}

		return numaric;
	}

    inline float doDoubleVar(std::string complete)
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

		if (Variables.contains(firstVar))
		{
			val = Variables[firstVar]->m_val;
		}
		else
		{
			containsBoth = false;
		}
		if (Variables.contains(secondVar))
		{
			which = 1;
			val = Variables[secondVar]->m_val;
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

		if (OP == OPERATORS::ADD && containsBoth) {
			Variables[firstVar]->m_val += Variables[secondVar]->m_val;
		}
		else if (OP == OPERATORS::SUB && containsBoth) {
			Variables[firstVar]->m_val -= Variables[secondVar]->m_val;
		}
		else if (OP == OPERATORS::MULT && containsBoth) {
			Variables[firstVar]->m_val *= Variables[secondVar]->m_val;
		}
		else if (OP == OPERATORS::DIV && containsBoth) {
			Variables[firstVar]->m_val /= Variables[secondVar]->m_val;
		}
		else if (OP == OPERATORS::ADD) {
			if (which)
				Variables[secondVar]->m_val += val;
			else
				Variables[firstVar]->m_val += val;
		}
		else if (OP == OPERATORS::SUB) {
			if (which)
				Variables[secondVar]->m_val -= val;
			else
				Variables[firstVar]->m_val -= val;
		}
		else if (OP == OPERATORS::MULT) {
			if (which)
				Variables[secondVar]->m_val *= val;
			else
				Variables[firstVar]->m_val *= val;
		}
		else if (OP == OPERATORS::DIV) {
			if (which)
				Variables[secondVar]->m_val /= val;
			else
				Variables[firstVar]->m_val /= val;
		}
		else if (OP == OPERATORS::EQUALS) {
				Variables[firstVar]->m_val = Variables[secondVar]->m_val;
		}

        return 0.0;
    }

	void getOperations(std::string line)
	{
	    const auto delims = findLocation(line, DELIM);
	    int last = 0;
	    for (int i = 0; i < delims.size(); i++)
	    {
	    	auto complete = line.substr(last, delims[i] + 1);
	    	complete = removeSpacing(complete);
	    	complete = removeChar(complete, DELIM);
			if (!complete.contains(VAR))
				Stack.push_back([complete]
				{
						doDoubleVar(complete);
				});
		
			last += delims[i];
	    }
	}
}