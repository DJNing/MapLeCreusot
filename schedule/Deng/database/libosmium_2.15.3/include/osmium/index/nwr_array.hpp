#ifndef OSMIUM_INDEX_NWR_ARRAY_HPP
#define OSMIUM_INDEX_NWR_ARRAY_HPP

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

#include <osmium/osm/item_type.hpp>

#include <array>

namespace osmium {

    /**
     * Often some information must be kept separately for nodes, ways, and
     * relations. Then this helper class becomes useful. It can keep three
     * somethings (of type T) which can be accessed conveniently through
     * the call operator.
     */
    template <typename T>
    class nwr_array {

        std::array<T, 3> m_data;

    public:

        nwr_array() :
            m_data() {
        }

        T& operator()(const osmium::item_type type) noexcept {
            return m_data[osmium::item_type_to_nwr_index(type)];
        }

        const T& operator()(const osmium::item_type type) const noexcept {
            return m_data[osmium::item_type_to_nwr_index(type)];
        }

    }; // class nwr_array

} // namespace osmium

#endif // OSMIUM_INDEX_NWR_ARRAY_HPP