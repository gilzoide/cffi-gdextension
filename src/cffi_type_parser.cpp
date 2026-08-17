#include "cffi_type_parser.hpp"

#include <godot_cpp/classes/reg_ex.hpp>
#include <godot_cpp/classes/reg_ex_match.hpp>

namespace cffi {

bool CFFITypeParser::is_valid() const {
	return !name.is_empty();
}

void CFFITypeParser::clear() {
	name = "";
	array_levels.clear();
}

const String& CFFITypeParser::get_base_name() const {
	return name;
}

const LocalVector<int>& CFFITypeParser::get_array_levels() const {
	return array_levels;
}

String CFFITypeParser::get_full_name() const {
	String full_name = name;
	for (int level : array_levels) {
		if (level < 0) {
			full_name += "*";
		}
		else {
			full_name += "[";
			full_name += String::num_int64(level);
			full_name += "]";
		}
	}
	return full_name;
}

bool CFFITypeParser::parse(const String &full_name) {
	clear();

	Ref<RegEx> identifier_re = RegEx::create_from_string("^\\s*(?:const|volatile)?\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s*");
	Ref<RegExMatch> identifier_match = identifier_re->search(full_name);
	if (identifier_match.is_null()) {
		return false;
	}

	// Pointer -> (const|volatile)? \*
	// Array -> \[ \d* \]
	Ref<RegEx> pointer_or_array_re = RegEx::create_from_string("\\s*(?:const|volatile)?\\s*\\*\\s*|\\s*\\[\\s*(\\d*)\\s*\\]\\s*");
	Ref<RegExMatch> pointer_or_array_match;

	LocalVector<int> levels;
	for (int offset = identifier_match->get_end(); offset < full_name.length(); offset = pointer_or_array_match->get_end()) {
		pointer_or_array_match = pointer_or_array_re->search(full_name, offset);
		if (pointer_or_array_match.is_null()) {
			return false;
		}

		// If pointer or empty array, capture will be empty
		String array_dimension = pointer_or_array_match->get_string(1);
		if (array_dimension.is_empty()) {
			levels.push_back(-1);
		}
		else {
			levels.push_back(array_dimension.to_int());
		}
	}

	name = identifier_match->get_string(1);
	array_levels = std::move(levels);
	return true;
}

}