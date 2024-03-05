#include "fwdec.hpp"

namespace sy {
    std::vector<std::string> getLines(std::string file)
    {
        std::vector<std::string> ret;

        std::ifstream f(file);
        std::string line;
        if (f.is_open()) {
            while (getline(f, line)) {
                if (!line.empty())
                    ret.push_back(line);
                line.clear();
            }
            f.close();
        }

        return ret;
    }
}
