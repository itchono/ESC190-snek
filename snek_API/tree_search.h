#include "snek_api.h"
#include "generic_stack.h"

typedef struct Node{
    GameBoard* board;
    struct stack* steps;
    int score;
}Node;

typedef struct step{
    int axis;
    int direction;
}step;

struct stack* diverge(struct Node* state);
struct stack* clone_step_stack(struct stack* steps);
struct stack* tree_search(GameBoard* board, int score);