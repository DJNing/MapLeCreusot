#ifndef OSMIUM_OSM_BOX_HPP
#define OSMIUM_OSM_BOX_HPP

/*

This file is part of Osmium (https://osmcode.org/libosmium).

Copyright 2013-2019 Jochen Topf <jochen@topf.org> and others (see README).

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/

#include <osmium/osm/location.hpp>

#include <cassert>
#include <iosfwd>

namespace osmium {

    /**
     * Bounding box. A box is defined by two locations (bottom left location
     * and top right location) or, alternatively by four coordinates (minx,
     * miny, maxx, and maxy). If both locations are undefined, the box is
     * undefined, too.
     */
    class Box {

        osmium::Location m_bottom_left{};
        osmium::Location m_top_right{};

    public:

        /**
         * Create undefined Box. Use the extend() function
         * to add actual bounds.
         */
        constexpr Box() noexcept = default;

        /**
         * Create box from minimum and maximum coordinates.
         *
         * @pre @code minx <= maxx && miny <= maxy @endcode
         */
        Box(double minx, double miny, double maxx, double maxy) :
            m_bottom_left(minx, miny),
            m_top_right(maxx, maxy) {
            assert(minx <= maxx && miny <= maxy);
        }

        /**
         * Create box from bottom left and top right locations.
         *
         * @pre Either both locations must be defined or neither.
         * @pre If both locations are defined, the
         *      bottom left location must actually be to the left and below
         *      the top right location. Same coordinates for bottom/top or
         *      left/right are also okay.
         */
        Box(const osmium::Location& bottom_left, const osmium::Location& top_right) :
            m_bottom_left(bottom_left),
            m_top_right(top_right) {
            assert(
                (!!bottom_left && !!top_right) ||
                (bottom_left.x() <= top_right.x() && bottom_left.y() <= top_right.y())
            );
        }

        /**
         * Extend this bounding box by the specified location. If the
         * location is invalid, the bounding box is unchanged. If the
         * box is undefined it will only contain the new location after
         * this call.
         *
         * @param location The location we want to extend the box by.
         * @returns A reference to this box.
         */
        Box& extend(const Location& location) noexcept {
            if (location.valid()) {
                if (m_bottom_left) {
                    if (location.x() < m_bottom_left.x()) {
                        m_bottom_left.set_x(location.x());
                    }
                    if (location.x() > m_top_right.x()) {
                        m_top_right.set_x(location.x());
                    }
                    if (location.y() < m_bottom_left.y()) {
                        m_bottom_left.set_y(location.y());
                    }
                    if (location.y() > m_top_right.y()) {
                        m_top_right.set_y(location.y());
                    }
                } else {
                    m_bottom_left = location;
                    m_top_right = location;
                }
            }
            return *this;
        }

        /**
         * Extend this bounding box by the specified box. If the
         * specified box is undefined, the bounding box is unchanged.
         *
         * @param box The box to extend by.
         * @returns A reference to this box.
         */
        Box& extend(const Box& box) noexcept {
            extend(box.bottom_left());
            extend(box.top_right());
            return *this;
        }

        /**
         * Box is defined, ie. contains defined locations.
         */
        explicit constexpr operator bool() const noexcept {
            return bool(m_bottom_left) && bool(m_top_right);
        }

        /**
         * Box is valid, ie. defined and inside usual bounds
         * (-180<=lon<=180, -90<=lat<=90).
         */
        constexpr bool valid() const noexcept {
            return bottom_left().valid() && top_right().valid();
        }

        /**
         * Access bottom-left location.
         */
        constexpr Location bottom_left() const noexcept {
            return m_bottom_left;
        }

        /**
         * Access bottom-left location.
         */
        Location& bottom_left() noexcept {
            return m_bottom_left;
        }

        /**
         * Access top-right location.
         */
        constexpr Location top_right() const noexcept {
            return m_top_right;
        }

        /**
         * Access top-right location.
         */
        Location& top_right() noexcept {
            return m_top_right;
        }

        /**
         * Check whether the location is inside the box.
         *
         * @pre Location must be defined.
         * @pre Box must be defined.
         */
        bool contains(const osmium::Location& location) const noexcept {
            assert(bottom_left());
            assert(top_right());
            assert(location);
            return location.x() >= bottom_left().x() && location.y() >= bottom_left().y() &&
                   location.x() <= top_right().x() && location.y() <= top_right().y();
        }

        /**
         * Calculate size of the box in square degrees.
         *
         * Note that this measure isn't very useful if you want to know the
         * real-world size of the bounding box!
         *
         * @throws osmium::invalid_location unless all coordinates are valid.
         */
        double size() const {
            return (m_top_right.lon() - m_bottom_left.lon()) *
                   (m_top_right.lat() - m_bottom_left.lat());
        }

    }; // class Box

    /**
     * Boxes are equal if both locations are equal. Undefined boxes will
     * compare equal.
     */
    inline constexpr bool operator==(const Box& lhs, const Box& rhs) noexcept {
        return lhs.bottom_left() == rhs.bottom_left() &&
               lhs.top_right() == rhs.top_right();
    }

    /**
     * Output a box to a stream. The format is "(LON, LAT, LON, LAT)" or
     * "(undefined)" if the box is undefined.
     *
     * @returns Reference to basic_ostream given as first parameter.
     */
    template <typename TChar, typename TTraits>
    inline std::basic_ostream<TChar, TTraits>& operator<<(std::basic_ostream<TChar, TTraits>& out, const osmium::Box& box) {
        if (box) {
            out << '(';
            box.bottom_left().as_string_without_check(std::ostream_iterator<char>(out));
            out << ',';
            box.top_right().as_string_without_check(std::ostream_iterator<char>(out));
            out << ')';
        } else {
            out << "(undefined)";
        }
        return out;
    }

} // namespace osmium

#endif // OSMIUM_OSM_BOX_HPP