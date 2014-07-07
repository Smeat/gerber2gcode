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

#ifndef GerberGeometry_H_
#define GerberGeometry_H_

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cmath>

#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

#include "Cords.h"
#include "shapes/Line.h"
#include "Util.h"
#include "Geometry.h"
#include "shapes/Circle.h"
#include "shapes/Shape.h"

class Aperture{
private:
	int _num;
	double _width, _height;
	char _type;

public:
	Aperture(int num, double width, char type, bool inInch);

	Aperture(int num, double width, double height, char type, bool inInch);

	virtual ~Aperture();


	inline int getNum(){
		return _num;
	}
	
	inline double getWidth(){
		return _width;
	}

	inline double getHeight(){
		return _height;
	}

	inline char getType(){
		return _type;
	}
};

class GerberGeometry : public Geometry{

private:
	std::vector<Aperture*> _apertures;
	Aperture* _curAperture;
	Cords _lastCords;
	
public:
	GerberGeometry();
	virtual ~GerberGeometry();

	/**
	 * Select a specific aperture from the internal list
	 */
	void selectAperture(int aperture);
	void addCircleAperture(int apertureNum, double width);
	void addRectangleAperture(int apertureNum, double width, double height);
	
	

	void addLine(Cords* end, bool multiline = true);
	void addLine(Cords* start, Cords* end, bool multiline = true);
	
	void goTo(Cords* p);


	void exposePoint(Cords* p);
	void createRec(Cords* p);
	void createCircle(Cords* p);
};

#endif /* GerberGeometry_H_ */
