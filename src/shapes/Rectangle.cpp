/*
 * Rectangle.cpp
 *
 *  Created on: 07.07.2014
 *      Author: kevin
 */

#include "Rectangle.h"

Rectangle::Rectangle() {
	// TODO Auto-generated constructor stub

}

Rectangle::Rectangle(const Line& line){
	createRect(line);
}

Rectangle::Rectangle(const Line_ptr line){
	createRect(*line);
}

Rectangle::Rectangle(Cords const& a, Cords const& b){
	_a = a;
	_b = b;

	boost::geometry::model::box<Cords> box(a, b);

	boost::geometry::convert(box, _polygon);

	boost::geometry::correct(_polygon);

}
Rectangle::Rectangle(const Circle_ptr circle){
	addCircle(circle->getMid(), circle->getRadius());
}

Rectangle::Rectangle(const Circle& circle){
	addCircle(circle.getMid(), circle.getRadius());
}

Rectangle::~Rectangle() {
	// TODO Auto-generated destructor stub
}

void Rectangle::createRect(const Line& line){
	Cords start = line.getStart();
	Cords end = line.getEnd();

	double width = line.getWidth();

	double angle = line.getSlopeAngle();

	double xFactor = sin(angle);
	double yFactor = cos(angle);




	_a = Cords(start.getX() + xFactor * width/2.0, start.getY() - yFactor * width/2.0);
	_b = Cords(end.getX() + xFactor * width/2.0, end.getY() - yFactor * width/2.0);
	_c = Cords(end.getX() - xFactor * width/2.0, end.getY() + yFactor * width/2.0);
	_d = Cords(start.getX() - xFactor * width/2.0, start.getY() + yFactor * width/2.0);

	if((start.getDistance(_a) - width/2.0) > 0.001){
		fprintf(stderr,"-------ERROR!! Width should be %f but is %f\n", width/2.0, start.getDistance(_a));
		printf("XFactor %f yFactor %f\n", xFactor, yFactor);
		printf("angle: %f\n", angle);


	}

	/*//If sum is <0 a-d are counterclockwise
	double direction = (_b.getX() - _a.getX()) * (_b.getY() + _a.getY()) + (_c.getX() - _b.getX()) * (_c.getY() + _b.getY()) + (_d.getX() - _c.getX()) * (_d.getY() + _c.getY()) + (_a.getX() - _d.getX()) * (_a.getY() + _d.getY());

	if(direction < 0){
		_polygon.outer().push_back(_d);
		_polygon.outer().push_back(_c);
		_polygon.outer().push_back(_b);
		_polygon.outer().push_back(_a);
		_polygon.outer().push_back(_d);
	}
	else {
		_polygon.outer().push_back(_a);
		_polygon.outer().push_back(_b);
		_polygon.outer().push_back(_c);
		_polygon.outer().push_back(_d);
		_polygon.outer().push_back(_a);
	}*/

	_polygon.outer().push_back(_a);
	_polygon.outer().push_back(_b);
	_polygon.outer().push_back(_c);
	_polygon.outer().push_back(_d);
	_polygon.outer().push_back(_a);

	boost::geometry::correct(_polygon);

//	std::cout << "Coords: " << _a << _b << _c << _d << std::endl;


//	printf("XFactor %f yFactor %f\n", xFactor, yFactor);
	//_a = Cords(start.getX() + xFactor * width/2.0, start.getY() - yFactor * width/2.0);
	//_b = Cords(end.getX() - xFactor * width/2.0, end.getY() + yFactor * width/2.0);

//	printf("Created rectangle [ ( %f, %f), ( %f, %f) ] from Line with start ( %f, %f) end ( %f, %f) and width %f\n", _a.getX(), _a.getY(), _b.getX(), _b.getY(), line.getStart().getX(), line.getStart().getY(), line.getEnd().getX(), line.getEnd().getY(), line.getWidth());
}


void Rectangle::addCircleAtEnds(const Line& line){
	addCircle(line.getStart(), line.getWidth()/2);
	addCircle(line.getEnd(), line.getWidth()/2);

}

void Rectangle::addCircle(const Cords& mid, double radius){

	Cords bottom = util::getCirclePos(mid, radius, 270); ///Go to the bottom of the circle

	Polygon poly;
	Cords first = util::getCirclePos(mid, radius, 360);

	for(double j = 360; j > 0; j-= 360/24){
		Cords pos = util::getCirclePos(mid, radius, j);
		poly.outer().push_back(pos);
	}

	poly.outer().push_back(first);

	//std::cout << "Merging circle: " << poly << " With " << _polygon <<  std::endl;

	_polygon = util::mergePolygon(_polygon, poly);

	//std::cout << "To " << _polygon << std::endl;

}

