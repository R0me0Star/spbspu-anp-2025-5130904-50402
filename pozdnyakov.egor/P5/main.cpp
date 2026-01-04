#include <iostream>

struct point_t {
	double x;
	double y;
};

struct rectangle_t {
	double width;
	double height;
	point_t pos;
};

class Shape {
public: 
	virtual double getArea() const = 0;
	virtual rectangle_t getFrameRect() const = 0;

	virtual void move(const point_t& pos) = 0;

	virtual void move(double dx, double dy) = 0;

	virtual void scale(double k) = 0;

	virtual ~Shape() = default;
};
