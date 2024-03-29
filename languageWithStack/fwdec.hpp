#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <map>
#include <filesystem>
#include <stack>
#include <functional>

namespace sy {
    class Variable
    {
    public:
        Variable(float val) : m_val(val)
        {
        }

        int asInt()
        {
            return (int)m_val;
        }

        bool asBool()
        {
            return (bool)m_val;
        }

        float asFloat()
        {
            return (float)m_val;
        }

    public:
        float m_val;
    };

    inline std::map<std::string, std::shared_ptr<Variable>> Variables;
    inline std::vector<std::function<void()>> Stack;

    class userFunction
    {
    public:
       std::map<std::string, std::shared_ptr<Variable>> Variables;
       std::vector<std::function<void()>> Stack;
  
       void execute()
       {
           for (auto& stack : Stack)
           {
               if (stack != nullptr)
                   stack();
           }
       }

       void clear()
       {
           Stack.clear();
           Variables.clear();
       }
    };

    inline std::map<std::string, std::shared_ptr<userFunction>> UserFunctions;

    extern std::vector<std::string> getLines(std::string file);
    extern void parseVariables(std::string line, std::string functionName);
    extern void getOperations(std::string line, std::string functionName);
    extern void executeStack();
    extern void parseLines(std::string file);
    extern void parseFunctions(std::string line, std::string functionName);
    extern void parseUserFunctions(std::string line);
    extern void parseForLoop(std::string line, std::string functionName);
    extern void parseFunctionReturn(std::string line, std::string functionName);

    enum class OPERATORS
    {
        NONE,
        EQUALS,
        ADD,
        SUB,
        DIV,
        MULT,
        EQADD,
        EQSUB,
        EQDIV,
        EQMULT,
        BIGGER,
        SMALLER,
        BIGGEREQL,
        SMALLEREQL
    };

    struct forLoopCTX
    {
        int value;
        int till;
        bool inc;
        OPERATORS OP;
        std::string functionName;
    };
    inline std::map <std::string, std::shared_ptr<forLoopCTX>> ForLoopContext;

    static std::map<OPERATORS, std::string> OPERATOR_TO_STRING = {
    {OPERATORS::EQUALS, "="},
    {OPERATORS::ADD, "+"},
    {OPERATORS::SUB, "-"},
    {OPERATORS::DIV, "/"},
    {OPERATORS::MULT, "*"},
    {OPERATORS::EQADD, "=+"},
    {OPERATORS::EQSUB, "=-"},
    {OPERATORS::EQDIV, "=/"},
    {OPERATORS::EQMULT, "=*"},
    };

    inline std::string removeSpacing(std::string str)
    {
        std::string temp;

        for (const auto c : str)
            if (c != ' ' && c != '\n')
                temp += c;

        return temp;
    }

    inline std::string removeChar(std::string str, char ch)
    {
        std::string temp;

        for (const auto c : str)
            if (c != ch)
                temp += c;

        return temp;
    }

    inline std::string removeString(std::string str, const std::string& strToRemove)
    {
        auto pos = str.find(strToRemove);

        while (pos != std::string::npos)
        {
            str = str.erase(pos, strToRemove.length());
            pos = str.find(strToRemove);
        }

        return str;
    }

    inline std::vector<int> findLocation(std::string sample, char findIt)
    {
        std::vector<int> temp;
        for (int i = 0; i < sample.length(); i++)
        {
            if (sample[i] == findIt)
                temp.push_back(i);
        }

        return temp;
    }

    inline std::vector<size_t> findLocationString(const std::string& mainStr, const std::string& subStr) {
        std::vector<size_t> occurrences;
        size_t pos = mainStr.find(subStr);

        while (pos != std::string::npos) {
            occurrences.push_back(pos);
            pos = mainStr.find(subStr, pos + 1);
        }

        return occurrences;
    }

    inline bool isFloat(std::string myString) {
        std::istringstream iss(myString);
        float f;
        iss >> f; // noskipws considers leading whitespace invalid
        // Check the entire string was consumed and if either failbit or badbit is set
        return iss.eof() && !iss.fail();
    }

    inline bool isNumaric(std::string myString) {
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

#define VAR "var"
#define DELIM ';'
#define COMMA ','
#define FOR "for"
#define PRINT "print"
#define DUMP_VAR "dump"
#define EXIT "exit"
#define QUOTE '\"'
#define FUNCTION "function"
#define INPUT "input"
#define RETURN "return"
}