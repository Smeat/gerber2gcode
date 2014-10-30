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

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "shapes/Line.h"
#include "Cords.h"
#include <vector>
#include "shapes/Shape.h"
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>

class Geometry{
protected:
	std::vector<Shape_ptr> _shapes;
	boost::geometry::model::multi_polygon< boost::geometry::model::polygon<Cords> > _polygons;
	std::vector<Linestring> _infill;
	//std::vector< boost::geometry::model::polygon<Cords> > _polygons;
	bool _inInch;
	bool _absolute;

public:
	Geometry(){
		_absolute = true;
		_inInch = false;
	}
	virtual ~Geometry(){}

	virtual void addLine(Cords_ptr end) = 0;
	virtual void addLine(Cords_ptr start, Cords_ptr end) = 0;
	virtual void createRec(Cords_ptr p) = 0;
	virtual void createCircle(Cords_ptr p) = 0;
	virtual std::vector<Shape_ptr>* getLines() { return &_shapes;}

	virtual std::vector< boost::geometry::model::polygon<Cords> >* getPolygons() { return &_polygons;}

	void enableRelative(){ _absolute = false;}
	void enableAbsolute(){ _absolute = true; }
	void setImperial() {_inInch=true;}
	void setMetric() {_inInch = false;}
};


#endif /* GEOMETRY_H_ */
