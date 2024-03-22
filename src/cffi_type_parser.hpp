#ifndef __CFFI_TYPE_PARSER_HPP__
#define __CFFI_TYPE_PARSER_HPP__

#include <ffi.h>
#include <godot_cpp/classes/reg_ex.hpp>

using namespace godot;

namespace cffi {

class CFFITypeParser {
public:
	bool is_valid() const;
	void clear();
	const String& get_base_name() const;
	int get_pointer_level() const;
	String get_full_name() const;
	bool parse(const String& full_name);

private:
	String name;
	int pointer_level;

	bool set_name(const String& new_name);
};

}

#endif  // __CFFI_TYPE_PARSER_HPP__