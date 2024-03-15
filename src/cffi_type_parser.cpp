#include "cffi_type_parser.hpp"

#include <godot_cpp/classes/reg_ex_match.hpp>

namespace cffi {

bool FFITypeParser::is_valid() const {
	return !name.is_empty();
}

void FFITypeParser::clear() {
	name = "";
	pointer_level = 0;
}

String FFITypeParser::get_base_name() const {
	return name;
}

int FFITypeParser::get_pointer_level() const {
	return pointer_level;
}

String FFITypeParser::get_full_name() const {
	return name + String::chr('*').repeat(pointer_level);
}

bool FFITypeParser::parse(const String &full_name) {
	clear();

	int start_index = 0;
	for (int64_t i = 0; i < full_name.length(); i++) {
		switch (full_name[i]) {
			case ' ':
			case '\t':
			case '\n':
			case '\r':
				if (start_index < i) {
					if (!set_name(full_name.substr(start_index, i - start_index))) {
						return false;
					}
				}
				start_index = i + 1;
				break;

			case '*':
				if (start_index < i) {
					if (!set_name(full_name.substr(start_index, i - start_index))) {
						return false;
					}
				}
				if (name.is_empty()) {
					return false;
				}
				pointer_level++;
				start_index = i + 1;
				break;
		}
	}
	if (start_index < full_name.length()) {
		if (!set_name(full_name.substr(start_index))) {
			return false;
		}
	}
	return is_valid();
}

bool FFITypeParser::set_name(const String& new_name) {
	if (new_name == "const" || new_name == "volatile" || new_name == "register") {
		return true;
	}
	else if (name.is_empty()) {
		name = new_name;
		return true;
	}
	else {
		return false;
	}
}

}