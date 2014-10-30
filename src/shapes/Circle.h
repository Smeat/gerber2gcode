/*
 * Circle.h
 *
 *  Created on: 27.06.2013
 *      Author: smeat
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "Shape.h"
#include "Cords.h"

#include <boost/shared_ptr.hpp>

class Circle : public Shape {
private:
	double _radius;
	Cords _mid;
public:
	Circle();
	Circle(Cords& _mid, double radius);
	Circle(Cords* _mid, double radius);
	virtual ~Circle();

	Cords getMid() const{
		return _mid;
	}

	Cords* getMid_ptr(){
		return &_mid;
	}

	double getRadius() const{
		return _radius;
	}


};

typedef boost::shared_ptr<Circle> Circle_ptr;

#endif /* CIRCLE_H_ */
