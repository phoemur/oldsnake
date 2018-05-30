#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include <utility>

// forward declaration
class Field;

enum class Directions {
    Up=1, Down=2, Left=5, Right=6
};

class Snake {
    Field* field;
    std::size_t size;
    std::deque<std::pair<std::size_t, std::size_t>> coordinates; // Store all pieces of the snake's body

    Directions next_dir;
public:
    Snake(Field* f, std::size_t init_size);

    void show();
    bool make_move();
    void increase_size();
    void set_direction(Directions dir);
    std::pair<std::size_t, std::size_t> get_coordinate() const {return coordinates.front();}
    std::size_t score() const {return size;}
};

#endif // SNAKE_H
