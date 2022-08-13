// Collin Bowers
// Flappy Bird Functions File

#include <stdio.h>
#include "flappyfunc.h"

// This function shows the start screen to the user, with the necessary information needed to play
void start_screen(int xsize, int ysize)
{
    gfx_color(247, 240, 14);
    char *thefont = "12x24"; // Update the font
    gfx_changefont(thefont);
    gfx_text(xsize/2-50, ysize/2-150, "Flappy Bird");
    gfx_text(xsize/2-110, ysize/2-50, "Press spacebar to play");
    gfx_text(xsize/2-85, ysize/2+50, "Press 'q' to quit");
    gfx_text(xsize/2-175, ysize/2+150, "Use the spacebar to fly the bird");
}

// This function draws the ground with the proper color
void draw_ground(int xsize, int ysize)
{
    int height = 75;
    gfx_color(224, 215, 150);
    gfx_fill_rectangle(0, ysize-height, xsize, height);
}

// This function initializes each tube to the proper position
void initialize_tube(Tube tubes[], int i, int xsize, int ysize, Tube *p)
{
    int width = 90, max_height = 400, min_height = 130, in_between = 200, above_ground = 75;
    p = &tubes[i]; // Update pointer to proper place in the array
    p->xt_1 = xsize-width;
    p->yt_1 = 0;
    p->ht_1 = rand()%(max_height+1 - min_height) + min_height; // The height of the top tube is a random number between 130 and 400 units vertically down from the x axis
    p->xt_2 = xsize-width;
    p->yt_2 = tubes[i].ht_1 + in_between; // The starting position of the bottom tube depends on the random height of the top tube
    p->ht_2 = ysize - tubes[i].yt_2 - above_ground;
    p->vx_t = 10;
}

// Draw the bird with the proper color and at the proper position
void bird(double xc, double yc, int radius)
{
    gfx_color(247, 240, 14);
    gfx_fill_circle(xc, yc, radius);
    gfx_flush();
}

// Draw both of the tubes with the proper dimensions determined above
void draw_tube(Tube tubes[], int i)
{
    int width = 90;
    gfx_color(21, 164, 16); // Display them with the proper color
    gfx_fill_rectangle(tubes[i].xt_1, tubes[i].yt_1, width, tubes[i].ht_1);
    gfx_fill_rectangle(tubes[i].xt_2, tubes[i].yt_2, width, tubes[i].ht_2);
    gfx_flush();
}

// Move the tubes across the screen
void move_tubes(Tube tubes[], int xsize, int ysize, Tube *p, bool *first, bool *init)
{
    // If it is the beginning of the game and there is only one tube on the screen, that is the only one we draw
    if (*first == true) {
        p = &tubes[0];
        p->xt_1 -= p->vx_t;
        p->xt_2 -= p->vx_t;
        draw_tube(tubes, 0);
    }
    // Otherwise, draw both tubes
    else {
        p = &tubes[0];
        p->xt_1 -= p->vx_t;
        p->xt_2 -= p->vx_t;
        p = &tubes[1];
        p->xt_1 -= p->vx_t;
        p->xt_2 -= p->vx_t;
        draw_tube(tubes, 0);
        draw_tube(tubes, 1);
    }

    // If we drew only the first tube and it has crossed a little beyond the middle of the screen, initialize the second tube
    if (*first == true && tubes[0].xt_1 < xsize/2.6) {
        initialize_tube(tubes, 1, xsize, ysize, p);
        *first = false;
    }
    // Once either the first or second tube has crossed a little beyond the middle of the screen, initialize the other tube
    else if (*first == false && *init == true && tubes[0].xt_1 < xsize/2.6) {
        initialize_tube(tubes, 1, xsize, ysize, p);
        *init = false;
    }
    else if (*first == false && *init == false && tubes[1].xt_1 < xsize/2.6) {
        initialize_tube(tubes, 0, xsize, ysize, p);
        *init = true;
    }
}

// Function to check if the bird is dead and to also update the score
bool check_dead(double xc, double yc, int radius, Tube tubes[], int i, int xsize, int ysize, int *s)
{   
    int width = 90;
    int ground = ysize - 75;
    int middle = xsize/2.6;
    double angle;
    double point_x;
    double point_y;
    for (angle = 0 ; angle <= 2*M_PI; angle += 0.1) {
        point_x = xc + radius*cos(angle); // Check points on the edge of the bird for overlap with the tubes
        point_y = yc + radius*sin(angle);
        if (point_y > ground) { // The bird dies if it hits the ground
            return true;
        }
        if (point_x >= tubes[i].xt_1 && point_x <= tubes[i].xt_1+width) { // If the bird is in between the tubes
            if (point_y <= tubes[i].ht_1 && point_y >= tubes[i].ht_1-3) { // If the bird hits the top tube then it dies
                bird(xc, tubes[i].ht_1+radius, radius);
                usleep(90000);
                return true;
            }
            if (point_y >= tubes[i].yt_2 || point_y <= tubes[i].ht_1-10) { // If the bird hits the bottom tube then it dies
                return true;
            }
        }
    }
    if (tubes[i].xt_1+width <= xc-radius && tubes[i].xt_1+width >= xc-radius-3) {
        (*s)++; // If the bird's leftmost edge passes the rightmost part of the tube, increment the score
    }

    return false;
}

// Display the end screen to the user
void end_screen(int xsize, int ysize, int *s)
{
    gfx_color(247, 240, 14);
    char *thefont = "12x24"; // Update the font
    gfx_changefont(thefont);
    gfx_text(xsize/2-50, ysize/2-100, "You died");

    char score_str[3];
    sprintf(score_str, "Score: %d", *s);
    gfx_text(xsize/2-50, ysize/2 - 50, score_str); // Display the user's score
    gfx_text(xsize/2-159, ysize/2, "Press spacebar to play again");
    gfx_text(xsize/2-100, ysize/2 + 50, "Press 'q' to quit");
}
