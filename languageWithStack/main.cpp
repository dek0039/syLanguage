#include "fwdec.hpp"
	
int main(int argc, char* argv[])
{
    sy::parseLines(argv[1]);
    sy::executeStack();

    return 0;
}   