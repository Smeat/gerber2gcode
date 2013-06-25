/*
 * GerberGCode.h
 *
 *  Created on: 25.06.2013
 *      Author: smeat
 */

#ifndef GERBERGCODE_H_
#define GERBERGCODE_H_

#include <string>
#include <vector>
#include <sstream>

#include "Cords.h"

class Aperture{
private:
	int _num;
	float _width, _height;
	char _type;

public:
	Aperture(int num, float width, char type, bool inInch);

	Aperture(int num, float width, float height, char type, bool inInch);

	virtual ~Aperture();


	int getNum(){
		return _num;
	}
	
	int getWidth(){
		return _width;
	}
};

class GerberGCode {
private:
	std::stringstream _gcodestr;
	bool _drawingOn;
	std::vector<Aperture> _apertures;
	float _penWidth;
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
	float _drawingHeight;
	
	/**
	 * Height where pan cannot draw
	 */
	float _freemoveHeight;//1.7f;
public:
	GerberGCode();
	virtual ~GerberGCode();

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
	void addCircleAperture(int apertureNum, float width);
	void addRectangleAperture(int apertureNum, float width, float height);
	
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

#endif /* GERBERGCODE_H_ */
