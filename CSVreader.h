#pragma once
#include <vector>
#include <string>
#include <map>
#include <fstream>

class CSVreader
{
private:
	std::ifstream file;
	std::vector<std::string> txt;
	std::map<std::string, int> columns;
	int commas_amount;
	void read();
	void calculate(std::string&, std::string&, int, std::size_t);
public:
	CSVreader(std::string);
	~CSVreader();
	void print();
};
