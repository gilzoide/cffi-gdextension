#include <string.h>

int global_int;

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

int sum_ints(const int *ints, int size) {
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sum += ints[i];
	}
	return sum;
}

ExampleStruct get_example_struct() {
	return (ExampleStruct) {
		.a = 1,
		.b = 2,
	};
}

ExampleStruct* get_global_example_struct() {
	static ExampleStruct g;
	return &g;
}

void get_global_example_struct_ptr(ExampleStruct** ptr) {
	*ptr = get_global_example_struct();
}

int example_struct_get_a(ExampleStruct s) {
	return s.a;
}

int example_struct_pointer_get_a(const ExampleStruct *s) {
	return s->a;
}

const char *get_message() {
	return "Hello world!";
}

void fill_message(char *msg, int buffer_size) {
	strncpy(msg, "Hello world!", buffer_size);
}

int str_length(const char *msg, int max_length) {
	if (!msg) {
		return 0;
	}

	int i = 0;
	while (*msg && i < max_length) {
		i++;
		msg++;
	}
	return i;
}
