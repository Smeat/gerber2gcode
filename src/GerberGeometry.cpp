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
	_drawingOn = true;
	_curAperture = NULL;
	_penWidth = 0.5;
	_inInch = false;
	_XYFeedrate = 60 * 60;
	_ZFeedrate = 100;
	_moveFeedrate = 130 *60;

	_drawingHeight = 1.8f;
	_freemoveHeight = 2.8f;//1.7f;
	// TODO Auto-generated constructor stub

	_gcodestr.precision(6);
	enableAbsolute();

}

GerberGeometry::~GerberGeometry() {

}

void GerberGeometry::enableAbsolute() {
	_gcodestr << "G90\n";
}

void GerberGeometry::enableRelative() {
	_gcodestr << "G91\n";
}

void GerberGeometry::setMetric() {
	_gcodestr << "G21\n";
}

void GerberGeometry::setImperial() {
	_inInch=true;
	setMetric();
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

void GerberGeometry::enableDrawing() {
	if(!_drawingOn){
		_gcodestr << ";Enable drawing\n";
		_gcodestr << "G1 Z" << _drawingHeight << " F" << _ZFeedrate << " \n";
		_drawingOn = true;
	}
}

void GerberGeometry::disableDrawing() {
	if(_drawingOn){
		_gcodestr << ";Disable drawing\n";
		_gcodestr << "G1 Z" << _freemoveHeight << " F" << _ZFeedrate << "\n";
		_drawingOn = false;
	}
}

void GerberGeometry::addLine(Cords* start, Cords* end, bool multiline) {

	Cords vec;
	bool delStart = false;

	_gcodestr << ";Drawing a Line\n";

	if(start == 0)
	{
		delStart = true;
		start = new Cords(_lastCords); //FIXME: dirty workaround
	}

	int numberOfLines = _curAperture->getWidth()/_penWidth + 0.5;
	//std::cout << _curAperture->getWidth() << " / " << _penWidth << " == " << numberOfLines << std::endl;
	if(numberOfLines < 1 || multiline == false) numberOfLines = 1;
		//std::cout << "Drawing "<<numberOfLines<< std::endl;

	if(numberOfLines > 1)
	{
		double length, factor;
		vec = Cords((end->getY()-start->getY())*(-1), end->getX()-start->getX(), false);
		
		
		length = vec.getLength();
		factor = _penWidth/length;
		
		//std::cout << " VEC X:"<<vec.getX()<<" Y: "<<vec.getY()<<" ("<<vec.getLength()<<")" << std::endl;
		vec *= factor;
		

		// Cords offCord = vec.clone().mul(Math.round(numberOfLines/2f));
		
		Cords offCord = vec * ((1.0f/2.0f)*(numberOfLines/2.0f));
		
		//std::cout << " Off X:" <<offCord.getX()<<" Y: "<< offCord.getY() << " (" <<offCord.getLength() << ")" << std::endl;
		
		*start -= offCord;
		*end -= offCord;		
	}

	enableDrawing();

	for(int curLine=1; curLine <= numberOfLines; curLine++)
	{
		//std::cout << "Drawing Line " << curLine << std::endl;
		
		/*if(lastCords.distance(end) < lastCords.distance(start))
		{
			Cords tmp = end;
			end = start;
			start = tmp;
			disableDrawing();
		}*/
		
	//	std::cout << "Last: (" << _lastCords.getX() << ", " << _lastCords.getY() << ") Start: (" << start->getX() << ", " << start->getY() << ")" << std::endl;
	//	std::cout << "Last != start? " << (_lastCords != (*start)) << std::endl;
		if(_lastCords != (*start))
		{
			//disableDrawing();
			goTo(start);
			//_gcodestr << "G1 X" << start->getX() << " Y" << start->getY() << " F" << _XYFeedrate <<"\n";
		}
		
		enableDrawing();				
		_gcodestr << "G1 X" << end->getX() << " Y" << end->getY() << "  F" << _XYFeedrate << "\n";
	//	std::cout << "G1 X" << end->getX() << " Y" << end->getY() << "  F" << _XYFeedrate << std::endl;
		_lastCords = Cords(end->getX(), end->getY(), false);
		_lines.push_back(Line_ptr(new Line(start, end)));
		
		//disableDrawing();
		
		*start += vec;
		*end += vec;
	}			


		
	//start.x += penWidth;
	//start.y += penWidth;
	//end.x += penWidth;
	//end.y += penWidth;
		

	if(delStart)
		delete start;
		

}

void GerberGeometry::addLine(Cords* end, bool multiline) {
	addLine(0, end, multiline);
}

void GerberGeometry::goTo(Cords* p) {
	if(p != 0){
		if(_lastCords == *p){
			_drawingOn = false; //FIXME: workardound
			 return;
		 }
	}
	_gcodestr << ";GoTo\n";

	disableDrawing();
	_gcodestr << "G1 X" << p->getX() << " Y" << p->getY() << " F" << _moveFeedrate << "\n";

	_lastCords = Cords(p->getX(), p->getY(), false);
}

void GerberGeometry::exposePoint(Cords* p) {
	createRec(p);
}

void GerberGeometry::createRec(Cords* p) {
	double startX, startY, endX, endY;

		_gcodestr << ";Creating Rect\n";

		double recWidth = _curAperture->getWidth()/2.0f;
		double recHeight = _curAperture->getHeight()/2.0f;

		startX = p->getX()-recWidth;
		startY = p->getY()-recHeight;
		endX = p->getX()+recWidth;
		endY = p->getY()-recHeight;
		//addLine(new Cords(startX, startY, false));
		goTo(new Cords(startX, startY, false));
		enableDrawing();
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




		disableDrawing();
		_gcodestr << ";End Rect\n";

	}

void GerberGeometry::createCircle(Cords* p) {
	double y;//,x
	//FIXME
	for(double i=0; i < _curAperture->getWidth(); i+=0.3)
	{

		y = (double) (p->getY() + std::sqrt( std::pow(_curAperture->getWidth(), 2)  - std::pow(i,2) ));
		addLine(0, new Cords(p->getX()+i, y, false));
	}

	for(double i=0; i < _curAperture->getWidth(); i+=0.3)
	{

		y = (double) (p->getY() - std::sqrt( std::pow(_curAperture->getWidth(), 2)  - std::pow(i,2) ));
		addLine(0, new Cords(p->getX()+i, y, false));
	}
}

std::string GerberGeometry::getGCode() {
	disableDrawing();
	return _gcodestr.str();
}
