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
	string get_parameter(const string & parameter_name) const;
	int get_word_size() const{ return atoi(get_parameter("word_size").c_str());}
	string get_dictionary_file() const{ return get_parameter("dictionary_file");}
	bool parameter_exists(const string & parameter_name) const;
	void display_parameters() const;
private:
	void read_parameter_name();
	void read_parameter_value();
	map<string, string> parameters_map;
	ifstream my_opened_file;
	string my_parameter_name;
	string my_parameter_value;
};

bool Parameters::parameter_exists(const string & parameter_name) const
{
	auto it = parameters_map.find(parameter_name);
	return it != parameters_map.end();
}

string Parameters::get_parameter(const string & parameter_name) const
{
	auto it = parameters_map.find(parameter_name);
	return it->second;
}

void Parameters::display_parameters() const
{
	for(auto it = parameters_map.begin() ; it != parameters_map.end() ; it++)
	{
		cout << it->first << " : " << it->second << endl;
	}
}

void Parameters::read_parameter_name()
{
	cout << "Reading parameter name : ";
	my_opened_file >> my_parameter_name;
			
	cout << my_parameter_name << endl;
}

void Parameters::read_parameter_value()
{
	//cout << "Reading parameter value" << endl;
	my_opened_file >> my_parameter_value;
	parameters_map[my_parameter_name] = my_parameter_value;
	//cout << my_parameter_value << endl;
}

void Parameters::read_parameters_from_txt_file(const string & filepath)
{
	cout << "Opening file " << filepath << endl;
	my_opened_file.open(filepath.c_str());
	while (my_opened_file)
	{
		if (my_opened_file.eof())
			cout << "Fin de fichier avant lecture parameter_name" << endl;
		read_parameter_name();
		if (parameter_exists(my_parameter_name))
			if (my_opened_file.eof())
				break;
		if (my_opened_file.eof())
			throw "Missing parameter value when reading input file";
		read_parameter_value();
	}
	cout << "Closing file " << filepath << endl;
	my_opened_file.close();
}


#endif /* PARAMETERREADER_H_ */
