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

#ifndef GERBERLOADER_H_
#define GERBERLOADER_H_

#include <fstream>
#include <sstream>
#include <iostream>

#include <cstdlib>

#include "GerberGeometry.h"
#include "FileLoader.h"

#include "Util.h"

#include <boost/algorithm/string/predicate.hpp>

class GerberLoader : public FileLoader {
private:
	GerberGeometry _geo;

	std::string _formatX;
	std::string _formatY;

	bool _inInch;

	double _offsetX, _offsetY;

public:
	GerberLoader();
	virtual ~GerberLoader();

	bool generateGeometry();
	std::vector<Shape_ptr>* getLines(){ return _geo.getLines();}
	std::vector< boost::geometry::model::polygon<Cords> >* getPolygons(){ return _geo.getPolygons();}

private:
	bool readLine(std::string* line);


};

#endif
