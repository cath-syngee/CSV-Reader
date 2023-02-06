#include "CSVreader.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <regex>

CSVreader::CSVreader(std::string csvfile) : commas_amount(0)
{
	file.open(csvfile);
	if (csvfile.substr(csvfile.rfind('.')) != ".csv")
	{
		throw std::runtime_error("it's not a .csv file");
	}
	
	std::string line;
	while (file >> line)
	{
		txt.push_back(line);
	}
}

CSVreader::~CSVreader()
{
	file.close();
}

void CSVreader::read()
{
	int count, pos;
	for (auto &str : txt)
	{
		if (str.substr(0, 1).find_first_of(",0123456789-") == std::string::npos)	//если str[0] не цифра или запятая - файл неправильный	
		{
			throw std::runtime_error("data in this file is incorrect");
		}

		count = std::count(str.begin(), str.end(), ',');
		if (commas_amount == 0) commas_amount = count;
		else
		{
			if (commas_amount != count)					//если в разных строках разное количество запятых
			{
				throw std::runtime_error("amount of commas in this file is incorrect");
			}
		} 	

		if (str[0] == ',')
		{
			if (!columns.empty()) throw std::runtime_error("data in this file is incorrect"); //если 2 строки с заголовком

			count = 0;
			std::string cell = str.substr(1);
			while ((pos = cell.find(',')) >= 0)
			{
				count++;
				columns.insert(std::make_pair(cell.substr(0, pos), count));
				cell = cell.substr(pos + 1);
				if (cell.find(',') == std::string::npos) 
				{
					columns.insert(std::make_pair(cell, count + 1));
				}
			}
			continue;
		}

		if(columns.empty()) throw std::runtime_error("no header with column names in this file");	//если нет строки с заголовком
		
		std::size_t found = str.find_first_not_of(",0123456789-.");
		while (found != std::string::npos)
		{
			if (str[found] == '=')
			{
				std::string cell = str.substr(found + 1);
				int pos = cell.find(',');
				cell = cell.substr(0, pos);
				int cell_len = cell.length();
				calculate(cell, str, cell_len, found);
			}
			else
			{
				throw std::runtime_error("data in file cells is incorrect");
			}
			found = str.find_first_not_of(",0123456789-.");
		}
	}
}

void CSVreader::print()
{
	read();
	for (auto str : txt)
	{
		std::cout << str << std::endl;
	}
}

void CSVreader::calculate(std::string& expression, std::string& str_tochange, int length, std::size_t position)
{
	static int recursion_stop = 0;
	if (recursion_stop >= 20) throw std::runtime_error("data dependency");
	recursion_stop++;
	double result;
	std::smatch match;
	std::regex regexpr("([a-zA-Z\\-]+)([0-9\\.]+)([\\+\\-\\*/])([a-zA-Z]+)([0-9\\.]+)");
	bool valid = regex_search(expression, match, regexpr); 

	int var1, var2;
	std::string column1, row1, operation, column2, row2;
	std::string cell1, cell2;
	if (!valid) 
	{
		std::regex reg1("([a-zA-Z\\-]+)([0-9\\.]+)([\\+\\-\\*/])([0-9\\.]+)"); //=A2*5
		std::regex reg2("([0-9\\-\\.]+)([\\+\\-\\*/])([a-zA-Z]+)([0-9\\.]+)"); //=5*A2
		std::regex reg3("([0-9\\-\\.]+)([\\+\\-\\*/])([0-9\\.]+)");			   //=5*6
		std::smatch mat1;
		std::smatch mat2;
		std::smatch mat3;
		bool v1 = regex_search(expression, mat1, reg1);
		bool v2 = regex_search(expression, mat2, reg2);
		bool v3 = regex_search(expression, mat3, reg3);
		if (!v1) 
		{
			if (!v2) 
			{
				if (!v3) 
				{
					throw std::runtime_error("invalid expression");
				}
				else
				{
					cell1 = mat3[1].str();
					operation = mat3[2].str();
					cell2 = mat3[3].str();
				}
			}
			else
			{
				cell1 = mat2[1].str();
				operation = mat2[2].str();
				column2 = mat2[3].str();
				row2 = mat2[4].str();
			}
		}
		else
		{
			column1 = mat1[1].str();
			row1 = mat1[2].str();
			operation = mat1[3].str();
			cell2 = mat1[4].str();
		}
	}
	else
	{
		column1 = match[1].str();
		row1 = match[2].str();
		operation = match[3].str();
		column2 = match[4].str();
		row2 = match[5].str();
	}

	int col1, col2;			//ищем порядковый номер нужных столбцов
	
	if (cell1.empty()) 
	{
		if (columns.find(column1) != columns.end())
		{
			col1 = columns.find(column1)->second;
		}
		else throw std::runtime_error("invalid expression: no such column in file");
	}
	else
	{
		var1 = std::stoi(cell1);
	}

	if (cell2.empty()) 
	{
		if (columns.find(column2) != columns.end())
		{
			col2 = columns.find(column2)->second;
		}
		else throw std::runtime_error("invalid expression: no such column in file");
	}
	else
	{
		var2 = std::stoi(cell2);
	}
	
	int pos1;
	int pos2 = 0;
	for (auto str : txt)
	{
		if ((!cell1.empty()) || (str.find(row1) == 0))
		{
			if (str.substr(0, str.find(',')) != row1) continue;		//проверка 2 или 22 до запятой
			for (auto st : txt)
			{
				if ((!cell2.empty()) || (st.find(row2) == 0))
				{
					if (st.substr(0, st.find(',')) != row2) continue;

					if (cell1.empty()) 
					{
						for (int i = 0; i < col1; i++)
						{
							pos1 = str.find(',', pos2);
							pos2 = str.find(',', pos1 + 1);
						}
						cell1 = str.substr(pos1 + 1, pos2 - pos1 - 1);
					}
					pos2 = 0;

					if (cell2.empty()) 
					{
						for (int i = 0; i < col2; i++)
						{
							pos1 = st.find(',', pos2);
							pos2 = st.find(',', pos1 + 1);
						}
						cell2 = st.substr(pos1 + 1, pos2 - pos1 - 1);
					}

					if (cell1.empty() || cell2.empty()) throw std::runtime_error("invalid expression: cell is empty");
					if (cell1[0] == '=') 
					{
						cell1 = cell1.substr(1);
						calculate(cell1, str, cell1.length(), str.find(cell1));
					}
					if (cell2[0] == '=') 
					{
						cell2 = cell2.substr(1);
						calculate(cell2, st, cell2.length(), st.find(cell2));
					}

					var1= std::stoi(cell1);
					var2= std::stoi(cell2);
					
					char op = operation[0];
					switch (op)
					{
					case '+':
						result = var1 + var2;
						break;
					case '-':
						result = var1 - var2;
						break;
					case '*':
						result = var1 * var2;
						break;
					case '/':
						if (var2 != 0)
						{ 
							result = double(var1) / var2;
						}
						else throw std::runtime_error("invalid expression: division by zero");
					}
				}
				else if ((st.find(row2) == std::string::npos) || (st.find(row2) != 0))
				{
					continue;
				}
				else throw std::runtime_error("invalid expression: no such line in file");
			}
		}
		else if ((str.find(row1) == std::string::npos) || (str.find(row1) != 0))
		{
			continue;
		}
		else throw std::runtime_error("invalid expression: no such line in file");
	}

	std::cout << "res=" << result;
	expression = std::to_string(result);
	expression = expression.substr(0, expression.find_last_not_of('0') + 2);
	str_tochange.replace(position, length + 1, expression);
}
