// Collin Bowers
// Flappy Bird Main Driver

#include <stdio.h>
#include "flappyfunc.h"

int main()
{
    // Initialize variables
    const int xsize = 800, ysize = 700, num_tubes = 2;
    double xb = xsize/2, yb = ysize/2.5, vy_b = 0, ay = 1.7;
    int pause = 30000, score = 0, radius = 17;
    char c;
    int i;
    int *s = &score;
    srand(time(0)); // Set a new seed for random number generation

    Tube tubes[num_tubes];
    Tube *p = tubes;
    initialize_tube(tubes, 0, xsize, ysize, p);

    // Initialize booleans
    bool first_run = true, init_tube_1 = true, dead = false, game_over = false, start = true, play_again = false;
    bool first_subsequent_run = true, change_color = true;
    bool *first = &first_run, *init = &init_tube_1;
    
    gfx_open(xsize, ysize, "Flappy Bird"); // Open the gfx window

    while (true) {
        gfx_clear_color(39, 172, 220); // Ensure background color is updated each time
        play_again = false;

        // If the game is just starting, execute the following
        if (start == true) {
            if (change_color == true) { // On the start screen, need to clear the screen and then update the color otherwise the color will not be updated
                gfx_clear();
                change_color = false;
            }
            gfx_clear_color(39, 172, 220);
            start_screen(xsize, ysize); // Call the start screen function
            if (gfx_event_waiting()) {
                c = gfx_wait();
                if (c == 32) {
                    start = false;
                    draw_tube(tubes, 0); // If spacebar is pressed, draw the first tube and begin the game
                }
                if (c == 'q') break;
            }
            continue;
        }

        draw_ground(xsize, ysize); // Draw the ground

        // This for loop iterates over each tube to check if the bird has hit it. It also prints the score
        for (i = 0; i < num_tubes; i++) {
            dead = check_dead(xb, yb, radius, tubes, i, xsize, ysize, s);
            char score_str[3];
            sprintf(score_str, "%d", *s);
            gfx_text(xsize/2, ysize/7, score_str);
            if (dead == true) {
                game_over = true;
                break;
            }
        }

        // If the game has ended, execute the following
        if (game_over == true) {
            gfx_clear();
            while (true) {
                end_screen(xsize, ysize, s); // If the game is over, stay on the end screen until the user quits or presses the spacebar
                if (gfx_event_waiting()) {
                    c = gfx_wait();
                    if (c == 32) {
                        game_over = false;
                        play_again = true;
                        break;
                    }
                    if (c == 'q') {
                        play_again = false;
                        break;
                    }  
                }
            }
            // If the user chooses to play again, reinitialize bird position and both of the tubes
            if (play_again == true) {
                xb = xsize/2;
                yb = ysize/2.5;
                bird(xb, yb, radius);
                vy_b = 0;
                ay = 1.7; // Update acceleration to original value
                *s = 0;
                first_run = true;
                initialize_tube(tubes, 0, xsize, ysize, p);
                initialize_tube(tubes, 1, xsize, ysize, p);
                continue; // Continue to next iteration
            }
            else if (play_again == false) break;
        }
            
        bird(xb, yb, radius); // Draw the bird at its current coordinates
        
        move_tubes(tubes, xsize, ysize, p, first, init); // Call the function to move the tubes across the screen

        // If user presses the space bar, move the bird's position up, set y velocity to zero and update the acceleration
        if (gfx_event_waiting()) {
            c = gfx_wait();
            if (c == 32) {
                yb -= 5*vy_b;
                vy_b = 0;
                ay = 1.7;
            }
            if (c == 'q') break;
        }
        yb += vy_b; // Move bird's position by the current y velocity
        vy_b += ay; // Use acceleration to increment the y velocity
        if (vy_b > 25) ay = 0; // If the y velocity is greater than a certain amount then it stops accelerating
        first_subsequent_run = false;
        usleep(pause); 
        gfx_clear();
    }

    return 0;
}