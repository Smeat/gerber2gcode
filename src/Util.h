/**
 *  This file is part of gerber2gcode.
 * 
 *  gerber2gcode is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  gerber2gcode is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gerber2gcode.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <Cords.h>

#include <boost/tokenizer.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>

typedef boost::geometry::model::polygon<Cords> Polygon;
typedef boost::geometry::model::multi_polygon<Polygon> MultiPolygon;

namespace util{

typedef boost::tokenizer<boost::char_separator<char> >
   tokenizer;

void split(std::vector<std::string>* elems, const std::string& s, const char* delim);

void writeFile(const std::string& file, const std::string& data);

Cords getCirclePos(Cords mid, double radius, double gegree);

//Only returns first polygon
Polygon mergePolygon(Polygon poly1, Polygon poly2);

///Obsolete already in boost::geometry boost::geometry::correct
/// @deprecated
void makeClockwise(Polygon* poly);
bool isClockwise(const Polygon& poly);

Polygon copyPolygon(const Polygon& poly);
MultiPolygon copyMultiPolygon(const MultiPolygon& poly);



}

std::ostream& operator<<(std::ostream& os, const Polygon& c);
std::ostream& operator<<(std::ostream& os, const MultiPolygon& c);

#endif
