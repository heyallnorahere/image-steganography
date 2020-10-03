#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <common.h>
#define REQUIRED_NUMBER_ARGUMENTS 4
static void help_text() {
	std::cout << "usage: input image file (string),\n length of text (integer),\n number of bits to read per byte (integer),\n output file name (string)" << std::endl;
}
int main(int argc, const char** argv) {
	if (argc < REQUIRED_NUMBER_ARGUMENTS + 1) {
		help_text();
		return -1;
	}
	std::string input_image_file_name = argv[1];
	int text_length = atoi(argv[2]);
	int bits = atoi(argv[3]);
	std::string output_file_name = argv[4];
	return common::decode(input_image_file_name, text_length, bits, output_file_name);
}