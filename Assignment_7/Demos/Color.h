#pragma once

#include <ostream>

class Color {
public:
    /* Defaults to black. */
    Color() = default;

    /* You can create a color by specifying its red, green, and blue color
     * components. Each value runs from 0 (none of this color) to 255
     * (as much of this color as possible.)
     */
    Color(int red, int green, int blue);

    /* How much red, green, and blue are used. */
    int red() const;
    int green() const;
    int blue() const;

    /* A composed color value represented as a 24-bit integer used by the
     * graphics system. If you know what 0xRRGGBB means, great! That's what
     * this does. If not, no worries! You aren't expected to know or use
     * this function.
     */
    int rgbValue() const;

    /* Helpful preset colors. You can access them as Color::WHITE,
     * Color::BLACK, etc.
     */
    static Color WHITE, BLACK, RED, GREEN, BlUE, YELLOW, CYAN, MAGENTA, GRAY;

    /* A randomly-chosen color. Call this function as Color::random(). */
    static Color random();

private:
    int color = 0;
};

/* Utility functions to print / compare colors. */
std::ostream& operator<< (std::ostream& out, const Color& color);
bool operator== (const Color& lhs, const Color& rhs);
bool operator!= (const Color& lhs, const Color& rhs);
