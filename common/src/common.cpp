#include "pch.h"
#include "../common.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>
namespace common {
	enum class image_format {
		png,
		jpeg,
		other,
	};
	image_format get_format(const std::string& file_name) {
		size_t period_pos = file_name.find_last_of('.');
		if (period_pos == std::string::npos) return image_format::other;
		std::string extension = file_name.substr(period_pos + 1);
		if (extension == "png") return image_format::png;
		if (extension == "jpg" || extension == "jpeg") return image_format::jpeg;
		return image_format::other;
	}
	// the encoding algorithm
	int encode(const std::string& image_file_name, const std::string& text, int bits, const std::string& output_file_name) {
		int width, height, channels;
		unsigned char* data = stbi_load(image_file_name.c_str(), &width, &height, &channels, NULL);
		if (!data) {
			std::cout << "could not load image: " << image_file_name << std::endl;
			return -2;
		}
		std::cout << "loaded image: " << image_file_name << std::endl;
		int bytes_per_character = 8 / bits;
		if (text.length() * bytes_per_character > width * height * channels) {
			std::cout << "text is too long" << std::endl;
			return -3;
		}
		std::vector<unsigned char> bytes;
		for (int i = 0; i < width * height * channels; i++) {
			bytes.push_back(data[i]);
		}
		stbi_image_free(data);
		int bits_allowed = 255 << bits;
		int shift = 0, current_character = 0;
		std::cout << "encoding text..." << std::endl;
		for (int i = 0; i < bytes_per_character * text.length(); i++) {
			unsigned char& byte = bytes[i];
			byte = byte & bits_allowed;
			char c = text[current_character];
			byte = byte | ((c >> shift) & (~bits_allowed));
			shift += bits;
			if (!(shift % 8)) {
				shift = 0;
				current_character++;
			}
		}
		std::cout << "finished encoding! writing..." << std::endl;
		int code = 0;
		switch (get_format(image_file_name)) {
		case image_format::png:
			code = stbi_write_png(output_file_name.c_str(), width, height, channels, bytes.data(), width * channels);
			break;
		case image_format::jpeg:
			code = stbi_write_jpg(output_file_name.c_str(), width, height, channels, bytes.data(), 100);
			break;
		default:
			return -4;
			break;
		}
		if (!code) return -5;
		std::cout << "finished writing!" << std::endl;
		return 0;
	}
	int decode(const std::string& image_file_name, int text_length, int bits, const std::string& output_file_name) {
		int width, height, channels;
		unsigned char* data = stbi_load(image_file_name.c_str(), &width, &height, &channels, NULL);
		if (!data) {
			std::cout << "could not load image: " << image_file_name << std::endl;
			return -2;
		}
		std::cout << "loaded image: " << image_file_name << std::endl;
		int bytes_per_character = 8 / bits;
		if (text_length * bytes_per_character > width * height * channels) {
			std::cout << "text is too long" << std::endl;
			return -3;
		}
		std::vector<unsigned char> bytes;
		for (int i = 0; i < width * height * channels; i++) {
			bytes.push_back(data[i]);
		}
		stbi_image_free(data);
		std::stringstream text;
		int bits_allowed = ~(255 << bits);
		int shift = 0;
		char character_to_commit = (char)0;
		std::cout << "decoding text..." << std::endl;
		for (int i = 0; i < bytes_per_character * text_length; i++) {
			unsigned char byte = bytes[i];
			byte = byte & bits_allowed;
			character_to_commit = character_to_commit | byte << shift;
			shift += bits;
			if (!(shift % 8)) {
				shift = 0;
				text << character_to_commit;
				character_to_commit = (char)0;
			}
		}
		std::string text_str = text.str();
		std::ofstream output_file(output_file_name);
		output_file << text_str << std::endl;
		output_file.close();
		return 0;
	}
}
