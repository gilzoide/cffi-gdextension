#ifndef __CFFI_SPAN_HPP__
#define __CFFI_SPAN_HPP__

#include <godot_cpp/classes/ref_counted.hpp>

#include "cffi_pointer.hpp"

using namespace godot;

namespace cffi {

class CFFIType;
class CFFIOwnedArray;

/**
 * Object that provides access to a finite number of contiguous elements in memory.
 *
 * Wraps a `CFFIPointer` and the number of elements.
 * Users are responsible for knowing when a pointer is valid or not and manage them accordingly.
 * @warning Misuse of pointers may crash your application and/or the Godot editor.
 */
class CFFISpan : public RefCounted {
	GDCLASS(CFFISpan, RefCounted);
public:
	/**
	 * Necessary to define a Godot class.
	 * @warning Never use this constructor.
	 */
	CFFISpan();
	/**
	 * Create a new span from an existing `address`, pointing to `length` elements of type `element_type`.
	 */
	CFFISpan(Ref<CFFIType> element_type, uint8_t *address, int64_t length);
	/**
	 * Create a new span from an existing `pointer`, pointing to `length` elements.
	 */
	CFFISpan(Ref<CFFIPointer> pointer, int64_t length);

	/**
	 * Get a pointer to the data.
	 */
	Ref<CFFIPointer> get_data() const;
	/**
	 * Get the type of the elements of the span.
	 */
	Ref<CFFIType> get_element_type() const;

	/**
	 * Get the number of elements in the span.
	 */
	int64_t get_length() const;
	/**
	 * Get the size of the span, in bytes.
	 */
	int64_t get_size_bytes() const;
	/**
	 * Returns `true` if the span has no elements.
	 */
	bool is_empty() const;

	/**
	 * Returns part of the span from the position `from` with `count` elements.
	 * If `count` is -1 (as by default), returns the rest of the span starting from the given position.
	 */
	Ref<CFFISpan> subspan(int from, int count = -1) const;
	/**
	 * Get a pointer to value at `index`.
	 * If the index is out of bounds, this method errors and returns `nullptr`.
	 */
	Ref<CFFIPointer> get_pointer(int index) const;

	/**
	 * Get the value at `index`.
	 *
	 * If the index is out of bounds, this method errors and returns `Variant()`.
	 *
	 * @see CFFIPointer::get_value
	 */
	Variant get_value(int index) const;
	/**
	 * Set the value at `index`.
	 *
	 * If the index is out of bounds, this method errors and returns `false`.
	 *
	 * Not all types support all kinds of values.
	 * For example, trying to set an element of struct type with a value of `true` will give an error.
	 *
	 * @return Whether the value was set correctly.
	 * @see CFFIPointer::set_value
	 */
	bool set_value(int index, const Variant& value) const;

	/**
	 * Duplicate data into a new `CFFIOwnedArray`.
	 */
	Ref<CFFIOwnedArray> duplicate() const;

	/**
	 * Get a String from this span, using ASCII encoding.
	 *
	 * This span should point to elements with 1 byte, such as `char` or `int8_t`.
	 * It's an error to read ASCII strings from elements with more than 1 byte.
	 */
	String get_string_from_ascii() const;
	/**
	 * Get a String from this span, using UTF-8 encoding.
	 *
	 * This span should point to elements with 1 byte, such as `char` or `int8_t`.
	 * It's an error to read UTF-8 strings from elements with more than 1 byte.
	 */
	String get_string_from_utf8() const;
	/**
	 * Get a String from this span, using UTF-16 encoding.
	 *
	 * This span should point to elements with 2 bytes, such as `char16_t` or `int16_t`.
	 * It's an error to read UTF-16 strings from elements with any size other than 2 bytes.
	 */
	String get_string_from_utf16() const;
	/**
	 * Get a String from this span, using UTF-32 encoding.
	 *
	 * This span should point to elements with 4 bytes, such as `char32_t` or `int32_t`.
	 * It's an error to read UTF-32 strings from elements with any size other than 2 bytes.
	 */
	String get_string_from_utf32() const;
	/**
	 * Get a String from a pointer to `wchar_t`.
	 *
	 * This span should point to elements with `sizeof(wchar_t)` bytes, such as `wchar_t`.
	 * It's an error to read wide strings from elements with any size other than `sizeof(wchar_t)` bytes.
	 */
	String get_string_from_wchar() const;
	/**
	 * Get a copy of the memory pointed by this pointer as a `PackedByteArray`.
	 */
	PackedByteArray to_byte_array() const;
	/**
	 * Get a copy of the memory pointed by this pointer as a `PackedInt32Array`.
	 */
	PackedInt32Array to_int32_array() const;
	/**
	 * Get a copy of the memory pointed by this pointer as a `PackedInt64Array`.
	 */
	PackedInt64Array to_int64_array() const;
	/**
	 * Get a copy of the memory pointed by this pointer as a `PackedFloat32Array`.
	 */
	PackedFloat32Array to_float32_array() const;
	/**
	 * Get a copy of the memory pointed by this pointer as a `PackedFloat64Array`.
	 */
	PackedFloat64Array to_float64_array() const;
	/**
	 * Get a copy of the memory pointed by this pointer as a `PackedVector2Array`.
	 */
	PackedVector2Array to_vector2_array() const;
	/**
	 * Get a copy of the memory pointed by this pointer as a `PackedVector3Array`.
	 */
	PackedVector3Array to_vector3_array() const;
	/**
	 * Get a copy of the memory pointed by this pointer as a `PackedVector4Array`.
	 */
	PackedVector4Array to_vector4_array() const;
	/**
	 * Get a copy of the memory pointed by this pointer as a `PackedColorArray`.
	 */
	PackedColorArray to_color_array() const;
	/**
	 * Get an Array with the values pointed by this pointer using `get_value`.
	 */
	Array to_array() const;

	/**
	 * Create a new span from `pointer` and a `count`.
	 * If `pointer` is `nullptr` or `count` is negative, prints an error and returns `nullptr`.
	 */
	static Ref<CFFISpan> from(Ref<CFFIPointer> pointer, int64_t count);

protected:
	static void _bind_methods();
	virtual String _to_string() const;

	Ref<CFFIType> element_type;
	uint8_t *address;
	int64_t length = 0;
};

}

#endif  // __CFFI_SPAN_HPP__