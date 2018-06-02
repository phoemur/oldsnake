#include "field.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>
#include <ncurses.h>

Field::Field(std::size_t y_size, std::size_t x_size)
    : x{x_size}, y{y_size}, numfruits{0}, field{}, seeder{}, engine{seeder()}
{
    initscr();
    if(has_colors() == FALSE) {
        endwin();
        std::cout << "Your terminal does not support colors\n";
        exit(1);
    }

    noecho();
    cbreak();
    timeout(100);
    keypad(stdscr, TRUE);
    start_color();
    curs_set(0);
    init_pair(1, COLOR_RED, COLOR_BLACK);    // must match enum class Color
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_WHITE);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);

    // Test terminal size
    int test_x, test_y;
    getmaxyx(stdscr, test_y, test_x);
    if (x == 1 and y == 1) {
        y = test_y - 2; // spare 2 rows to print information
        x = test_x - 2;
    }
    else {
        if (x+2 > test_x) x = test_x-2;
        if (y+2 > test_y) y = test_y-2;
    }

    dist_x = std::uniform_int_distribution<std::size_t>(0, x-1);
    dist_y = std::uniform_int_distribution<std::size_t>(0, y-1);

    init_field();
}

Field::~Field()
{
    endwin(); //restore terminal
}

void Field::init_field()
{
    field = std::vector<std::vector<char>> (y, std::vector<char>(x, 0));

    std::fill(field[0].begin(), field[0].end(), static_cast<char>(Color::white));
    std::fill(field.back().begin(), field.back().end(), static_cast<char>(Color::white));

    for (auto& row: field) {
        row.back() = static_cast<char>(Color::white);
        row.front() = static_cast<char>(Color::white);
    }

    add_random_fruit();
    add_random_fruit();
}

void Field::show()
{
    move(0, 0);
    printw("Press q to quit. Eat the red fruits. Avoid the obstacles.\n");
    for (std::size_t i=0; i<y; ++i) {
        for (std::size_t j=0; j<x; ++j) {
            if (field[i][j] == static_cast<char>(Color::white)) {
                attron(COLOR_PAIR(Color::white));
                printw("*");
                attroff(COLOR_PAIR(Color::white));
            }
            else if (field[i][j] == static_cast<char>(Color::red)) {
                attron(COLOR_PAIR(Color::red));
                printw("O");
                attroff(COLOR_PAIR(Color::red));
            }
            else {
                printw(" ");
            }
        }
        printw("\n");
    }
}

void Field::add_random_obstacle(const std::pair<std::size_t, std::size_t>& snakehead)
{
    std::size_t y=0, x=0;

    while (field[y][x] != 0) {
        y = dist_y(engine);
        x = dist_x(engine);

        // Ensure new obstacle is at least 8 cells far from snake's head
        if (std::hypot(y - snakehead.first, x - snakehead.second) < 8) {
            x = 0;
            y = 0;
        }
    }

    field[y][x] = static_cast<char>(Color::white);
}

void Field::add_random_fruit()
{
    std::size_t y=0, x=0;

    while (field[y][x] != 0) {
        y = dist_y(engine);
        x = dist_x(engine);
    }

    field[y][x] = static_cast<char>(Color::red);
    ++numfruits;
}

void Field::endgame(std::size_t score)
{
    move(0, 0);
    clear();
    refresh();
    timeout(-1);
    attron(COLOR_PAIR(Color::cyan));
    printw("You hit something... Game Over... Score: %d \nPress Q to Exit...", score);
    for (int c = 0; c != 'q' && c != 'Q'; c = getch()) {}

    attroff(COLOR_PAIR(Color::cyan));
}

void Field::eat_fruit(std::size_t y, std::size_t x)
{
    --numfruits;
    field[y][x] = 0;

    if (numfruits == 0) {
        add_random_fruit();
        add_random_fruit();
        show();
    }
}
