#include "snake.h"
#include "field.h"

#include <cmath>
#include <ncurses.h>

Snake::Snake(Field* f, std::size_t init_size)
    : field{f}, size{init_size}, coordinates{}, next_dir{Directions::Right}
{
    // Get init position
    auto y = field->height() / 2;
    auto x = field->width() / 2;
    coordinates.emplace_back(y, x);

    // Fill body of snake
    for (std::size_t i=1; i<size; ++i)
        if ((*field)[y][--x] == 0) {
            coordinates.emplace_back(y, x);
            (*field)[y][x] = static_cast<char>(Color::green);
        }
        else {break;}
}

void Snake::show()
{
    // Print head
    auto coord = coordinates.front();
    move(coord.first+1, coord.second);
    attron(COLOR_PAIR(Color::cyan));
    switch(next_dir) {
        case Directions::Right:
            printw(">");
            break;
        case Directions::Left:
            printw("<");
            break;
        case Directions::Up:
            printw("^");
            break;
        case Directions::Down:
            printw("v");
            break;
    }
    attroff(COLOR_PAIR(Color::cyan));

    // Print body
    for (std::size_t i=1; i < size; ++i) {
        coord = coordinates[i];
        move(coord.first+1, coord.second);
        attron(COLOR_PAIR(Color::green));
        printw("=");
        attroff(COLOR_PAIR(Color::green));
    }

    // Print score
    move(field->height() + 1, 0);
    printw("Score: %d", size);
}

void Snake::increase_size()
{
    ++size;
}

bool Snake::make_move()
{
    auto coord = coordinates.front();

    switch(next_dir) {
        case Directions::Right:
            coord.second++;
            break;
        case Directions::Left:
            coord.second--;
            break;
        case Directions::Up:
            coord.first--;
            break;
        case Directions::Down:
            coord.first++;
            break;
    }

    if ((*field)[coord.first][coord.second] == static_cast<char>(Color::red)) { // Hit a fruit
        increase_size();
        field->eat_fruit(coord.first, coord.second);
    }
    else if ((*field)[coord.first][coord.second] != 0) { // Hit anything else
        return false;
    }

    (*field)[coord.first][coord.second] = static_cast<char>(Color::green);
    coordinates.push_front(std::move(coord));

    // Erase the tail of the snake to fit size
    if (coordinates.size() > size) {
        auto last = coordinates.back();
        (*field)[last.first][last.second] = 0;
        move(last.first+1, last.second);
        printw(" ");
        coordinates.pop_back();
    }

    return true;
}

void Snake::set_direction(Directions dir)
{
    if (std::abs(static_cast<int>(dir) - static_cast<int>(next_dir)) >= 2) { // Avoid 180 degrees turns
        next_dir = dir;
    }

}
