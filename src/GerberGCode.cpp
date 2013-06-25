/*
 * GerberGCode.cpp
 *
 *  Created on: 25.06.2013
 *      Author: smeat
 */

#include "GerberGCode.h"

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

GerberGCode::GerberGCode() {
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

GerberGCode::~GerberGCode() {
	// TODO Auto-generated destructor stub
}

void GerberGCode::enableAbsolute() {
	_gcodestr << "G90\n";
}

void GerberGCode::enableRelative() {
	_gcodestr << "G91\n";
}

void GerberGCode::setMetric() {
	_gcodestr << "G21\n";
}

void GerberGCode::setImperial() {
	_inInch=true;
	setMetric();
}

void GerberGCode::selectAperture(int aperture) {
	for(auto iter = _apertures.begin(); iter != _apertures.end(); ++iter){
		if(iter->getNum() == aperture){
			_curAperture = &(*iter); //I HATE THAT! Is there a better way?
			break;
		}
	}
}

void GerberGCode::addCircleAperture(int apertureNum, float width) {
	_apertures.push_back(Aperture(apertureNum, width, 'C', _inInch));
}

void GerberGCode::addRectangleAperture(int apertureNum, float width,
		float height) {
	_apertures.push_back(Aperture(apertureNum, width, height, 'R', _inInch));
}

void GerberGCode::enableDrawing() {
	if(!_drawingOn){
		_gcodestr << "G1 Z" << _drawingHeight << " F" << _ZFeedrate << " \n";
		_drawingOn = true;
	}
}

void GerberGCode::disableDrawing() {
	if(_drawingOn){
		_gcodestr << "G1 Z" << _freemoveHeight << " F" << _ZFeedrate << "\n";
		_drawingOn = false;
	}
}

void GerberGCode::addLine(Cords* start, Cords* end, bool multiline) {

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
		
		//disableDrawing();
		
		*start += vec;
		*end += vec;
	}			


		
	//start.x += penWidth;
	//start.y += penWidth;
	//end.x += penWidth;
	//end.y += penWidth;
		
		

}

void GerberGCode::addLine(Cords* end, bool multiline) {
	addLine(NULL, end, multiline);
}

void GerberGCode::goTo(Cords* p) {
}

void GerberGCode::exposePoint(Cords* p) {
}

void GerberGCode::createRec(Cords* p) {
}

void GerberGCode::createCircle(Cords* p) {
}

std::string GerberGCode::getGCode() {
	return _gcodestr.str();
}
