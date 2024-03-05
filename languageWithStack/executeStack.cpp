#include "fwdec.hpp"

namespace sy
{
	void executeStack()
	{
		for (auto& stack : Stack)
		{
			if (stack != nullptr)
				stack();
		}
	}
}