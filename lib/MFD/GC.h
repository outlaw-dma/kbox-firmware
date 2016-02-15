#pragma once

#include <stdint.h>

class Point {
  private:
    uint16_t _x, _y;

  public:
    Point(uint16_t x, uint16_t y) : _x(x), _y(y) {};

    uint16_t x() const {
      return _x;
    };
    uint16_t y() const {
      return _y;
    };
};

static const Point Origin = Point(0, 0);

class Size {
  private:
    uint16_t _width, _height;

  public:
    Size(uint16_t w, uint16_t h) : _width(w), _height(h) {};

    uint16_t width() const {
      return _width;
    };

    uint16_t height() const {
      return _height;
    };
};

class Rectangle {
  private:
    Point _origin;
    Size _size;

  public:
    Rectangle(Point origin, Size size) : _origin(origin), _size(size) {};
    Rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) : _origin(x1, y1), _size(x2 - x1, y2 - y1) {};
};

typedef uint16_t Color;

// FIXME: Color definitions change with displays. This should be made display-dependent.
static const Color ColorBlue = 0x001F;
static const Color ColorRed = 0xF800;
static const Color ColorWhite = 0xFFFF;
static const Color ColorBlack = 0x0000;

enum Font {
  FontDefault
};

/* Graphics Context offers basic drawing primitives. */
class GC {
  public:
    virtual void drawText(Point a, Font font, Color color, const char *text) = 0;
    virtual void drawLine(Point a, Point b, Color color) = 0;
    virtual void drawRectangle(Point orig, Size size, Color color) = 0;
    virtual void fillRectangle(Point orig, Size size, Color color) = 0;
};