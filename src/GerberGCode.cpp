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
	if(!_dawingOn){
		_gcodestr << "G1 Z" << _drawingHeight << " F" << _ZFeedrate << " \n";
		_dawingOn = true;
	}
}

void GerberGCode::disableDrawing() {
	if(_dawingOn){
		_gcodestr << "G1 Z" << _freemoveHeight << " F" << _ZFeedrate << "\n";
		_dawingOn = false;
	}
}

void GerberGCode::addLine(Cords& start, Cords& end) {
}

void GerberGCode::addLine(Cords& start, Cords& end, bool multiline) {
}

void GerberGCode::addLine(Cords& end, bool multiline) {
	//addLine(NULL, end, multiline); //TODO: null reference? or use pointer
}

void GerberGCode::goTo(Cords& p) {
}

void GerberGCode::exposePoint(Cords& p) {
}

void GerberGCode::createRec(Cords& p) {
}

void GerberGCode::createCircle(Cords& p) {
}

std::string GerberGCode::getGCode() {
	return _gcodestr.str();
}
