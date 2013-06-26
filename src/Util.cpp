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

#include "Util.h"



void util::split(std::vector<std::string>* elems, const std::string& s, const char* delim) {
	boost::char_separator<char> sep(delim);

	 tokenizer tokens(s, sep);

	 for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter){
		 std::string token = *tok_iter;
		 elems->push_back(token);
	 }
}

void util::writeFile(const std::string& file, const std::string& data) {
	std::ofstream ostream(file, std::ios::out);

	ostream << data;

	ostream.close();
}
