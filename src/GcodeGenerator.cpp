/**
 *  This file is part of gerber2gcode.
 *
 *  gerber2gcode is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  gerber2gcode is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gerber2gcode.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GcodeGenerator.h"

GcodeGenerator::GcodeGenerator() {
	_XYFeedrate = 60 * 60;
	_ZFeedrate = 100;
	_moveFeedrate = 130 *60;

	_drawingHeight = 1.8;
	_freemoveHeight = 3;//1.7f;
	// TODO Auto-generated constructor stub

	_gcodestr.precision(6);
	_penWidth = 0.5;

	enableAbsolute();

}

GcodeGenerator::GcodeGenerator(int xyfeedrade, int zfeedrate, int movefeedrate,
		double drawingheight, double freemoveheight, double penWidth) {
	_XYFeedrate = xyfeedrade;
	_ZFeedrate = zfeedrate;
	_moveFeedrate = movefeedrate;

	_drawingHeight = drawingheight;
	_freemoveHeight = freemoveheight;//1.7f;

	_gcodestr.precision(6);
	_penWidth = penWidth;

	enableAbsolute();
}

GcodeGenerator::~GcodeGenerator() {
	// TODO Auto-generated destructor stub
}

void GcodeGenerator::enableDrawing() {
	if(!_drawingOn){
		_gcodestr << ";Enable drawing\n";
		_gcodestr << "G1 Z" << _drawingHeight << " F" << _ZFeedrate << " \n";
		_drawingOn = true;

		++_stats.zMoves;
	}
}

void GcodeGenerator::disableDrawing() {
	if(_drawingOn){
		_gcodestr << ";Disable drawing\n";
		_gcodestr << "G1 Z" << _freemoveHeight << " F" << _ZFeedrate << "\n";
		_drawingOn = false;

		++_stats.zMoves;
	}
}

void GcodeGenerator::goTo(Cords* p) {
	if(p == 0 || _lastPos == *p){
		return;
	}
	_gcodestr << ";GoTo\n";

	if(_drawingOn && (_lastPos.getDistance(*p) <= _penWidth)) disableDrawing();
	_gcodestr << "G1 X" << p->getX() << " Y" << p->getY() << " F" << _moveFeedrate << "\n";
	_stats.distance += _lastPos.getDistance(*p);
	_lastPos = *p;
	enableDrawing();

	++_stats.moves;
}

void GcodeGenerator::enableAbsolute() {
	_gcodestr << "G90\n";
}

void GcodeGenerator::enableRelative() {
	_gcodestr << "G91\n";
}

void GcodeGenerator::setMetric() {
	_gcodestr << "G21\n";
}

void GcodeGenerator::setImperial() {
	_inInch=true;
	setMetric();
}

std::string GcodeGenerator::getGCode() {
	return _gcodestr.str();
}

void GcodeGenerator::generateGcode(std::vector<Shape_ptr>* shapes) {

	while (shapes->size() != 0){
		Shape_ptr drawing = getNearestShape(shapes, _lastPos);
		draw(drawing);
	}



/*
	for(auto iter = shapes->begin(); iter != shapes->end(); ++iter){
		draw(*iter);
		_drawn.push_back(*iter);
		shapes->erase(iter);
	}*/

	_gcodestr << ";------Stats-------\n;Lines Drawn: " << _stats.lines << "\n;Circles Drawn: " << _stats.circles << "\n;Moves: " << _stats.moves << "\n;zMoves: " << _stats.zMoves << "\n;Total distance: " << _stats.distance << "mm\n";
}

Shape_ptr GcodeGenerator::getNearestShape(std::vector<Shape_ptr>* shapes, Cords point, bool deleteElement) {
	auto nearest = shapes->end(); //just assign one
	double min = 1000000; //high number
	for(auto iter = shapes->begin(); iter != shapes->end(); ++iter){
		Shape_ptr curShape = *iter;
		double localMin = minDistanceTo(curShape, point);

		if(localMin < min){
			min = localMin;
			nearest = iter;
		}
	}
	Shape_ptr retp = *nearest;
	if(deleteElement && nearest != shapes->end()){
		shapes->erase(nearest);
	}

	return retp;

}

double GcodeGenerator::minDistanceTo(Shape_ptr shape, Cords point){
	double min = 0;
	if(Line_ptr line = boost::dynamic_pointer_cast<Line>(shape)){
		double min2 = 0;
		min = line->getStart().getDistance(point);
		min2 = line->getEnd().getDistance(point);

		if(min2 < min){
			min = min2;
			line->swapCords();
		}
	}
	else if(Circle_ptr circle = boost::dynamic_pointer_cast<Circle>(shape)){
		min = (circle->getMid() - Cords(0,circle->getRadius())).getDistance(point); //bottom
	}

	return min;
}
void GcodeGenerator::draw(Shape_ptr shape){
	if(Line_ptr line = boost::dynamic_pointer_cast<Line>(shape)){ //TODO: is this the optimal way? no such thing as typeof?

		drawLine(line);
	}
	else if(Circle_ptr circle = boost::dynamic_pointer_cast<Circle>(shape)){
		drawCircle(circle);
	}

}

void GcodeGenerator::drawLine(Line_ptr line) {
	Cords start = line->getStart();
	Cords end = line->getEnd();
	double width = line->getWidth();
	int numberOfLines = width / _penWidth + 0.5;

	drawCircle(Circle_ptr(new Circle(start, width/2)));
	drawCircle(Circle_ptr(new Circle(end, width/2)));

	//std::cout << "Number of lines: " << numberOfLines << std::endl;

	Cords vec;

	if(numberOfLines > 1){
		double length, factor;
		vec = Cords((end.getY()-start.getY())*(-1), (end.getX()-start.getX())*(1), false);


		length = vec.getLength();
		factor = _penWidth/length;

	//	std::cout << " VEC X:" << vec.getX() << " Y: " << vec.getY() << " (" << vec.getLength() <<")" << std::endl;

		vec *= factor;

		Cords offCord = vec * (numberOfLines/2.0);

		start -=offCord;
		end -=offCord;
	}

	for(int curLine=1; curLine <= numberOfLines; curLine++){
		if(start.getDistance(_lastPos) > end.getDistance(_lastPos)){
			G01(end, start);
		}
		else{
			G01(start, end);
		}


		start +=vec;
		end += vec;

	}


}

void GcodeGenerator::drawCircle(Circle_ptr circle) {
	Cords mid = circle->getMid();
	double radius = circle->getRadius();

	_gcodestr << ";Generating circle with mid (" << mid.getX() << ", " << mid.getY() << ") and r " << radius << std::endl;


	for(double i = 0; i < radius; i+=_penWidth){
		Cords bottom = mid - Cords(0,i); ///Go to the bottom of the cirlce only reduce y
		goTo(&bottom);
		_gcodestr << "G02 J" << i << " F" << _XYFeedrate << "\n"; ///Drawing a circle upwards
		_lastPos = bottom;

		++_stats.circles;
		_stats.distance += 2 * M_PI * i;

	}


}


bool GcodeGenerator::writeData(const std::string& fileName) {
	util::writeFile(fileName, getGCode()); //TODO: move to base class
	return true;
}

void GcodeGenerator::G01(Cords start, Cords end) {
	goTo(&start);
	_gcodestr << "G01 X" << end.getX() << " Y" << end.getY() << " F" << _XYFeedrate << '\n';

	_stats.distance += _lastPos.getDistance(end);
	_lastPos = end;

	++_stats.lines;
}
