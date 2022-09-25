#include <stdio.h>

void set_value_of_item(int*, int, int);

void print_hello();
void print_message(const char* message);
void print_goodbye();

void (*print_pointer) ();
void (*print_array_pointer[2])();
void (*print_message_pointer)(const char*);

struct my_variable
{
	int value;
	const char* name;
};

int main()
{
	int value_array[5] = { 1, 2, 3, 4, 5 };
	int* value_array_pointer = value_array;
	int** value_array_double_pointer = &value_array_pointer;

	for (; value_array_pointer <= &value_array[4]; value_array_pointer++)
	{
		printf("Value of array item is: %d", *value_array_pointer);
		printf(", Pointer of array item is: %p", value_array_pointer);
		printf(", Double pointer of array item is: %p\n", ++value_array_double_pointer);
	}

	printf("Sizeof array item is: %d\n", (int)sizeof(&value_array[0]));
	printf("Sizeof array is: %d\n", (int)sizeof(value_array));
	printf("Sizeof pointer array is: %d\n", (int)sizeof(value_array_pointer));
	printf("Sizeof double pointer array is: %d\n", (int)sizeof(value_array_double_pointer));

	set_value_of_item(value_array, 10, 2);
	printf("Edited value of item is: %d\n", value_array[2]);

	print_pointer = print_hello;
	print_pointer();

	print_array_pointer[0] = print_hello;
	print_array_pointer[1] = print_goodbye;

	for (int i = 0; i < 2; i++)
	{
		print_array_pointer[i]();
	}

	print_message_pointer = print_message;
	print_message_pointer("Hello world!");

	int x = 10;
	int* px = &x;
	printf("Pointer px is: %p\n", px);
	printf("Pointer ++px is: %p %d\n", ++px, *px);

	my_variable variable;
	variable.name = "Int";
	variable.value = 120;
	printf("%s = %d throw my_variable\n", variable.name, variable.value);

	my_variable* variable_pointer = &variable;
	printf("%s = %d throw my_variable*\n", variable_pointer->name, variable_pointer->value);
}

void set_value_of_item(int* array, int new_value, int index)
{
	array[index] = new_value;
}

void print_hello()
{
	printf("Hello!\n");
}

void print_message(const char* message)
{
	printf("%s\n", message);
}

void print_goodbye()
{
	printf("Goodbye!\n");
}
