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

GerberGCode::GerberGCode() {
	// TODO Auto-generated constructor stub

}

GerberGCode::~GerberGCode() {
	// TODO Auto-generated destructor stub
}

