//#include "snek_api.h"
//#include "generic_stack.h"
#include "random_search.h"
//Outputs
struct stack* random_search(GameBoard* board) {
    int debug = 0;
    //srand(time(0));
    //struct stack* layer = create_stack();
    int highest_score = -1;
    struct stack *highest_steps = NULL;
    for (int trials = 0; trials < 50000; trials++) {//50000
        if (debug) printf("Trial %d\n", trials);
        int alive = 1;
        struct stack *steps = create_stack();
        if (debug) printf("stack created\n");
        struct GameBoard *clone = clone_board(board);
        if (debug) printf("board cloned\n");
        int delta_score = 0;//((clone->timeOut) - (clone->currFrame))
        for (int s = 0; s < 100 && alive; s++) {//100
            if (debug) printf("Step %d\n",s);
            struct step *new_step = (struct step *) malloc(sizeof(step));
            if (debug) printf("Created step\n");
            int axis = (rand() % 2) * 2 - 1;
            int direction = (rand() % 2) * 2 - 1;
            new_step->axis = axis;
            new_step->direction = direction;
            push(steps, new_step);
            if (debug) printf("Added step to stack\n");
            int result = contained_advance_frame(axis,direction,clone);
            delta_score += result;
            if (debug) printf("Frame advanced\n");
            if (result == 0) {
                //printf("%d",trials);
                alive = 0;
                if (s < 1 && trials!=-1) {
                    trials--;
                }
            }
        }
        //printf("final score: %d || ",clone->score);
        if (delta_score > highest_score) {
            if (debug) printf("New highest\n");
            highest_score = delta_score;
            if (debug) printf("Highest score replaced\n");
            if (highest_steps != NULL) {
                if (debug) printf("Not null\n");
                //Free highest steps
                if (highest_steps->size == 0) {
                    if (debug) printf("0\n");
                    free(highest_steps);
                } else {
                    if (debug) printf("Non 0\n");
                    int last_entry = 0;
                    struct stack_entry *top = highest_steps->top;
                    while (!last_entry) {
                        struct stack_entry *next = top->next;
                        free(top->value);
                        free(top);
                        if (next != NULL) {
                            top = next;
                        } else {
                            last_entry = 1;
                        }
                    }
                    free(highest_steps);
                }
                if (debug) printf("Freed highest steps\n");
            }
            //
            highest_steps = steps;
            if (debug) printf("Highest steps replaced\n");
        } else {
            if (debug) printf("Not highest\n");
            //Free steps
            if (steps->size == 0){
                free(steps);
            } else {
                int last_entry = 0;
                struct stack_entry* top = steps->top;
                while (!last_entry){
                    struct stack_entry* next = top->next;
                    free(top->value);
                    free(top);
                    if (next != NULL){
                        top = next;
                    } else {
                        last_entry = 1;
                    }
                }
                free(steps);
            }
            //
            if (debug) printf("Freed steps");
        }
        if (debug) printf("Freeing board...\n");
        delete_board(&clone);
        if (debug) printf("Freed board");
    }
    if (debug) printf("Highest Score: %d, currFrame: %d/%d", highest_score,board->currFrame,board->timeOut);
    return highest_steps;
}