#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/pointer.h"
#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "D:/EnhancerScenario/EnhancerScenario/option_parameters.h"

using std::cout;
using std::endl;
using std::string;

bool print_error_msg(rapidjson::Document& document, std::string json_file)
{
	// Get the parse error and offset
	rapidjson::ParseErrorCode parse_error = document.GetParseError();
	size_t parse_offset = document.GetErrorOffset();

	cout << "ERROR: ";

	if (parse_error == rapidjson::kParseErrorDocumentEmpty)   // Empty document
	{
		cout << "JSON file \'" << json_file << "\'" << " is empty" << endl;
		return false;
	}

	switch (parse_error)
	{
	case (rapidjson::kParseErrorDocumentRootNotSingular):
		cout << "JSON roots error";
		break;
	case (rapidjson::kParseErrorValueInvalid):
		cout << "Invalid value";
		break;
	case (rapidjson::kParseErrorObjectMissName):
		cout << "Object member is missing a name";
		break;
	case (rapidjson::kParseErrorObjectMissColon):
		cout << "Object member is missing a colon after name";
		break;
	case (rapidjson::kParseErrorObjectMissCommaOrCurlyBracket):
		cout << "Object member is missing a comma or \'}\'";
		break;
	case (rapidjson::kParseErrorArrayMissCommaOrSquareBracket):
		cout << "Array element is missing a comma or \']\'";
		break;
	case (rapidjson::kParseErrorStringUnicodeEscapeInvalidHex):
		cout << "String has incorrect hex digit after escape";
		break;
	case (rapidjson::kParseErrorStringUnicodeSurrogateInvalid):
		cout << "String has an invalid surrogate pair";
		break;
	case (rapidjson::kParseErrorStringEscapeInvalid):
		cout << "String has an invalid escape character";
		break;
	case (rapidjson::kParseErrorStringMissQuotationMark):
		cout << "String is missing a closing quotation mark";
		break;
	case (rapidjson::kParseErrorStringInvalidEncoding):
		cout << "String has invalid encoding";
		break;
	case (rapidjson::kParseErrorNumberTooBig):
		cout << "Number is too big to be stored as double";
		break;
	case (rapidjson::kParseErrorNumberMissFraction):
		cout << "Number is missing the fraction part";
		break;
	case (rapidjson::kParseErrorNumberMissExponent):
		cout << "Number is missing the exponent part";
		break;
	case (rapidjson::kParseErrorTermination):
		cout << "Rapid JSON parsing terminated";
		break;
	case (rapidjson::kParseErrorUnspecificSyntaxError):
		cout << "Unspecific syntax error";
		break;
	default:
		cout << "Unknown";
	}
	cout << " starting at " << parse_offset << endl;

	return false;
}

int find_line_begin(std::ifstream& json_string, int start_pos)
{
	int pos = start_pos;
	const int move_size = 5;
	char buffer[move_size];

	// Look for the beginning of the line that includes the given position
	while (true)
	{
		// Move back from prev_pos
		pos -= move_size;

		// Check that position is a positive value
		if (pos > 0)
		{
			json_string.seekg(pos);

			// Read bytes
			json_string.read(buffer, move_size);

			// Look for a newline byte, which terminates previous line
			int eol_pos;
			for (eol_pos = sizeof(buffer) - 1; eol_pos >= 0; --eol_pos)
			{
				if (buffer[eol_pos] == '\n')
				{
					break;
				}
			}

			// If found newline or got to beginning of file - done looking
			if (eol_pos >= 0 || pos < move_size)
			{
				pos += eol_pos + 1;
				break;
			}
		}
		else
		{
			pos = 0;
			break;
		}
	}
	return pos;
}

bool parse_json_file(rapidjson::Document& document, std::string json_file)
{
	bool ret_val = true;

	// Check file name exists
	if (json_file.length() < 1)
	{
		cout << "Warning: option file was not specified" << endl;
		return true;
	}

	// Open JSON file
	std::ifstream ifs( json_file );
	if (!ifs.good())
	{
		cout << "ERROR: unable to open JSON file \'" << json_file << "\'" << endl;
		return false;
	}

	// Convert std::ifstream to RapidJSON input stream
	rapidjson::IStreamWrapper isw(ifs);

	// Parse and check for errors
	if (document.ParseStream(isw).HasParseError())
	{
		ret_val = print_error_msg(document, json_file);

		// Find size of file
		std::streamoff json_len;
		ifs.clear();
		ifs.seekg(0, ifs.end);
		json_len = ifs.tellg();

		// check if file has content
		if (json_len > 0)
		{
			// print text around error location
			int print_len = 40;
			std::string json_string;
			json_string.resize(print_len);

			// check if file length is less than amount to print
			if (json_len < print_len)
			{
				ifs.seekg(0, ifs.beg);
			}
			else
			{
				size_t half_len = (size_t)(0.5*(float)print_len);
				int start_index, line_index;
				size_t parse_offset = document.GetErrorOffset();

				// determine start index for printing
				if (parse_offset < half_len)
				{
					line_index = 0;
				}
				else if (parse_offset > (json_len - half_len))
				{
					start_index = (int)json_len - print_len;
					ifs.seekg(start_index);
					line_index = find_line_begin(ifs, start_index);
				}
				else
				{
					start_index = parse_offset - half_len;
					ifs.seekg(start_index);
					line_index = find_line_begin(ifs, start_index);
				}
				ifs.seekg(line_index);
			}
			// print
			for (int cntr = 0; cntr < 3; cntr++)
			{
				std::getline(ifs, json_string);
				cout << json_string << endl;
			}
		}
	}

	return ret_val;
}

std::vector<std::string> split_section(const char *str, char c = '/')
{
	std::vector<std::string> result;

	do
	{
		const char* begin = str;

		while (*str != c && *str)
		{
			str++;
		}

		result.push_back(std::string(begin, str));
	} while (0 != *str++);

	return result;
}

template <class T>

void get_parameter(rapidjson::Document& document, const std::string& section, const std::string& key, T& parameter)
{
	rapidjson::Value* value;

	// add "/" in front of key for Pointer
	std::string str_key = "/" + key;
	const char* char_key = str_key.c_str();

	// if section is not defined then use key
	if (section.compare("") == 0)
	{
		// check if key value is not found
		value = rapidjson::Pointer(char_key).Get(document);
		if (!value)
		{
			cout << "Unable to locate key \'" << char_key << "\'" << endl;
			return;
		}
	}
	else
	{
		std::vector<string> section_tokens;
		section_tokens = split_section(section.c_str(), '/');

		// add "/" to each token for Pointer
		for (auto& element : section_tokens)
		{
			element = "/" + element;
		}

		// check if first section is not found
		cout << "section token 0 is " << section_tokens[0] << endl;
		value = rapidjson::Pointer(section_tokens[0].c_str()).Get(document);
		if (!value)
		{
			cout << "Unable to locate section \'" << section_tokens[0] << "\'";
			if (section_tokens.size() > 0)
			{
				cout << " from \'" << section << "\'" << endl;
			}
			return;
		}
		
		// loop for each token element
		for (unsigned i = 1; i < section_tokens.size(); ++i)
		{
			cout << section_tokens[i] << endl;

			// check if next section is not found
			value = rapidjson::Pointer(section_tokens[i].c_str()).Get(*value);
			if (!value)
			{
				cout << "Unable to locate sub section \'" << section_tokens[i] << "\' from \'" << section << "\'" << endl;
				return;
			}
		}

		// if key is defined
		if (key.compare("") != 0)
		{
			// check if key value is not found
			value = rapidjson::Pointer(char_key).Get(*value);
			if (!value)
			{
				cout << "Unable to locate key \'" << char_key << "\' from \'" << section << "\'" << endl;
				return;
			}
		}
	}

	// get parameter
	get_parameter(*value, parameter);
}

void get_parameter(rapidjson::Value& value, std::string& parameter)
{
	if (!value.IsString())
	{
		cout << "Value is not set as string value. (" << value.GetString() << ")" << endl;
		return;
	}
	else
	{
		parameter = value.GetString();
	}
}

void get_parameter(rapidjson::Value& value, int& parameter)
{
	if (!value.IsInt())
	{
		cout << "Value is not set as integer value. (" << value.GetString() << ")" << endl;
		return;
	}
	else
	{
		parameter = value.GetInt();
	}
}

void get_parameter(rapidjson::Value& value, double& parameter)
{
	if (!value.IsDouble())
	{
		cout << "Value is not set as double value. (" << value.GetString() << ")" << endl;
		return;
	}
	else
	{
		parameter = value.GetDouble();
	}
}

void get_parameter(rapidjson::Value& value, float& parameter)
{
	if (!value.IsFloat())
	{
		cout << "Value is not set as float value. (" << value.GetString() << ")" << endl;
		return;
	}
	else
	{
		parameter = value.GetFloat();
	}
}

void get_parameter(rapidjson::Value& value, bool& parameter)
{
	if (!value.IsBool())
	{
		cout << "Value is not set as bool value. (" << value.GetString() << ")" << endl;
		return;
	}
	else
	{
		parameter = value.GetBool();
	}
}

int main(int argc, char** argv)
{
	// Read JSON file name
	string json_filename = "simple_file1.json";

	if (argc >= 2)
	{
		json_filename = argv[1];
	}

	if (json_filename.length() < 1)
	{
		cout << "Warning: JSON file not specified" << endl;
		exit(EXIT_FAILURE);
	}

	// Parse a JSON string into DOM.
	rapidjson::Document document;

	if (parse_json_file(document, json_filename))
	{
		// Set string parameter
		get_parameter(document, "hello", "", _hello);
		cout << "hello " << _hello << endl;

		get_parameter(document, "bye", "name1", _name1);
		cout << "bye " << _name1 << endl;

		get_parameter(document, "state/Illinois/city/Chicago/school", "UoC", _UoC);
		cout << "state/Illinois/city/Chicago/school/UoC is " << _UoC << endl;

		// Set int parameter
		get_parameter(document, "", "i", _i);
		cout << "i is " << _i << endl;

		get_parameter(document, "bye", "num2", _num2);
		cout << "bye/num2 is " << _num2 << endl;

		get_parameter(document, "state/Illinois/city/Chicago/area_code", "city", _city);
		cout << "state/Illinois/city/Chicago/area_code/city is " << _city << endl;

		// Set double parameter
		get_parameter(document, "pi", "", _pi);
		cout << "pi is " << _pi << endl;
		_pi = 0.0;
		get_parameter(document, "", "pi", _pi);
		cout << "pi is " << _pi << endl;

		get_parameter(document, "grades", "John", _John);
		cout << "grades for John is " << _John << endl;

		// Set float parameter
		get_parameter(document, "const", "", _const);
		cout << "const is " << _const << endl;

		get_parameter(document, "finished/homework", "number2", _number2);
		cout << "finished/homework/number2 is " << _number2 << endl;

		get_parameter(document, "finished/homework/number3", "partA", _partA);
		cout << "finished/homework/number3/partA is " << _partA << endl;

		// Set bool parameter
		get_parameter(document, "t", "", _t);
		cout << "t is " << _t << endl;

		get_parameter(document, "finished", "assignment", _assignment);
		cout << "finished/assignment is " << _assignment << endl;

		cout << "Success!" << endl;
	}
	cout << "Press any key to exit" << endl;
	_getch();

	return 0;
}
