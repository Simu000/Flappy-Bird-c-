
#include "flappy.h"

Game::Game()
{
    background = load_bitmap("background", "background.png");
    bird_image = load_bitmap("bird", "bird.png");
    game_font = load_font("arial.ttf", "arial.ttf");
    score = 0;
    bird_x = 100;
    bird_y = SCREEN_HEIGHT / 2;
    bird_velocity = 0;
    last_pipe_x_passed = 0;

    pipes.reserve(MAX_PIPES);
    for (int i = 0; i < MAX_PIPES; i++)
    {
        pipes.push_back(Pipe(SCREEN_WIDTH + i * 200.0));
    }
}

void Game::draw()
{
    clear_screen(COLOR_SKY_BLUE);

    draw_bitmap(background, -400, -300, option_scale_bmp(SCREEN_WIDTH / (float)bitmap_width(background), SCREEN_HEIGHT / (float)bitmap_height(background)));

    draw_bitmap(bird_image, bird_x - 250, bird_y - 250, option_scale_bmp(40 / (float)bitmap_width(bird_image), 40 / (float)bitmap_height(bird_image)));

    for (int i = 0; i < MAX_PIPES; i++)
    {
        fill_rectangle(COLOR_GREEN, pipes[i].x, 0, PIPE_WIDTH, pipes[i].top_height);
        fill_rectangle(COLOR_GREEN, pipes[i].x, pipes[i].bottom_y, PIPE_WIDTH, SCREEN_HEIGHT - pipes[i].bottom_y);
    }

    draw_text("Score: " + std::to_string(score), COLOR_WHITE, game_font, 20, SCREEN_WIDTH / 2 - 50, 10);

    refresh_screen(60);
}

void Game::flap()
{
    if (key_down(SPACE_KEY))
    {
        bird_velocity = -JUMP_STRENGTH;
    }
}

void Game::move_bird()
{
    bird_velocity += GRAVITY;
    bird_y += bird_velocity;

    if (bird_y >= SCREEN_HEIGHT - BIRD_RADIUS)
    {
        bird_y = SCREEN_HEIGHT - BIRD_RADIUS;
        bird_velocity = 0;
    }
}

void Game::move_pipes()
{
    for (int i = 0; i < MAX_PIPES; i++)
    {
        pipes[i].x -= PIPE_SPEED;
        pipes[i].check_passed(score, last_pipe_x_passed, bird_x);
    }

    if (pipes[0].x + PIPE_WIDTH < 0)
    {
        pipes[0].x = pipes[MAX_PIPES - 1].x + 200;
        pipes[0].top_height = rnd(50, SCREEN_HEIGHT - GAP_HEIGHT - 50);
        pipes[0].bottom_y = pipes[0].top_height + GAP_HEIGHT;

        for (int i = 0; i < MAX_PIPES - 1; i++)
        {
            pipes[i] = pipes[i + 1];
        }
    }
}

void Game::check_collision(GameState &game_state)
{
    if (bird_y <= 0 || bird_y >= SCREEN_HEIGHT - BIRD_RADIUS)
    {
        game_state = GAME_OVER;
    }

    for (int i = 0; i < MAX_PIPES; i++)
    {
        if ((bird_x + BIRD_RADIUS > pipes[i].x && bird_x - BIRD_RADIUS < pipes[i].x + PIPE_WIDTH) &&
            (bird_y - BIRD_RADIUS < pipes[i].top_height || bird_y + BIRD_RADIUS > pipes[i].bottom_y))
        {
            game_state = GAME_OVER;
        }
    }
}

void Game::reset_game()
{
    bird_y = SCREEN_HEIGHT / 2;
    bird_velocity = 0;
    score = 0;
    last_pipe_x_passed = 0;

    pipes.clear();
    for (int i = 0; i < MAX_PIPES; i++)
    {
        pipes.push_back(Pipe(SCREEN_WIDTH + i * 200.0));
    }
}

extern Game game;

Pipe::Pipe(double x_pos)
{
    x = x_pos;
    top_height = rnd(50, SCREEN_HEIGHT - GAP_HEIGHT - 50);
    bottom_y = top_height + GAP_HEIGHT;
}

void Pipe::check_passed(int &score, double &last_pipe_x_passed, double bird_x)
{
    if (bird_x > x + PIPE_WIDTH && last_pipe_x_passed < x)
    {
        last_pipe_x_passed = x;
        score++;
    }
}

void show_main_menu()
{
    clear_screen(COLOR_SKY_BLUE);
    bitmap screen = load_bitmap("screen", "screen.jpg");

    draw_bitmap(screen, -350, -200, option_scale_bmp(SCREEN_WIDTH / (float)bitmap_width(screen), SCREEN_HEIGHT / (float)bitmap_height(screen)));

    draw_text("Press N for New Game", COLOR_WHITE, game.game_font, 30, SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 180);
    draw_text("Press E to Exit", COLOR_WHITE, game.game_font, 30, SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 230);
    refresh_screen(60);
}

void handle_main_menu_input(GameState &state)
{
    if (key_typed(N_KEY))
    {
        game.reset_game();
        state = GAME_RUNNING;
    }
    else if (key_typed(E_KEY))
    {
        state = QUIT_GAME;
    }
}

void show_game_over_screen()
{
    clear_screen(COLOR_SKY_BLUE);
    draw_text("Game Over!", COLOR_RED, game.game_font, 80, SCREEN_WIDTH / 2 - 220, SCREEN_HEIGHT / 2 - 100);
    draw_text("Press R to Restart or E to Exit", COLOR_WHITE, game.game_font, 30, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2);
    draw_text("Press L for Leaderboard", COLOR_WHITE, game.game_font, 30, SCREEN_WIDTH / 2 - 170, SCREEN_HEIGHT / 2 + 40);
    fill_rectangle(COLOR_WHITE, 10, 10, 50, 50);
    fill_triangle(COLOR_RED, 10, 10, 10, 60, 60, 35);

    if (key_typed(L_KEY))
    {
        draw_text("Scores: " + std::to_string(game.score), COLOR_WHITE, game.game_font, 30, SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 100);
    }

    refresh_screen(60);
}

void handle_game_over_input(GameState &state)
{
    if (key_typed(R_KEY))
    {
        game.reset_game();
        state = GAME_RUNNING;
    }
    else if (key_typed(E_KEY))
    {
        state = QUIT_GAME;
    }
    else if (mouse_clicked(LEFT_BUTTON))
    {
        if (mouse_x() >= 10 && mouse_x() <= 60 && mouse_y() >= 10 && mouse_y() <= 60)
        {
            state = MAIN_MENU;
        }
    }
    else if (key_typed(L_KEY))
    {
        show_game_over_screen();
        delay(3000);
    }
}
