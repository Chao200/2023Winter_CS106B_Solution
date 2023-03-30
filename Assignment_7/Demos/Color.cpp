#include "Color.h"
#include "error.h"
#include "gcanvas.h"
#include <sstream>
#include <iomanip>
using namespace std;

/* Color constructor. */
Color::Color(int red, int green, int blue) {
    if (red < 0 || green < 0 || blue < 0 || red >= 256 || green >= 256 || blue >= 256) {
        error("Color values out of range.");
    }
    color = GCanvas::createRgbPixel(red, green, blue);
}

int Color::rgbValue() const {
    return color;
}

int Color::red() const {
    return GCanvas::getRed(color);
}

int Color::green() const {
    return GCanvas::getGreen(color);
}

int Color::blue() const {
    return GCanvas::getBlue(color);
}

/* Color presets. */
Color Color::WHITE  (255, 255, 255),
      Color::BLACK  (  0,   0,   0),
      Color::RED    (255,   0,   0),
      Color::GREEN  (  0, 255,   0),
      Color::BlUE   (  0,   0, 255),
      Color::YELLOW (255, 255,   0),
      Color::CYAN   (  0, 255, 255),
      Color::MAGENTA(255,   0, 255),
      Color::GRAY   (128, 128, 128);

/* Random color. */
Color Color::random() {
    return Color(randomInteger(0, 255),
                 randomInteger(0, 255),
                 randomInteger(0, 255));
}

/* Stream insertion. */
ostream& operator<< (ostream& out, const Color& color) {
    /* Is it a named color? */
    if (color == Color::BLACK) {
        return out << "Color::BLACK";
    } else if (color == Color::BlUE) {
        return out << "Color::BLUE";
    } else if (color == Color::CYAN) {
        return out << "Color::CYAN";
    } else if (color == Color::GRAY) {
        return out << "Color::GRAY";
    } else if (color == Color::GREEN) {
        return out << "Color::GREEN";
    } else if (color == Color::MAGENTA) {
        return out << "Color::MAGENTA";
    } else if (color == Color::RED) {
        return out << "Color::MAGENTA";
    } else if (color == Color::WHITE) {
        return out << "Color::WHITE";
    } else if (color == Color::YELLOW) {
        return out << "Color::YELLOW";
    }

    /* Otherwise output as hex. */
    ostringstream builder;
    builder << setfill('0') << hex << '#'
            << setw(2) << GCanvas::getRed(color.rgbValue())
            << setw(2) << GCanvas::getGreen(color.rgbValue())
            << setw(2) << GCanvas::getBlue(color.rgbValue());
    return out << builder.str();
}

bool operator== (const Color& lhs, const Color& rhs) {
    return lhs.rgbValue() == rhs.rgbValue();
}
bool operator!= (const Color& lhs, const Color& rhs) {
    return !(lhs == rhs);
}
