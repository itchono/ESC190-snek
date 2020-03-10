//#include "snek_api.h"
//#include "generic_stack.h"
#include "tree_search.h"
int debug = 1;
//Outputs
struct stack* tree_search(GameBoard* board, int score){
    struct stack* layer = create_stack();
    struct Node* start = (Node*)malloc(sizeof(Node));
    start->board = board;
    start->steps = create_stack();
    start->score = score;
    push(layer,start);
    for (int depth = 0; depth < 5; depth++){
        if(1){
            printf("Depth: %d\n",depth);
        }
        struct stack* new_layer = create_stack();
        while (layer->size > 0){
            if(debug){printf("Working through nodes\n");}
            struct stack* possible_moves = diverge(pop(layer));
            if(debug){printf("Generated possible moves\n");}
            while (possible_moves->size > 0){
                if(debug){("Working through possible moves\n");}
                struct Node* move = pop(possible_moves);
                if (move != NULL){
                    push(new_layer,move);
                    if(debug){printf("Move added to new layer\n");}
                }
            }
            if(debug){printf("Added this node's possibilities to new layer\n");}
            delete_stack(possible_moves);
            //free(possible_moves);
            if(debug){printf("Deleted possible moves\n");}
        }
        if(debug){printf("Total number of possible board states reachable in %d turns: %d\n", depth+1, new_layer->size);}
        if (new_layer->size == 0){
            if(debug){printf("No possibilities\n");}
            break;
        } else {
            delete_stack(layer);
            layer = new_layer;
        }
    }
    if(debug){printf("Exited\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");}

    int highest_score = -1;
    struct Node* highest_node = start;
    while (layer->size > 0){
        struct Node* temp = pop(layer);
        if (temp->score > highest_score){
            highest_node = temp;
            highest_score = temp->score;
        }
    }
    struct stack* best_steps = highest_node->steps;
    delete_stack(layer);
    free(start);
    return best_steps;
}

struct stack* diverge(struct Node* state){
    struct stack* moves = create_stack();
    for (int axis = -1; axis <= 1; axis+=2){
        for (int direction = -1; direction <= 1; direction+=2) {
            if(debug){printf("Testing axis %d and direction %d\n", axis, direction);}
            if(debug){printf("Hits self: %d\n", hits_self(axis, direction, state->board));}
            if(debug){printf("Hits edge: %d\n", hits_edge(axis, direction, state->board));}
            if(debug){printf("Times out: %d\n", (state->board->moogleFlag == 1 && state->board->currFrame > state->board->timeOut));}
            if (hits_self(axis, direction, state->board) || hits_edge(axis, direction, state->board) || (state->board->moogleFlag == 1 && state->board->currFrame > state->board->timeOut)) {
                if(debug){printf("Move failed\n");}
            } else {
                if(debug){printf("Move will not fail\n");}
                GameBoard *clone = clone_board(state->board);
                if(debug){printf("Board cloned\n");}
                struct stack *clone_steps = clone_step_stack(state->steps);
                if(debug){printf("Steps cloned\n");}
                int delta_score = contained_advance_frame(axis, direction, clone);
                if(debug){printf("Delta score: %d\n", delta_score);}
                struct step *new_step = (step *) malloc(sizeof(step));
                new_step->axis = axis;
                new_step->direction = direction;
                if(debug){printf("Created a new step with axis and direction\n");}
                //struct step* new_step = {axis,direction};
                push(clone_steps, new_step);
                if(debug){printf("Added new step to the cloned steps\n");}
                struct Node *new_node = (Node *) malloc(sizeof(Node));
                new_node->board = clone;
                new_node->steps = clone_steps;
                new_node->score = state->score + delta_score;
                if(debug){printf("Created a new node representing this state\n");}
                //struct Node* new_node = {clone,clone_steps,state->score+delta_score};
                push(moves, new_node);
                if(debug){printf("Pushed new node as a move\n");}
            }
            if(debug){printf("Starting next potential move\n\n");}
        }
    }
    if(debug){printf("Analysed all options, returning list of moves\n");}
    return moves;
}

struct stack* clone_step_stack(struct stack* steps){
    struct stack* temp = create_stack();
    while (steps->size > 0){
        push(temp,pop(steps));
    }
    struct stack* clone = create_stack();
    while (temp->size > 0){
        struct step* element = pop(temp);
        push(steps,element);
        push(clone,element);
    }
    free(temp);
    return clone;
}