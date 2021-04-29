CC = gcc
CFLAGS = -Wall --std=c99 -g
OBJECTS = main.o generic_vector.o string.o tree.o
TESTOBJ = string.o unit_test.o test_def.o
ALL: string_driver unit_test

string_driver: $(OBJECTS)
        $(CC) $(CFLAGS) -o string_driver $(OBJECTS)
main.o: main.c
        $(CC) $(CFLAGS) -c main.c -o main.o
generic_vector.o: generic_vector.c
        $(CC) $(CFLAGS) -c generic_vector.c -o generic_vector.o
string.o: my_string.c
        $(CC) $(CFLAGS) -c my_string.c -o string.o
tree.o: tree.c
        $(CC) $(CFLAGS) -c tree.c -o tree.o
clean:
        rm string_driver $(OBJECTS)
