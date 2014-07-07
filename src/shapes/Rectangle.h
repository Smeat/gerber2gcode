/*
 * Rectangle.h
 *
 *  Created on: 07.07.2014
 *      Author: kevin
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "Shape.h"
#include "Cords.h"

class Rectangle : public Shape {
private:
	Cords _a, _b; //point a and b


public:
	Rectangle();
	virtual ~Rectangle();
};


typedef boost::shared_ptr<Rectangle> Rectangle_ptr;

#endif /* RECTANGLE_H_ */
