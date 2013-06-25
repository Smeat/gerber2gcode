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
};

class GerberGCode {
private:
	std::stringstream _gcodestr;
	bool _dawingOn = true;
	std::vector<Aperture> _apertures;
	float _penWidth;
	Aperture* _curAperture = NULL;
	bool _inInch=false;
	Cords* _lastCords = NULL;

	int _XYFeedrate = 1000;
	int _ZFeedrate = 70;
	int _moveFeedrate = 1500;

	float _drawingHeight = 1.8f;
	float _freemoveHeight = 3.8f;//1.7f;
public:
	GerberGCode();
	virtual ~GerberGCode();

	void enableAbsolute();
	void enableRelative();
	void setMetric();
	void setImperial();
	void selectAperture(int aperture);
	void addCircleAperture(int apertureNum, float width);
	void addRectangleAperture(int apertureNum, float width, float height);
	void enableDrawing();
	void disableDrawing();
	void addLine(Cords& start, Cords& end);
	void addLine(Cords& end, bool multiline = true);
	void addLine(Cords& start, Cords& end, bool multiline);
	void goTo(Cords& p);
	void exposePoint(Cords& p);
	void createRec(Cords& p);
	void createCircle(Cords& p);
	std::string getGCode();
};

#endif /* GERBERGCODE_H_ */
