#ifndef __CFFI_POINTER_HPP__
#define __CFFI_POINTER_HPP__

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

namespace cffi {

class CFFIType;

/**
 * FFI pointers wrapped in a RefCounted object.
 *
 * This class is merely a representation of raw pointers in a Godot object.
 * Users are responsible for knowing when a pointer is valid or not and manage them accordingly.
 * @warning Misuse of pointers may crash your application and/or the Godot editor.
 */
class CFFIPointer : public RefCounted {
	GDCLASS(CFFIPointer, RefCounted);
public:
	/**
	 * Necessary to define a Godot class.
	 * @warning Never use this constructor.
	 */
	CFFIPointer();
	/**
	 * Create a new pointer from an existing `address`, pointing to elements of type `element_type`.
	 */
	CFFIPointer(Ref<CFFIType> element_type, uint8_t *address);

	/**
	 * Get the raw pointer address, offset by `offset`.
	 *
	 * To get the base pointer, call this with `offset` as 0.
	 */
	uint8_t *address_offset_by(int offset) const;
	/**
	 * Returns a new pointer pointing to the base address offset by `offset` elements.
	 *
	 * Calling this with a 0 offset returns a copy of the base pointer.
	 * @note The element type is taken into consideration when offsetting pointers.
	 *       For example, a pointer to `uint32_t` offset by 1 element will offset the pointer by 4 bytes.
	 */
	Ref<CFFIPointer> offset_by(int offset) const;
	/**
	 * Get the value at `index`.
	 *
	 * @warning Users are responsible for knowing if a pointer is valid before dereferencing it.
	 *
	 * @see CFFIType::data_to_variant
	 */
	Variant get_value(int index = 0) const;
	/**
	 * Set the value at `index`.
	 *
	 * @warning Users are responsible for knowing if a pointer is valid before dereferencing it.
	 * Not all types support all kinds of values.
	 * For example, trying to set an element of struct type with a value of `true` will give an error.
	 *
	 * @return Whether the value was set correctly.
	 * @see CFFIType::variant_to_data
	 */
	bool set_value(const Variant& value, int index = 0) const;

	/**
	 * @return Element type.
	 */
	Ref<CFFIType> get_element_type() const;
	/**
	 * Get a new pointer with the same base address, but pointing to a different element type.
	 *
	 * Use this to cast between pointer types.
	 *
	 * @param element_type  Element type. Should be a string or a `CFFIType`.
	 */
	Ref<CFFIPointer> cast_elements(const Variant& element_type) const;

	/**
	 * Get a String from this pointer, using ASCII encoding.
	 *
	 * @warning Users are responsible for knowing if a pointer is valid before dereferencing it.
	 * This pointer should point to elements with 1 byte, such as `char` or `int8_t`.
	 * It's an error to read ASCII strings from elements with more than 1 byte.
	 *
	 * @param length  String length, in bytes.
	 *                Use -1 to read null-terminated ASCII strings.
	 */
	String get_string_from_ascii(int length = -1) const;
	/**
	 * Get a String from this pointer, using UTF-8 encoding.
	 *
	 * @warning Users are responsible for knowing if a pointer is valid before dereferencing it.
	 * This pointer should point to elements with 1 byte, such as `char` or `int8_t`.
	 * It's an error to read UTF-8 strings from elements with more than 1 byte.
	 *
	 * @param length  String length, in bytes.
	 *                Use -1 to read null-terminated UTF-8 strings.
	 */
	String get_string_from_utf8(int length = -1) const;
	/**
	 * Get a String from this pointer, using UTF-16 encoding.
	 *
	 * @warning Users are responsible for knowing if a pointer is valid before dereferencing it.
	 * This pointer should point to elements with 2 bytes, such as `char16_t` or `int16_t`.
	 * It's an error to read UTF-16 strings from elements with any size other than 2 bytes.
	 *
	 * @param length  String length, in 2-byte chars.
	 *                Use -1 to read null-terminated UTF-16 strings.
	 */
	String get_string_from_utf16(int length = -1) const;
	/**
	 * Get a String from this pointer, using UTF-32 encoding.
	 *
	 * @warning Users are responsible for knowing if a pointer is valid before dereferencing it.
	 * This pointer should point to elements with 4 bytes, such as `char32_t` or `int32_t`.
	 * It's an error to read UTF-32 strings from elements with any size other than 2 bytes.
	 *
	 * @param length  String length, in 2-byte chars.
	 *                Use -1 to read null-terminated UTF-32 strings.
	 */
	String get_string_from_utf32(int length = -1) const;
	/**
	 * Get a String from a pointer to `wchar_t`.
	 *
	 * @param length  String length, in `sizeof(wchar_t)` chars.
	 *                Use -1 to read null-terminated wide strings.
	 */
	String get_string_from_wchar(int length = -1) const;
	/**
	 * Get a copy of the memory pointed by this pointer as a `PackedByteArray`.
	 *
	 * @warning Users are responsible for knowing if a pointer is valid before dereferencing it.
	 *
	 * @param length  Buffer length, in number of elements.
	 *                For example, a buffer of length 2 taken from a pointer to `int32_t` would have 8 bytes.
	 */
	PackedByteArray get_buffer(int length) const;
	/**
	 * Get an Array with the values pointed by this pointer using `get_value`.
	 *
	 * @warning Users are responsible for knowing if a pointer is valid before dereferencing it.
	 *
	 * @param length  Array length, in number of elements.
	 */
	Array to_array(int length) const;

protected:
	static void _bind_methods();
	String _to_string() const;

	Ref<CFFIType> element_type;
	uint8_t *address;
};

}

#endif  // __CFFI_POINTER_HPP__