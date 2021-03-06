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

#include <boost/tokenizer.hpp>

namespace util{

typedef boost::tokenizer<boost::char_separator<char> >
   tokenizer;

void split(std::vector<std::string>* elems, const std::string& s, const char* delim);

void writeFile(const std::string& file, const std::string& data);


}

#endif
