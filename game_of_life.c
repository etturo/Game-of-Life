/*GAME OF LIFE rules:
-if a cell is alive and have a 2 or 3 neighbour cells alive, it stays alive
-if a cell is dead it becomes alive if have 3 neighbours cells alive
*/

#include <stdio.h>
#include <unistd.h>

#define GRID_COLS 50
#define GRID_ROWS 50
#define GRID_CELLS (GRID_COLS*GRID_ROWS)
#define ALIVE '*'
#define DEAD ' '

/* Translate the specified x,y grid point into the index int the linear array. 
 * This function implements wrapping, so both negative and positive inddexes coordinates that are out of the grid will wrap around. */
int cell_to_index(int x, int y){
    if(x < 0){
        x = (-x) % GRID_COLS;
        x = GRID_COLS - x;
    }
    if(y < 0){
        y = (-y) % GRID_ROWS;
        y = GRID_ROWS - y;
    }
    if(x >= GRID_COLS) x = x % GRID_COLS;
    if(y >= GRID_ROWS) y = y % GRID_ROWS;

    return y*GRID_COLS+x;
}

/* The function sets the specified cell at x,y to the specified state. */
void set_cell(char *grid, int x, int y, char state){
    grid[cell_to_index(x,y)] = state;
}

/* The function return the state of the grid at x,y. */
char get_cell(char *grid, int x, int y){
    return grid[cell_to_index(x,y)];
}

/* Show the grid on the screen, clearing the terminal using the required VT100 escape sequence. 
 * Added: Counting at the end of the table the Number of Simulations. */
void print_grid(char * grid, unsigned int count){
    printf("\x1b[2J\x1b[H\x1b[3J"); //clear screen.
    for(int y = 0; y < GRID_ROWS; y++){
        for(int x = 0; x < GRID_COLS; x++){
            printf(" %c", get_cell(grid,x,y));
        }
        printf("\n");
    }
    printf("\nNumber of Simulations: %u\n", count);
}

/* Set all the grid cells to the specified state. */
void set_grid(char *grid, char state){
    for(int y = 0; y < GRID_ROWS; y++){
        for(int x = 0; x < GRID_COLS; x++){
            set_cell(grid,x,y,state);
        }
    }
}

/* Return the living cells around a specific cell in the grid. */
int count_living_neighbors(char *grid, int x, int y){
    return 
        (get_cell(grid, x-1, y) == ALIVE) +         //the WEST neighbor
        (get_cell(grid, x-1, y-1) == ALIVE) +       //the SUD-WEST neighbor
        (get_cell(grid, x, y-1) == ALIVE) +         //the SUD neighbor
        (get_cell(grid, x+1, y-1) == ALIVE) +       //the SUD-EAST neighbor
        (get_cell(grid, x+1, y) == ALIVE) +         //the EAST neighbor
        (get_cell(grid, x+1, y+1) == ALIVE) +       //the NORD-EAST neighbor
        (get_cell(grid, x, y+1) == ALIVE) +         //the NORD neighbor
        (get_cell(grid, x-1, y+1) == ALIVE);        //the NORD-WEST neighbor
        
}

/* Compute the new state of Game of Life according to its rules. */
void compute_new_state(char *old, char *new){
    int n_alive;
    int new_state;
    for(int y = 0; y < GRID_ROWS; y++){
        for(int x = 0; x < GRID_COLS; x++){
            n_alive = count_living_neighbors(old, x, y);
            new_state = DEAD;
            if(get_cell(old, x, y) == ALIVE){
                if(n_alive == 2 || n_alive == 3){
                    new_state = ALIVE;
                }
            } else {
                if(n_alive == 3){
                    new_state = ALIVE;
                }
            }
            set_cell(new,x,y,new_state);
        }
    }
}

int main(void){
    printf("\x1b[2J\x1b[H\x1b[3J"); //clear screen.
    //adding the select the pattern feature
    printf("Select the Pattern between:\n->1\n->2\n>");
    int answer;
    //adding a choice to select the velocity of simulation
    scanf("%d", &answer);
    unsigned int velocity = 100000;
    int vel_choice;
    printf("\x1b[2J\x1b[H\x1b[3J"); //clear screen.
    printf("Select the velocity of the simulation:\n1->Slow\n2->Normal\n3->Fast\n4->Very Fast\n>");
    scanf("%u", &vel_choice);
    printf("\x1b[2J\x1b[H\x1b[3J"); //clear screen.
    unsigned int count = 0;
    switch(vel_choice){
        case 1:
            velocity = 1000000;
        break;
        case 2:
            velocity = 500000;
        break;
        case 3:
            velocity = 100000;
        break;
        case 4:
            velocity = 10000;
        break;

    }
    char old_grid[GRID_CELLS];
    char new_grid[GRID_CELLS];
    set_grid(old_grid, DEAD);
    //SET OF PATTERNS
    switch(answer){
        case 1:
            //FIRST PATTERN
            //
            set_cell(old_grid, 25,25, ALIVE);
            set_cell(old_grid, 24,25, ALIVE);
            set_cell(old_grid, 25,26, ALIVE);
            set_cell(old_grid, 26,26, ALIVE);
            set_cell(old_grid, 26,24, ALIVE);
            //
            set_cell(old_grid, 11,12, ALIVE);
            set_cell(old_grid, 10,11, ALIVE);
            set_cell(old_grid, 9,11, ALIVE);
            set_cell(old_grid, 11,11, ALIVE);
            set_cell(old_grid, 11,10, ALIVE);
            set_cell(old_grid, 9,10, ALIVE);
            set_cell(old_grid, 9,9, ALIVE);
            //
            set_cell(old_grid, 40,43, ALIVE);
            set_cell(old_grid, 41,43, ALIVE);
            set_cell(old_grid, 39,42, ALIVE);
            set_cell(old_grid, 42,42, ALIVE);
            set_cell(old_grid, 39,41, ALIVE);
            set_cell(old_grid, 41,41, ALIVE);
            set_cell(old_grid, 40,40, ALIVE);

            count = 0;

            while(1){
                count++;
                compute_new_state(old_grid, new_grid);
                print_grid(new_grid, count);
                usleep(velocity);
                compute_new_state(new_grid,old_grid);
                print_grid(old_grid, count);
                usleep(velocity);
            }
        break;
        case 2:
            //SECOND PATTERN INFINITE GROWTH
            //
            set_cell(old_grid, 25,29, ALIVE);
            set_cell(old_grid, 27,29, ALIVE);
            set_cell(old_grid, 29,29, ALIVE);
            //
            set_cell(old_grid, 26,28, ALIVE);
            set_cell(old_grid, 27,28, ALIVE);
            set_cell(old_grid, 29,28, ALIVE);
            //
            set_cell(old_grid, 28,27, ALIVE);
            set_cell(old_grid, 29,27, ALIVE);
            //
     
            set_cell(old_grid, 25,26, ALIVE);
            //
            set_cell(old_grid, 25,25, ALIVE);
            set_cell(old_grid, 26,25, ALIVE);
            set_cell(old_grid, 27,25, ALIVE);
            set_cell(old_grid, 29,25, ALIVE);

            count = 0;

            while(1){
                count++;
                compute_new_state(old_grid, new_grid);
                print_grid(new_grid, count);
                usleep(velocity);
                compute_new_state(new_grid,old_grid);
                print_grid(old_grid, count);
                usleep(velocity);
            }
        break;
    }
    
    return 0;
}