#include <stdio.h>
#include <stdlib.h>
#include "generic_stack.h"

struct stack* create_stack(){
    struct stack* pointer = (struct stack*)malloc(sizeof(struct stack));
    pointer->size = 0;
    pointer->top = NULL;
	return (pointer);
}

void delete_stack(struct stack* s){
    if (s->size == 0){
        free(s);
    } else {
        int last_entry = 0;
        struct stack_entry* top = s->top;
        while (!last_entry){
            struct stack_entry* next = top->next;
            free(top);
            if (next != NULL){
                top = next;
            } else {
                last_entry = 1;
            }
        }
        free(s);
    }
}

void* pop(struct stack* s){
    if ((s == NULL)||((*s).size == 0)) {
        //printf("ree");
        return NULL;
    } else {
        struct stack_entry* top = s->top;
        void* value = top->value;
        struct stack_entry *next = (top->next);
        free(top);
        s->top = next;
        s->size--;
        return value;
    }
}

int push(struct stack* s, void* e){
    struct stack_entry* top = (struct stack_entry*) malloc(sizeof(struct stack_entry));
    if (top == NULL){
        return -1;
    } else {
        top->next = (*s).top;
        top->value = e;
        (*s).top = top;
        (*s).size++;
        //printf("%d",(*s).size);
        return 1;
    }
}
