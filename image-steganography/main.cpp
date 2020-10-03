#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <common.h>
#define REQUIRED_NUMBER_ARGUMENTS 4
static void help_text() {
	std::cout << "usage: input image file (string),\n input text file (string),\n number of bits to write per byte (integer),\n output file name (string)" << std::endl;
}
static std::string read_file(const std::string& file_name, char return_char = '\n') {
	std::ifstream file(file_name);
	std::stringstream file_contents;
	std::string line;
	while (std::getline(file, line)) {
		file_contents << line << return_char;
	}
	file.close();
	return file_contents.str();
}
int main(int argc, const char** argv) {
	if (argc < REQUIRED_NUMBER_ARGUMENTS + 1) {
		help_text();
		return -1;
	}
	std::string input_image_file_name = argv[1];
	std::string input_text = read_file(argv[2], ' ');
	int bits = atoi(argv[3]);
	std::string output_file_name = argv[4];
	return common::encode(input_image_file_name, input_text, bits, output_file_name);
}