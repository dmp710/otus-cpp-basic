#pragma once
#include "Point.hpp"
#include <cmath>

class Velocity {
  public:
    inline Velocity() = default;

    // конструктор использует угол в радианах и абсолютное значение скорости, чтобы вычислить компоненты скорости по осям x и y
    inline Velocity(double abs, double angle) {
        const double x = std::cos(angle);
        const double y = std::sin(angle);
        vec = Point{x, y} * abs;
    }
    // конструктор просто передает вектор в метод setVector для инициализации
    inline Velocity(const Point& vector) {
        setVector(vector);
    }

    // устанавливает внутреннюю переменную вектор скорости
    inline void setVector(const Point& vector) {
        vec = vector;
    }

    inline Point vector() const {
        return vec;
    }

  private:
    Point vec;
};
