#include <string.h>

typedef struct {
	int a;
	int b;
} ExampleStruct;

int get_answer() {
    return 42;
}

int double_int(int i) {
	return i + i;
}

int double_float(float f) {
	return f + f;
}

ExampleStruct get_example_struct() {
	return (ExampleStruct) {
		.a = 1,
		.b = 2,
	};
}

const char *get_message() {
	return "Hello world!";
}

void fill_message(char *msg, int buffer_size) {
	strncpy(msg, "Hello world!", buffer_size);
}
