#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <random>

enum class Color {
    red=1, green, white, cyan
};

class Field {
    std::size_t x, y, numfruits;
    std::vector<std::vector<char>> field;

    std::random_device seeder;
    std::mt19937 engine;
    std::uniform_int_distribution<std::size_t> dist_x, dist_y;

public:
    Field(std::size_t y_size = 1, std::size_t x_size = 1);
    ~Field();

    void show();
    void add_random_obstacle(const std::pair<std::size_t, std::size_t>& snakehead);
    void add_random_fruit();
    void endgame(std::size_t score);
    void eat_fruit(std::size_t y, std::size_t x);

    std::vector<char>& operator[](std::size_t y) {return field[y];}
    const std::vector<char>& operator[](std::size_t y) const {return field[y];}
    std::size_t width()  const {return x;}
    std::size_t height() const {return y;}

private:
    void init_field();
};

#endif // FIELD_H
