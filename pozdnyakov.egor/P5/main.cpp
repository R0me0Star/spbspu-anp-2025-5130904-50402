#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

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
    virtual const char* getName() const = 0;
    virtual ~Shape() = default;
  };

  class Rectangle : public Shape {
  private:
    rectangle_t frame;
  public:
    Rectangle(const point_t& center, double width, double height) {
      frame.pos = center;
      frame.width = width;
      frame.height = height;
    }

    double getArea() const override {
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
      frame.width *= k;
      frame.height *= k;
    }

    const char* getName() const override {
      return "Rectangle";
    }
  };

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
      diag_h *= k;
      diag_v *= k;
    }

    const char* getName() const override {
      return "Diamond";
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
      point_t center = getCenter();
      p1.x = center.x + (p1.x - center.x) * k;
      p1.y = center.y + (p1.y - center.y) * k;
      p2.x = center.x + (p2.x - center.x) * k;
      p2.y = center.y + (p2.y - center.y) * k;
      p3.x = center.x + (p3.x - center.x) * k;
      p3.y = center.y + (p3.y - center.y) * k;
    }

    const char* getName() const override {
      return "Triangle";
    }
  };

  void scaleShapeAtPoint(Shape* shape, const point_t& target, double k) {
    point_t center = shape->getFrameRect().pos;
    double dx = (center.x - target.x) * (k - 1.0);
    double dy = (center.y - target.y) * (k - 1.0);

    shape->move(dx, dy);
    shape->scale(k);
  }

  void printShapesInfo(Shape** shapes, size_t count) {
    std::cout << std::fixed << std::setprecision(1);

    double totalArea = 0.0;

    double global_min_x = 0, global_max_x = 0;
    double global_min_y = 0, global_max_y = 0;
    bool first = true;

    for (size_t i = 0; i < count; ++i) {
      double area = shapes[i]->getArea();
      totalArea += area;
      rectangle_t frame = shapes[i]->getFrameRect();

      std::cout << shapes[i]->getName()
        << ": Area=" << area
        << ", FrameCenter=(" << frame.pos.x << ", " << frame.pos.y << ")"
        << ", w=" << frame.width << ", h=" << frame.height << "\n";

      double half_w = frame.width / 2.0;
      double half_h = frame.height / 2.0;
      double left = frame.pos.x - half_w;
      double right = frame.pos.x + half_w;
      double bottom = frame.pos.y - half_h;
      double top = frame.pos.y + half_h;

      if (first) {
        global_min_x = left; global_max_x = right;
        global_min_y = bottom; global_max_y = top;
        first = false;
      }
      else {
        if (left < global_min_x) global_min_x = left;
        if (right > global_max_x) global_max_x = right;
        if (bottom < global_min_y) global_min_y = bottom;
        if (top > global_max_y) global_max_y = top;
      }
    }

    std::cout << "Total Area: " << totalArea << "\n";

    if (!first) {
      double global_w = global_max_x - global_min_x;
      double global_h = global_max_y - global_min_y;
      double global_cx = global_min_x + global_w / 2.0;
      double global_cy = global_min_y + global_h / 2.0;

      std::cout << "Global FrameRect: center=(" << global_cx << ", " << global_cy << ")"
        << ", w=" << global_w << ", h=" << global_h << "\n";
    }
    std::cout << "-----------------------------------------------------------\n";
  }
}

int main() {
  using namespace pozdnyakov;

  const size_t count = 3;
  Shape** shapes = new Shape * [count];

  shapes[0] = new Rectangle({ 5.0, 5.0 }, 10.0, 5.0);
  shapes[1] = new Diamond({ 20.0, 5.0 }, 10.0, 10.0);
  shapes[2] = new Triangle({ 0.0, 0.0 }, { 5.0, 10.0 }, { 10.0, 0.0 });

  std::cout << "--- Before Scaling ---\n";
  printShapesInfo(shapes, count);

  point_t targetPoint;
  double k;

  std::cout << "Enter scaling center (x y): ";
  if (!(std::cin >> targetPoint.x >> targetPoint.y)) {
    std::cerr << "Invalid coordinates input.\n";
    for (size_t i = 0; i < count; ++i) delete shapes[i];
    delete[] shapes;
    return 1;
  }

  std::cout << "Enter scaling coefficient (k >= 0): ";
  if (!(std::cin >> k)) {
    std::cerr << "Invalid input for coefficient.\n";
    for (size_t i = 0; i < count; ++i) delete shapes[i];
    delete[] shapes;
    return 1;
  }

  if (k < 0.0) {
    std::cerr << "Scaling coefficient must be non-negative.\n";
    for (size_t i = 0; i < count; ++i) delete shapes[i];
    delete[] shapes;
    return 1;
  }

  for (size_t i = 0; i < count; ++i) {
    scaleShapeAtPoint(shapes[i], targetPoint, k);
  }

  std::cout << "\n--- After Scaling ---\n";
  printShapesInfo(shapes, count);

  for (size_t i = 0; i < count; ++i) {
    delete shapes[i];
  }
  delete[] shapes;

  return 0;
}
