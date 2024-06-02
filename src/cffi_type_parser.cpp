#include "cffi_type_parser.hpp"

namespace cffi {

bool CFFITypeParser::is_valid() const {
	return !name.is_empty();
}

void CFFITypeParser::clear() {
	name = "";
	pointer_level = 0;
}

const String& CFFITypeParser::get_base_name() const {
	return name;
}

int CFFITypeParser::get_pointer_level() const {
	return pointer_level;
}

String CFFITypeParser::get_full_name() const {
	return name + String::chr('*').repeat(pointer_level);
}

bool CFFITypeParser::parse(const String &full_name) {
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

bool CFFITypeParser::set_name(const String& new_name) {
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