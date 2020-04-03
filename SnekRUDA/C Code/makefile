snek_AI: main.o snek_api.o generic_stack.o random_search.o controller.o
	gcc -o snek_AI main.o snek_api.o generic_stack.o random_search.o controller.o

main.o: main.c snek_api.h random_search.h
	gcc -O -c main.c

snek_api.o: snek_api.c snek_api.h
	gcc -O -c snek_api.c

generic_stack.o: generic_stack.c generic_stack.h
	gcc -O -c generic_stack.c

random_search.o: random_search.c random_search.h
	gcc -O -c random_search.c
    
controller.o: controller.c
	gcc -O -c controller.c
    
python:
	gcc -fPIC -shared -o ../controller.so controller.c random_search.c snek_api.c generic_stack.c
	
clean:
	rm *.o
