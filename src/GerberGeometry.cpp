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

Aperture::Aperture(int num, float width, char type, bool inInch){
	this->_num = num;
	this->_width = width;
	this->_height = width;
	this->_type = type;

	if(inInch)
	{
		this->_width = this->_width*25.4f;
	}
}

Aperture::Aperture(int num, float width, float height, char type, bool inInch){

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
	_XYFeedrate = 1000;
	_ZFeedrate = 70;
	_moveFeedrate = 1500;

	_drawingHeight = 1.8f;
	_freemoveHeight = 3.8f;//1.7f;
	// TODO Auto-generated constructor stub

}

GerberGeometry::~GerberGeometry() {
	deleteVector(&_apertures);
	deleteVector(&_lines);
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
			break;
		}
	}
}

void GerberGeometry::addCircleAperture(int apertureNum, float width) {
	_apertures.push_back(new Aperture(apertureNum, width, 'C', _inInch));
}

void GerberGeometry::addRectangleAperture(int apertureNum, float width,
		float height) {
	_apertures.push_back(new Aperture(apertureNum, width, height, 'R', _inInch));
}

void GerberGeometry::enableDrawing() {
	if(!_drawingOn){
		_gcodestr << "G1 Z" << _drawingHeight << " F" << _ZFeedrate << " \n";
		_drawingOn = true;
	}
}

void GerberGeometry::disableDrawing() {
	if(_drawingOn){
		_gcodestr << "G1 Z" << _freemoveHeight << " F" << _ZFeedrate << "\n";
		_drawingOn = false;
	}
}

void GerberGeometry::addLine(Cords* start, Cords* end, bool multiline) {

	Cords vec;

	_gcodestr << ";Drawing a Line\n";

	if(start == NULL)
	{
		start = &_lastCords; //FIXME ?
	}

	int numberOfLines = _curAperture->getWidth()/_penWidth + 0.5;
	if(numberOfLines < 1 || multiline == false) numberOfLines = 1;

	//System.out.println("Drawing "+numberOfLines);

	if(numberOfLines > 1)
	{
		float length, factor;
		vec = Cords((end->getY()-start->getY())*(-1), end->getX()-start->getX(), false);
		
		
		length = vec.getLength();
		factor = _penWidth/length;
		
		//System.out.println(" VEC X:"+vec.x+" Y: "+vec.y+" ("+vec.length()+")");
		vec *= factor;
		

		// Cords offCord = vec.clone().mul(Math.round(numberOfLines/2f));
		
		Cords offCord = vec * ((1.0f/2.0f)*(numberOfLines/2.0f));
		
		//System.out.println(" Off X:"+offCord.x+" Y: "+offCord.y+" ("+offCord.length()+")");
		
		*start -= offCord;
		*end -= offCord;		
	}

	enableDrawing();

	for(int curLine=1; curLine <= numberOfLines; curLine++)
	{
		
		/*if(lastCords.distance(end) < lastCords.distance(start))
		{
			Cords tmp = end;
			end = start;
			start = tmp;
			disableDrawing();
		}*/
		
		
		if(_lastCords != (*start))
		{
			disableDrawing();			
			_gcodestr << "G1 X" << start->getX() << " Y" << start->getY() << " F" << _XYFeedrate <<"\n";					
		}
		
		enableDrawing();				
		_gcodestr << "G1 X" << end->getX() << " Y" << end->getY() << "  F" << _XYFeedrate << "\n";
		_lastCords = Cords(end->getX(), end->getY(), false);
		_lines.push_back(new Line(start, end));
		
		//disableDrawing();
		
		*start += vec;
		*end += vec;
	}			


		
	//start.x += penWidth;
	//start.y += penWidth;
	//end.x += penWidth;
	//end.y += penWidth;
		
		

}

void GerberGeometry::addLine(Cords* end, bool multiline) {
	addLine(NULL, end, multiline);
}

void GerberGeometry::goTo(Cords* p) {
}

void GerberGeometry::exposePoint(Cords* p) {
}

void GerberGeometry::createRec(Cords* p) {
}

void GerberGeometry::createCircle(Cords* p) {
}

std::string GerberGeometry::getGCode() {
	return _gcodestr.str();
}
