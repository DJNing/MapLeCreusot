#include "projection.h"
#include <QPoint>

#define PI           3.14159265358979323846

constexpr double earth_radius_for_epsg3857 = 6378137.0;
constexpr double max_coordinate_epsg3857 = 20037508.34;


inline constexpr double deg_to_rad(double degree) noexcept {
            return degree * (PI / 180.0);
        }

constexpr inline double lon_to_x(double lon) noexcept {
                return earth_radius_for_epsg3857 * deg_to_rad(lon);
            }

            inline double lat_to_y_with_tan(double lat) { // not constexpr because math functions aren't
                return earth_radius_for_epsg3857 * std::log(std::tan(PI / 4 + deg_to_rad(lat) / 2));
            }

inline double lat_to_y(double lat) { // not constexpr because math functions aren't
    if (lat < -78.0 || lat > 78.0) {
        return lat_to_y_with_tan(lat);
    }

    return earth_radius_for_epsg3857 *
        ((((((((((-3.1112583378460085319e-23  * lat +
                   2.0465852743943268009e-19) * lat +
                   6.4905282018672673884e-18) * lat +
                  -1.9685447939983315591e-14) * lat +
                  -2.2022588158115104182e-13) * lat +
                   5.1617537365509453239e-10) * lat +
                   2.5380136069803016519e-9)  * lat +
                  -5.1448323697228488745e-6)  * lat +
                  -9.4888671473357768301e-6)  * lat +
                   1.7453292518154191887e-2)  * lat)
        /
        ((((((((((-1.9741136066814230637e-22  * lat +
                  -1.258514031244679556e-20)  * lat +
                   4.8141483273572351796e-17) * lat +
                   8.6876090870176172185e-16) * lat +
                  -2.3298743439377541768e-12) * lat +
                  -1.9300094785736130185e-11) * lat +
                   4.3251609106864178231e-8)  * lat +
                   1.7301944508516974048e-7)  * lat +
                  -3.4554675198786337842e-4)  * lat +
                  -5.4367203601085991108e-4)  * lat + 1.0);
}

QPointF projection(double lon, double lat)
{
    return QPointF(lon_to_x(lon), -lat_to_y(lat));  // reverse the y position to make north in the upper side
}

QPointF projection(osmium::Location loc)
{
    return QPointF(lon_to_x(loc.lon()), -lat_to_y(loc.lat()));
}
