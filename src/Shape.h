/*
 * Shape.h
 *
 *  Created on: 27.06.2013
 *      Author: smeat
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include <boost/shared_ptr.hpp>

class Shape{
public:
	Shape(){}
	virtual ~Shape(){}
};

typedef boost::shared_ptr<Shape> Shape_ptr;


#endif /* SHAPE_H_ */
