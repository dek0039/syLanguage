#include "fwdec.hpp"

namespace sy {

    inline float extractValue(std::string complete)
    {
        if (!complete.contains("+") && !complete.contains("-") && !complete.contains("/") && !complete.contains("*"))
        {
            if (complete.contains("="))
            {
                auto pos = complete.find("=");
                auto str = complete.substr(pos + 1, complete.length());
                
                if (Variables.contains(str))
                {
                    return Variables[str]->asFloat();
                }
                else
                {
                    try {
                        return std::atof(str.c_str());
                    }
                    catch (...)
                    {
                        std::cerr << "Invalid syntax by: " << complete;
                    }
                }  
            }
        }

        return 0.0;
    }

    std::string extractName(std::string complete)
    {
        if (!complete.contains("+") && !complete.contains("-") && !complete.contains("/") && !complete.contains("*"))
        {
            if (complete.contains("="))
            {
                auto pos = complete.find("=");
                return complete.substr(0, pos);
            }
        }

        return "0";
    }

    void parseVariables(std::string line)
    {
       const auto delims = findLocation(line, DELIM);
       int last = 0;
       for (int i = 0; i < delims.size(); i++)
       {
           auto complete = line.substr(last, delims[i]);
           complete = removeSpacing(complete);
           complete = removeChar(complete, DELIM);
           complete = removeString(complete, VAR);

           float value = extractValue(complete);
           std::string name = extractName(complete);
           if (name != "0")
                Variables[name] = std::make_shared<Variable>(value);

    //       std::cout << name << ":" << value << std::endl;
           last += delims[i];
       }
	}
}
