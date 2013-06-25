/*
 * Cords.cpp
 *
 *  Created on: 25.06.2013
 *      Author: smeat
 */

#include "Cords.h"

Cords::Cords(float x, float y, bool inch) : _x(x), _y(y), _inInch(inch) {
	if(inch){
		inchToMM();
	}
}

Cords::~Cords() {
	// TODO Auto-generated destructor stub
}

void Cords::inchToMM(){
	if(_inInch){
		_x = _x*25.4f;
		_y = _y*25.4f;

		_inInch=false;
	}
}
float Cords::getLength() const{
	return (float) sqrt(pow(_x, 2)+pow(_y, 2));
}

float Cords::getDistance(const Cords& ob) const{
	Cords b = ob;
	b -= *this;

	return b.getLength();
}
