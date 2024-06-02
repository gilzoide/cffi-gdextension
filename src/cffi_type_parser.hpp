#ifndef __CFFI_TYPE_PARSER_HPP__
#define __CFFI_TYPE_PARSER_HPP__

#include <ffi.h>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

namespace cffi {

/**
 * Type full name parser.
 *
 * Counts pointer levels (e.g.: "int **" pointer level == 2).
 * Skips CV qualifiers "const", "volatile" and "register".
 * Does not support arrays (e.g.: "int[]").
 */
class CFFITypeParser {
public:
	/**
	 * @return Whether a valid type name was parsed by this parser.
	 */
	bool is_valid() const;
	/**
	 * Clear the parser state.
	 */
	void clear();
	/**
	 * Get the parsed type's base name.
	 */
	const String& get_base_name() const;
	/**
	 * @return The parsed type's pointer level.
	 *         If the type is not a pointer, returns 0.
	 */
	int get_pointer_level() const;
	/**
	 * Get the parsed type's full name, including pointer levels.
	 */
	String get_full_name() const;
	/**
	 * Parse a type from a full name representation.
	 *
	 * @return Whether the parsing succeeded and `full_name` represents a valid FFI type.
	 */
	bool parse(const String& full_name);

private:
	String name;
	int pointer_level;

	bool set_name(const String& new_name);
};

}

#endif  // __CFFI_TYPE_PARSER_HPP__