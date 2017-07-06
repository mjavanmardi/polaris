#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>

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
		cout << "Success!" << endl;
	}
	cout << "Press any key to exit" << endl;
	_getch();

	return 0;
}
