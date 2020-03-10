#include "snek_api.h"
#include "generic_stack.h"

typedef struct step{
    int axis;
    int direction;
}step;

struct stack* random_search(GameBoard* board);