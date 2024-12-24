#include "flappy.h"
Game game;
int main()
{
    open_window("Flappy Bird", SCREEN_WIDTH, SCREEN_HEIGHT);

    GameState state = MAIN_MENU;

    while (state != QUIT_GAME)
    {
        process_events();

        if (quit_requested())
        {
            state = QUIT_GAME;
        }

        switch (state)
        {
        case MAIN_MENU:
            show_main_menu();
            handle_main_menu_input(state);
            break;

        case GAME_RUNNING:
            game.flap();
            game.move_bird();
            game.move_pipes();
            game.check_collision(state);
            game.draw();
            break;

        case GAME_OVER:
            show_game_over_screen();
            handle_game_over_input(state);
            break;

        default:
            break;
        }
    }

    return 0;
}
