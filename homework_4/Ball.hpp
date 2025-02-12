#pragma once
#include "Painter.hpp"
#include "Point.hpp"
#include "Velocity.hpp"
#include "stdexcept"

class Ball {
public:
    Ball(double radius, const Color& color, bool isCollidable = true);
    ~Ball();

    void setVelocity(const Velocity& velocity);
    Velocity getVelocity() const;
    void draw(Painter& painter) const;
    void setCenter(const Point& center);
    Point getCenter() const;
    double getRadius() const;
    double getMass() const;
    bool isCollidable() const { return collidable; }
    void setCollidable(bool collidable) { this->collidable = collidable; }
private:
    Point center;  
    Velocity velocity;  
    Color color;
    double radius; 
    bool collidable; 
};
