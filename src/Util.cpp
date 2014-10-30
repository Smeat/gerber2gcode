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


Cords util::getCirclePos(Cords mid, double radius, double gegree){
	double x = mid.getX() + radius * cos(gegree * M_PI /180.0);
	double y = mid.getY() + radius * sin(gegree * M_PI /180.0);

	return Cords(x,y);
}

Polygon util::mergePolygon(Polygon poly1, Polygon poly2){
	std::vector< Polygon> output;
	Polygon outPoly = poly1;
	boost::geometry::union_(poly1, poly2, output);

	if(output.size() > 0){
		outPoly = output[0];
	}

	//std::cout << "Merged with " << output.size() << " results!" << std::endl;

	return outPoly;
}

///Only outer for now
void util::makeClockwise(Polygon* poly){
	if(isClockwise(*poly)){
		return;
	}

	Polygon poly2 = *poly;
	poly->clear();


	for(auto iter = poly2.outer().rbegin(); iter != poly2.outer().rend(); ++iter){
		poly->outer().push_back(*iter);
	}

}

bool util::isClockwise(const Polygon& poly){

	auto& outer = poly.outer();

	double sum = 0;
	// (x2 - x1) * (y2 + y1) + ...
	for(auto iter = outer.begin(); iter != outer.end(); ++iter){

		auto nextIter = iter +1;

		//next element is the first aka this is the last
		if(nextIter == outer.end()){
			nextIter = outer.begin();
		}



		sum += (nextIter->getX() - iter->getX() ) * ( nextIter->getY() + iter->getY());

	}

	return sum >=0;
}


Polygon util::copyPolygon(const Polygon& poly){
	std::stringstream ss;

	ss << poly;

	Polygon ret;

	boost::geometry::read_wkt(ss.str(), ret);

	return ret;

}

MultiPolygon util::copyMultiPolygon(const MultiPolygon& poly){
	std::stringstream ss;

	ss << poly;

	MultiPolygon ret;

	boost::geometry::read_wkt(ss.str(), ret);

	return ret;
}



std::ostream& operator<<(std::ostream& os, const Polygon& c){

	os << "POLYGON (( ";

	for(auto iter = c.outer().begin(); iter != c.outer().end(); ++iter){
		os << iter->getX() << " " << iter->getY();
		if((iter + 1) != c.outer().end()){
			os << " , ";
		}
	}
	os << " ))";


	return os;
}

std::ostream& operator<<(std::ostream& os, const MultiPolygon& c){

	os << "MULTIPOLYGON ( ";

	for(auto polyIter = c.begin(); polyIter != c.end(); ++polyIter){

		os << "((";

		for(auto iter = polyIter->outer().begin(); iter != polyIter->outer().end(); ++iter){
			os << iter->getX() << " " << iter->getY();
			if((iter + 1) != polyIter->outer().end()){
				os << " , ";
			}
		}
		os << " ))";


	}

	os << ")";

	return os;
}

