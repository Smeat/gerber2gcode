/*
 * Circle.cpp
 *
 *  Created on: 27.06.2013
 *      Author: smeat
 */

#include "Circle.h"

Circle::Circle() {
	_radius = 0;

}

Circle::Circle(Cords& mid, double radius){
	_mid = mid;
	_radius = radius;
}

Circle::Circle(Cords* mid, double radius){
	_mid = *mid;
	_radius = radius;
}

Circle::~Circle() {
	// TODO Auto-generated destructor stub
}

