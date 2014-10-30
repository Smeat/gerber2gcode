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

#ifndef LINE_H_
#define LINE_H_

#include "Cords.h"
#include "Shape.h"

#include <cmath>

#include <boost/shared_ptr.hpp>

class Line : public Shape{
private:
	Cords _start, _end;
	double _width;

	
public:
	Line(const Cords& start, const Cords& end, double width);
	Line(Cords* start, Cords* end, double width);
	Line();
	virtual ~Line();
	
	Cords getStart() const {
		return _start;
	}

	Cords getEnd() const {
		return _end;
	}

	Cords* getStart_ptr(){
		return &_start;
	}

	Cords* getEnd_ptr(){
		return &_end;
	}

	double getWidth() const {
		return _width;
	}

	//Steigung der Geraden
	double getSlope() const {
		double a = _end.getY() - _start.getY();
		double b = _end.getX() - _start.getX();

		if(b == 0){
			return INFINITY;
		}
		return a / b;
	}

	double getSlopeAngle() const {
		double slope = getSlope();
		if(slope == INFINITY){
			return M_PI_2;
		}
		return std::atan(getSlope());
	}

	double getSlopeAngleDegree() const {
		return getSlopeAngle() * 180 / M_PI;
	}

	/**
	 *  Swap start and end Position. Shouldn't affect Result.
	 */
	void swapCords(){
		Cords temp = _start;
		_start = _end;
		_end = temp;
	}
};

typedef boost::shared_ptr<Line> Line_ptr;

#endif
