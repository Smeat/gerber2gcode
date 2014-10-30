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

#include <boost/foreach.hpp>

#define OPTIMIZED

GcodeGenerator::GcodeGenerator() {
	_XYFeedrate = 60 * 60;
	_ZFeedrate = 100;
	_moveFeedrate = 130 *60;

	_drawingHeight = 1.8;
	_freemoveHeight = 3;//1.7f;

	_gcodestr.precision(6);
	_penWidth = 0.5;
	_drawingOn = false;
	_absolute = true;
	_inInch = false;
	_arcSupport = false;
	_lastPos = Cords(0,0);
}

GcodeGenerator::GcodeGenerator(int xyfeedrade, int zfeedrate, int movefeedrate,
		double drawingheight, double freemoveheight, double penWidth, bool arcSupport) {
	_drawingOn = false;
	_XYFeedrate = xyfeedrade;
	_ZFeedrate = zfeedrate;
	_moveFeedrate = movefeedrate;

	_drawingHeight = drawingheight;
	_freemoveHeight = freemoveheight;//1.7f;

	_gcodestr.precision(6);
	_penWidth = penWidth;
	_absolute = true;
	_inInch = false;
	_arcSupport = arcSupport;
	_lastPos = Cords(0,0);
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

void GcodeGenerator::goTo(const Cords* p) {
	if(p != 0){
		goTo(*p);
	}
}

void GcodeGenerator::goTo(const Cords& p){
	if(_lastPos == p){
		return;
	}
	_gcodestr << ";GoTo Distance: " <<_lastPos.getDistance(p) << "\n";


	if(_lastPos.getDistance(p) > _penWidth*1.005)
		disableDrawing();

	_gcodestr << "G1 X" << p.getX() << " Y" << p.getY() << " F" << _moveFeedrate << "\n";
	_stats.distance += _lastPos.getDistance(p);
	_lastPos = p;
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

void GcodeGenerator::generateGcode(std::vector< boost::geometry::model::polygon<Cords> >* polygons){
	enableAbsolute();


	for(auto iter = polygons->begin(); iter != polygons->end(); ++iter){
		_gcodestr << ";----Drawing Polygon----\n";
		goTo(&(*(iter->outer().rbegin())));
		//Draw outer Polygons
		for(auto outer = iter->outer().begin(); outer != iter->outer().end(); ++outer){
			G01(*outer);
		}
		//Draw inner. Only for enclosing polygons
		for(auto inner_ring = iter->inners().begin(); inner_ring != iter->inners().end(); ++inner_ring){
			//Go to last point on polygon == first point
			goTo(&(*(inner_ring->rbegin())));
			for(auto inner = inner_ring->begin() ; inner != inner_ring->end(); ++inner){
				G01(*inner);
			}

		}
	}
}

void GcodeGenerator::generateGcode(std::vector<Shape_ptr>* shapes, bool mirrorX, bool mirrorY) {

	//_export = new SVGExport("test.svg", 100, 100);

	//zeroShapes(shapes);

	if(mirrorX){
		mirrorXAxis(shapes);
	}
	if(mirrorY){
		mirrorYAxis(shapes);
	}
	_gcodestr << ";-----Config-----\n"
			";XYFeedrate: " << _XYFeedrate << "\n"
			";moveFeedrate: " << _moveFeedrate << "\n"
			";zFeedrate: " << _ZFeedrate << "\n"
			";zHeight: " << _freemoveHeight << "\n"
			";Draw Height: " << _drawingHeight << "\n"
			";------------\n";

	enableAbsolute();


#ifdef OPTIMIZED
	while (shapes->size() != 0){
		Shape_ptr drawing = getNearestShape(shapes, _lastPos);
		draw(drawing);
	}

#else


	for(auto iter = shapes->begin(); iter != shapes->end(); ++iter){
		draw(*iter);
	//	_drawn.push_back(*iter);
		//shapes->erase(iter);
	}
#endif

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


	for(double i = _penWidth; i < radius; i+=_penWidth){
		Cords bottom = util::getCirclePos(mid, i, 270); ///Go to the bottom of the circle

		if(_arcSupport){
			goTo(&bottom);
			_gcodestr << "G02 J" << i << " F" << _XYFeedrate << "\n"; ///Drawing a circle upwards
			_stats.distance += 2 * M_PI * i;
		}
		else {
			for(double j = 0; j < 360; j+= 360/6){
				G01(_lastPos, util::getCirclePos(mid, i, j));
			}
		}
		_lastPos = bottom;

		++_stats.circles;

	}


}


bool GcodeGenerator::writeData(const std::string& fileName) {
	util::writeFile(fileName, getGCode()); //TODO: move to base class
	printf("Writing output file \"%s\"\n", fileName.c_str());
	//_export->write();
	return true;
}

void GcodeGenerator::G01(Cords start, Cords end) {
	goTo(&start);
	G01(end);
}

void GcodeGenerator::G01(Cords end) {
	_gcodestr << "G01 X" << end.getX() << " Y" << end.getY() << " F" << _XYFeedrate << '\n';

	//_export->addLine(_lastPos, end, _penWidth);
	_stats.distance += _lastPos.getDistance(end);
	_lastPos = end;



	++_stats.lines;

}

/*void GcodeGenerator::zeroShapes(std::vector<Shape_ptr>* shapes){
	double minX = 0;
	double minY = 0;

	for(auto iter = shapes->begin(); iter != shapes->end(); ++iter){
		if(Line_ptr line = boost::dynamic_pointer_cast<Line>(*iter)){
			if(line->_start.getX() < minX) minX = line->_start.getX();
			if(line->_end.getX() < minX) minX = line->_end.getX();
			if(line->_start.getY() < minY) minY = line->_start.getY();
			if(line->_end.getY() < minY) minY = line->_end.getY();
		}
	}

}*/

//TODO: O(n^2) is bad. merge mirrorX and mirrorY
void GcodeGenerator::mirrorYAxis(std::vector<Shape_ptr>* shapes) {
	double maxY = 1;
	double minY = 1000;

	for(auto iter = shapes->begin(); iter != shapes->end(); ++iter){
		if(Line_ptr line = boost::dynamic_pointer_cast<Line>(*iter)){
			double startY = line->getEnd().getY();
			double endY = line->getStart().getY();
			double localMin = (startY < endY) ? startY : endY;
			double localMax = (startY > endY) ? startY : endY;

			maxY = (localMax > maxY) ? localMax : maxY;
			minY = (localMin < minY) ? localMin : minY;
		}
		else if(Circle_ptr circle = boost::dynamic_pointer_cast<Circle>(*iter)){
			double localMin = (util::getCirclePos(circle->getMid(), circle->getRadius(), 180)).getY(); //most right
			double localMax = (util::getCirclePos(circle->getMid(), circle->getRadius(), 0)).getY(); //Most left

			maxY = (localMax > maxY) ? localMax : maxY;
			minY = (localMin < minY) ? localMin : minY;
		}
	}

	for(auto iter = shapes->begin(); iter != shapes->end(); ++iter){
		if(Line_ptr line = boost::dynamic_pointer_cast<Line>(*iter)){
			mirrorYAxis(line->getStart_ptr(), maxY, minY);
			mirrorYAxis(line->getEnd_ptr(), maxY, minY);
		}
		else if(Circle_ptr circle = boost::dynamic_pointer_cast<Circle>(*iter)){
			mirrorYAxis(circle->getMid_ptr(), maxY, minY);
		}
	}
}

void GcodeGenerator::mirrorXAxis(std::vector<Shape_ptr>* shapes) {
	double maxX = 1;
	double minX = 1000;

	for(auto iter = shapes->begin(); iter != shapes->end(); ++iter){
		if(Line_ptr line = boost::dynamic_pointer_cast<Line>(*iter)){
			double startX = line->getEnd().getX();
			double endX = line->getStart().getX();
			double localMin = (startX < endX) ? startX : endX;
			double localMax = (startX > endX) ? startX : endX;

			maxX = (localMax > maxX) ? localMax : maxX;
			minX = (localMin < minX) ? localMin : minX;
		}
		else if(Circle_ptr circle = boost::dynamic_pointer_cast<Circle>(*iter)){
			double localMin = (util::getCirclePos(circle->getMid(), circle->getRadius(), 180)).getX(); //most right
			double localMax = (util::getCirclePos(circle->getMid(), circle->getRadius(), 0)).getX(); //Most left

			maxX = (localMax > maxX) ? localMax : maxX;
			minX = (localMin < minX) ? localMin : minX;
		}
	}

	for(auto iter = shapes->begin(); iter != shapes->end(); ++iter){
		if(Line_ptr line = boost::dynamic_pointer_cast<Line>(*iter)){
			mirrorXAxis(line->getStart_ptr(), maxX, minX);
			mirrorXAxis(line->getEnd_ptr(), maxX, minX);
		}
		else if(Circle_ptr circle = boost::dynamic_pointer_cast<Circle>(*iter)){
			mirrorXAxis(circle->getMid_ptr(), maxX, minX);
		}
	}
}

void GcodeGenerator::mirrorXAxis(Cords* cord, double max, double min) {
	double x = cord->getX();

	double newX = fabs(x - (max+min));

	cord->setX(newX);
}

void GcodeGenerator::mirrorYAxis(Cords* cord, double max, double min) {
	double y = cord->getY();

	double newY = fabs(y - (max+min));

	cord->setY(newY);
}
