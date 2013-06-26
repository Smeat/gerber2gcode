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
#include "Line.h"
#include "Util.h"

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
};

class GerberGeometry {

private:
	std::stringstream _gcodestr;
	bool _drawingOn;
	std::vector<Aperture*> _apertures;
	std::vector<Line_ptr> _lines;
	double _penWidth;
	Aperture* _curAperture;
	bool _inInch;
	Cords _lastCords;
	
	/**
	 * @brief Feedrate for drawing X and Y moves (mm/min)
	 */
	int _XYFeedrate;
	
	/**
	 * Feedrate for Z movement (mm/min)
	 */
	int _ZFeedrate;
	
	/**
	 * Feedrate for non drawing X and Y moves (mm/min)
	 */
	int _moveFeedrate;

	/**
	 * Height where pen can draw
	 */
	double _drawingHeight;
	
	/**
	 * Height where pan cannot draw
	 */
	double _freemoveHeight;//1.7f;
public:
	GerberGeometry();
	virtual ~GerberGeometry();

	/**
	 * Enable absolute movement
	 * @todo Move to extra class
	 */
	void enableAbsolute();
	
	/**
	 * Enable relative movement
	 * @todo Move to extra class
	 */
	void enableRelative();
	
	/**
	 * Set units to metric 
	 */
	void setMetric();
	
	/**
	 * Set units to imperial
	 */
	void setImperial();
	
	/**
	 * Select a specific aperture from the internal list
	 */
	void selectAperture(int aperture);
	void addCircleAperture(int apertureNum, double width);
	void addRectangleAperture(int apertureNum, double width, double height);
	
	/**
	 * Move head low to enable drawing
	 */
	void enableDrawing();
	
	/**
	 * Move head high to disable drawing
	 */
	void disableDrawing();
	void addLine(Cords* end, bool multiline = true);
	void addLine(Cords* start, Cords* end, bool multiline = true);
	
	/**
	 * Go to position without drawing
	 * @param p Coordinates
	 */
	void goTo(Cords* p);
	void exposePoint(Cords* p);
	void createRec(Cords* p);
	void createCircle(Cords* p);
	std::string getGCode();
};

#endif /* GerberGeometry_H_ */
