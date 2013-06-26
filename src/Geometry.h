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

#include "Line.h"

class Geometry{
protected:
	std::vector<Line_ptr> _lines;
	double _penWidth;
	bool _inInch;
	bool _absolute;

public:
	Geometry(){
		_penWidth = 0.5;
		_absolute = true;
		_inInch = false;
	}
	virtual ~Geometry(){}

	virtual void addLine(Cords* end, bool multiline = true) = 0;
	virtual void addLine(Cords* start, Cords* end, bool multiline = true) = 0;
	virtual void createRec(Cords* p) = 0;
	virtual void createCircle(Cords* p) = 0;
	virtual std::vector<Line_ptr>* getLines() { return &_lines;}

	void enableRelative(){ _absolute = false;}
	void enableAbsolute(){ _absolute = true; }
	void setImperial() {_inInch=true;}
	void setMetric() {_inInch = false;}
};


#endif /* GEOMETRY_H_ */
