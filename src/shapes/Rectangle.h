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
#include "Line.h"
#include "Util.h"
#include "Circle.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>

//TODO: Rename to polygon

class Rectangle : public Shape{
public:

	boost::geometry::model::polygon<Cords> _polygon;

	Cords _a, _b, _c, _d; //point a and b

	void createRect(const Line& line);
	void addCircleAtEnds(const Line& line);
	void addCircle(const Cords& mid, double radius);


public:
	Rectangle();
	Rectangle(const Line& line);
	Rectangle(const Line_ptr line);
	Rectangle(const Circle_ptr line);
	Rectangle(const Circle& line);
	Rectangle(Cords const& a, Cords const& b);
	virtual ~Rectangle();

	Cords const& getA() const{
		return _a;
	}
	Cords const& getB() const{
		return _b;
	}
	Cords const& getC() const{
		return _c;
	}
	Cords const& getD() const{
		return _d;
	}

    inline Cords& getA() { return _b; }
    inline Cords& getB() { return _a; }
    inline Cords& getC() { return _c; }
    inline Cords& getD() { return _d; }

	boost::geometry::model::polygon<Cords> getPolygon() const{
		return _polygon;
	}
};

typedef boost::shared_ptr<Rectangle> Rectangle_ptr;

#endif /* RECTANGLE_H_ */
