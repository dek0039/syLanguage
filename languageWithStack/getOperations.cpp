#include "fwdec.hpp"

namespace sy
{
	inline float doSingleVar(std::string complete)
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


		float val = NAN;
		int which = 0;
		if (Variables.contains(firstVar))
		{
			val = Variables[firstVar]->m_val;
		}
		if (Variables.contains(secondVar))
		{
			which = 1;
			val = Variables[secondVar]->m_val;
		}

		if (isnan(val))
		{
			std::cerr << "Syntax error! " << firstVar << "->" << secondVar;
			exit(0);
		}

		Stack.push_back([OP, firstVar, secondVar, which] {
			
			});

		return 0.0;
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

		if (isnan(val))
		{
			std::cerr << "Syntax error! Unidentified variable(s)" << firstVar << ":" << secondVar;
			exit(0);
		}

		Stack.push_back([OP, firstVar, secondVar, which, containsBoth, val] {
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
		});

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
				doDoubleVar(complete);

			last += delims[i];
	    }
	}
}