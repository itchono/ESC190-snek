#include "snek_api.h"
#include "generic_stack.h"

typedef struct step{
    int axis;
    int direction;
}step;

//struct stack* random_search(GameBoard* board);
struct stack* random_search_cant_die(GameBoard* board);
void delete_step_stack(struct stack* steps);


extern int dead_stack;
