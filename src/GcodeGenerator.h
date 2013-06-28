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

#ifndef GCODEGENERATOR_H_
#define GCODEGENERATOR_H_

#include <sstream>

#include "Cords.h"
#include "Line.h"
#include "Util.h"
#include "Circle.h"

struct Statistics{
	uint64_t lines;
	uint64_t circles;
	uint64_t moves;
	uint64_t zMoves;
	uint64_t distance;

	Statistics(){
		circles = 0;
		lines = 0;
		moves = 0;
		zMoves = 0;
		distance = 0;
	}
};

class GcodeGenerator {
protected:
	bool _inInch;
	bool _absolute;
private:
	std::stringstream _gcodestr;
	bool _drawingOn;
	double _penWidth;
	Cords _lastPos;

	Statistics _stats;
	std::vector<Shape_ptr> _drawn;




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
	GcodeGenerator();
	GcodeGenerator(int xyfeedrade, int zfeedrate, int movefeedrate, double drawingheight,  double freemoveheight, double penWidth);
	virtual ~GcodeGenerator();


	std::string getGCode();
	void generateGcode(std::vector<Shape_ptr>* _shapes);

	bool writeData(const std::string& fileName);

private:
	/**
	 * Go to position without drawing
	 * @param p Coordinates
	 */
	void goTo(Cords* p);

	/**
	 * Move head low to enable drawing
	 */
	void enableDrawing();

	/**
	 * Move head high to disable drawing
	 */
	void disableDrawing();

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

	void draw(Shape_ptr);
	void drawLine(Line_ptr);
	void drawCircle(Circle_ptr);

	void G01(Cords start, Cords end);

	Shape_ptr getNearestShape(std::vector<Shape_ptr>* shapes, Cords point, bool deleteElement = true);
	double minDistanceTo(Shape_ptr shape, Cords point);
};

#endif /* GCODEGENERATOR_H_ */
