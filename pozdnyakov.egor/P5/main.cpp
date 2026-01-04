#include <iostream>

namespace pozdnyakov {
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

	class Diamond : public Shape {
	private:
		point_t center;
		double diag_h;
		double diag_v;

	public:
		Diamond(const point_t& center, double diag_h, double diag_v)
			: center(center), diag_h(diag_h), diag_v(diag_v) {
		}

		double getArea() const override {
			return (diag_h * diag_v) / 2.0;
		}

		rectangle_t getFrameRect() const override {
			return { diag_h, diag_v, center };
		}

		void move(const point_t& pos) override {
			center = pos;
		}

		void move(double dx, double dy) override {
			center.x += dx;
			center.y += dy;
		}

		void scale(double k) override {
			if (k < 0.0) return;
			diag_h *= k;
			diag_v *= k;
		}
	};

	class Triangle : public Shape {
	private:
		point_t p1, p2, p3;

		point_t getCenter() const {
			return { (p1.x + p2.x + p3.x) / 3.0, (p1.y + p2.y + p3.y) / 3.0 };
		}

	public:
		Triangle(const point_t& a, const point_t& b, const point_t& c)
			: p1(a), p2(b), p3(c) {
		}

		double getArea() const override {
			return 0.5 * std::abs((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y));
		}

		rectangle_t getFrameRect() const override {
			double min_x = std::min({ p1.x, p2.x, p3.x });
			double max_x = std::max({ p1.x, p2.x, p3.x });
			double min_y = std::min({ p1.y, p2.y, p3.y });
			double max_y = std::max({ p1.y, p2.y, p3.y });

			double width = max_x - min_x;
			double height = max_y - min_y;

			point_t rect_pos = { min_x + width / 2.0, min_y + height / 2.0 };

			return { width, height, rect_pos };
		}

		void move(const point_t& pos) override {
			point_t current_center = getCenter();
			double dx = pos.x - current_center.x;
			double dy = pos.y - current_center.y;
			move(dx, dy);
		}

		void move(double dx, double dy) override {
			p1.x += dx; p1.y += dy;
			p2.x += dx; p2.y += dy;
			p3.x += dx; p3.y += dy;
		}

		void scale(double k) override {
			if (k < 0.0) return;

			point_t center = getCenter();

			p1.x = center.x + (p1.x - center.x) * k;
			p1.y = center.y + (p1.y - center.y) * k;

			p2.x = center.x + (p2.x - center.x) * k;
			p2.y = center.y + (p2.y - center.y) * k;

			p3.x = center.x + (p3.x - center.x) * k;
			p3.y = center.y + (p3.y - center.y) * k;
		}
	};
}

int main() {
	using namespace pozdnyakov;

	Shape** shapes = new Shape * [3];

	shapes[0] = new Rectangle({ 0, 0 }, 10, 5);
	shapes[1] = new Diamond({ 10, 0 }, 10, 10);
	shapes[2] = new Triangle({ 0, 0 }, { 5, 10 }, { 10, 0 });

	for (int i = 0; i < 3; ++i) {
		delete shapes[i];
	}
	delete[] shapes;

	return 0;
}
