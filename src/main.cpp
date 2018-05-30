#include "field.h"
#include "snake.h"

#include <ncurses.h>
#include <chrono>

int main()
{
    using namespace std::chrono;

    Field field (30, 60);
    Snake snake (&field, 6);

    bool exit_flag = false;
    int ch;
    auto t1 = steady_clock::now();
    field.show();

    while (!exit_flag) {

        ch = getch();
        switch(ch) {
            case KEY_UP: case 'w':
                snake.set_direction(Directions::Up);
                break;
            case KEY_DOWN: case 's':
                snake.set_direction(Directions::Down);
                break;
            case KEY_LEFT: case 'a':
                snake.set_direction(Directions::Left);
                break;
            case KEY_RIGHT: case 'd':
                snake.set_direction(Directions::Right);
                break;
            case 'q': case KEY_EXIT: case 27:
                exit_flag = true;
                break;
            default:
                break;
        }

        // add an obstacle every 10 seconds
        auto t2 = steady_clock::now();
        auto duration = duration_cast<seconds>( t2 - t1 ).count();
        if (duration > 10) {
            field.add_random_obstacle(snake.get_coordinate());
            field.show();
            t1 = t2;
        }

        if (snake.make_move()) {
            snake.show();
        }
        else { // Snake hit somehing -> Endgame
            exit_flag = true;
            field.endgame(snake.score());
        }

        refresh();
    }

    return 0;
}
