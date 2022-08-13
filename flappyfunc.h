// Collin Bowers
// Flappy Bird Header File

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "gfx2.h"
#include <math.h>

typedef struct {
    double xt_1; 
    double yt_1;
    double ht_1;
    double xt_2;
    double yt_2;
    double ht_2;
    double vx_t;
} Tube;

void start_screen(int xsize, int ysize);
void draw_ground(int xsize, int ysize);
void initialize_tube(Tube tubes[], int i, int xsize, int ysize, Tube *p);
void bird(double xc, double yc, int radius);
void draw_tube(Tube tubes[], int i);
void move_tubes(Tube tubes[], int xsize, int ysize, Tube *p, bool *first, bool *init);
bool check_dead(double xc, double yc, int radius, Tube tubes[], int i, int xsize, int ysize, int *s);
void end_screen(int xsize, int ysize, int *s);