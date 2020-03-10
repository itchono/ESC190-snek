snek_AI: main.o snek_api.o generic_stack.o tree_search.o random_search.o
	gcc -o snek_AI main.o snek_api.o generic_stack.o tree_search.o random_search.o

main.o: main.c snek_api.h tree_search.h random_search.h
	gcc -O -c main.c

snek_api.o: snek_api.c snek_api.h
	gcc -O -c snek_api.c

generic_stack.o: generic_stack.c generic_stack.h
	gcc -O -c generic_stack.c

tree_search.o: tree_search.c snek_api.h generic_stack.h tree_search.h
	gcc -O -c tree_search.c

random_search.o: random_search.c random_search.h
	gcc -O -c random_search.c

libsnek_py.so: snek_api.c snek_api.h
	gcc -fPIC -shared -o ./py/libsnek_py.so snek_api.c
    
python:
	gcc -fPIC -shared -o ../src/libsnek_py.so snek_api.c
	
clean:
	rm *.o
