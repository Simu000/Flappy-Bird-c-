#ifndef FLAPPY_H
#define FLAPPY_H

#include <splashkit.h>
#include <string>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PIPE_WIDTH = 50;
const int GAP_HEIGHT = 150;
const int PIPE_SPEED = 3;
const int BIRD_RADIUS = 20;
const int GRAVITY = 1;
const int JUMP_STRENGTH = 5;
const int MAX_PIPES = 100;

enum GameState
{
    MAIN_MENU,
    GAME_RUNNING,
    GAME_OVER,
    QUIT_GAME
};

class Pipe
{
public:
    double x;
    double top_height;
    double bottom_y;

    Pipe(double x_pos);
    void check_passed(int &score, double &last_pipe_x_passed, double bird_x);
};

class Game
{
public:
    bitmap background;
    bitmap bird_image;
    font game_font;
    int score;
    double bird_x;
    double bird_y;
    int bird_velocity;
    double last_pipe_x_passed;
    std::vector<Pipe> pipes;

    Game();
    void draw();
    void flap();
    void move_bird();
    void move_pipes();
    void check_collision(GameState &game_state);
    void reset_game();
};

void show_main_menu();
void handle_main_menu_input(GameState &state);
void show_game_over_screen();
void handle_game_over_input(GameState &state);

#endif 
