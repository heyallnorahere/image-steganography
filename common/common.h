#pragma once
namespace common {
	int encode(const std::string& image_file_name, const std::string& text, int bits, const std::string& output_file_name);
	int decode(const std::string& image_file_name, int text_length, int bits, const std::string& output_file_name);
}