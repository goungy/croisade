/*
 * ParameterReader.h
 *
 *  Created on: 25 sept. 2012
 *      Author: jerome
 */

#ifndef PARAMETERREADER_H_
#define PARAMETERREADER_H_

#include <string>
#include <map>
#include <fstream>
#include <iostream>
using namespace std;

class Parameters
{
public:
	void read_parameters_from_txt_file(const string & filepath);
	void display_parameters() const;
private:
	void read_parameter_name();
	void read_parameter_value();
	map<string, string> parameters_map;
	ifstream my_opened_file;
	string my_parameter_name;
	string my_parameter_value;
};

void Parameters::display_parameters() const
{
	for(auto it = parameters_map.begin() ; it != parameters_map.end() ; it++)
	{
		cout << it->first << " : " << it->second << endl;
	}
}

void Parameters::read_parameter_name()
{
	cout << "Reading parameter name" << endl;
	my_opened_file >> my_parameter_name;
	cout << my_parameter_name << endl;
}

void Parameters::read_parameter_value()
{
	cout << "Reading parameter value" << endl;
	my_opened_file >> my_parameter_value;
	parameters_map[my_parameter_name] = my_parameter_value;
	cout << my_parameter_value << endl;
}

void Parameters::read_parameters_from_txt_file(const string & filepath)
{
	cout << "Opening file " << filepath << endl;
	my_opened_file.open(filepath.c_str());
	while (!my_opened_file.eof())
	{
		read_parameter_name();
		if (my_opened_file.eof())
			throw "Missing parameter value in input file";
		read_parameter_value();
	}
	cout << "Closing file " << filepath << endl;
	my_opened_file.close();
}


#endif /* PARAMETERREADER_H_ */
