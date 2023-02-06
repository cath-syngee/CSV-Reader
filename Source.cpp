#include "CSVreader.h"
#include <iostream>

int main(int argc, char* argv[])
{
	try
	{
		if (argc == 2)
		{
			CSVreader file(argv[1]);
			file.print();
		}
		else
		{
			std::cout << "amount of files is incorrect";
			return 1;
		}
	}
	catch (std::runtime_error e)
	{
		std::cout << "Runtime error: " << e.what() << std::endl;
	}
	return 0;
}
