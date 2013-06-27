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

#include "GerberGeometry.h"

Aperture::Aperture(int num, double width, char type, bool inInch){
	this->_num = num;
	this->_width = width;
	this->_height = width;
	this->_type = type;

	if(inInch)
	{
		this->_width = this->_width*25.4f;
	}
}

Aperture::Aperture(int num, double width, double height, char type, bool inInch){

	this->_num = num;
	this->_width = width;
	this->_height = height;
	this->_type = type;

	if(inInch)
	{
		this->_width = this->_width*25.4f;
		this->_height = this->_height*25.4f;
	}
}

Aperture::~Aperture(){

}

GerberGeometry::GerberGeometry() {
	_curAperture = NULL;
	_inInch = false;

	enableAbsolute();

}

GerberGeometry::~GerberGeometry() {

}

void GerberGeometry::selectAperture(int aperture) {
	for(auto iter = _apertures.begin(); iter != _apertures.end(); ++iter){
		if((*iter)->getNum() == aperture){
			_curAperture = *iter;
			return;
		}
	}
	std::cerr << "Could not select aperture!!!!!!" << std::endl;
}

void GerberGeometry::addCircleAperture(int apertureNum, double width) {
	_apertures.push_back(new Aperture(apertureNum, width, 'C', _inInch));
}

void GerberGeometry::addRectangleAperture(int apertureNum, double width,
		double height) {
	_apertures.push_back(new Aperture(apertureNum, width, height, 'R', _inInch));
}

void GerberGeometry::addLine(Cords* start, Cords* end, bool multiline) {

	Cords vec;
	bool delStart = false;

	if(start == 0)
	{
		delStart = true;
		start = new Cords(_lastCords); //FIXME: dirty workaround
	}

	_shapes.push_back(Line_ptr(new Line(start, end, _curAperture->getWidth(), true)));

	if(delStart)
		delete start;
		

}

void GerberGeometry::addLine(Cords* end, bool multiline) {
	addLine(0, end, multiline);
}

void GerberGeometry::goTo(Cords* p) {
	if(p != 0){
		if(_lastCords == *p){
			 return;
		 }
	}
	_lastCords = Cords(p->getX(), p->getY(), false);
}

void GerberGeometry::exposePoint(Cords* p) {
	if(_curAperture->getType() == 'C')
		createCircle(p);
	else
		createRec(p);
}

void GerberGeometry::createRec(Cords* p) {
	double startX, startY, endX, endY;

	double recWidth = _curAperture->getWidth()/2.0f;
	double recHeight = _curAperture->getHeight()/2.0f;

	startX = p->getX()-recWidth;
	startY = p->getY()-recHeight;
	endX = p->getX()+recWidth;
	endY = p->getY()-recHeight;
	//addLine(new Cords(startX, startY, false));
	goTo(new Cords(startX, startY, false));
	//  ---->
	//  |   |
	addLine(new Cords(endX, endY, false), false);

	endX = p->getX()+recWidth;
	endY = p->getY()+recHeight;
	//   ____
	//  |    :
	addLine(new Cords(endX, endY, false), false);

	endX = p->getX()-recWidth;
	endY = p->getY()+recHeight;
	//  |    |
	//  <-----
	addLine(new Cords(endX, endY, false), false);

	endX = p->getX()-recWidth;
	endY = p->getY()-recHeight;
	//  ^     |
	//  :_____|
	addLine(new Cords(endX, endY, false), false);
}

void GerberGeometry::createCircle(Cords* p) {
		_shapes.push_back(Shape_ptr(new Circle(p, _curAperture->getWidth()/2)));

}
