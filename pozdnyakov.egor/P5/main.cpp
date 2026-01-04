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

class Rectangle : public Shape {
private: 
	rectangle_t frame;
};
public:
	Rectangle(const point_t& center, double width, double height) {
		frame.pos = center;
		frame.width = width;
		frame.height = height;
	}

	double get Area() const override {
		return frame.width * frame.height;
	}

	rectangle_t getFrameRect() const override {
		return frame;
	}

	void move(const point_t& pos) override {
		frame.pos = pos;
	}

	void move(double dx, double dy) override {
		frame.pos.x += dx;
		frame.pos.y += dy;
	}

	void scale(double k) override {
		if (k < 0.0) return;
		frame.width *= k;
		frame.height *= k;
	}
