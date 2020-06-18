#include "Handler.h"

int main(int argc, char* argv[])
{
	try
	{
		Handler handler;
		handler.Run(argc, argv);
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
		return 1;
	}

	return 0;
}
