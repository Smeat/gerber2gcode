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

	_drawingHeight = 1.8f;
	_freemoveHeight = 2.8f;//1.7f;
	// TODO Auto-generated constructor stub

	_gcodestr.precision(6);

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
	}
}

void GcodeGenerator::disableDrawing() {
	if(_drawingOn){
		_gcodestr << ";Disable drawing\n";
		_gcodestr << "G1 Z" << _freemoveHeight << " F" << _ZFeedrate << "\n";
		_drawingOn = false;
	}
}

void GcodeGenerator::goTo(Cords* p) {
	if(p != 0){
		if(_lastCords == *p){
			 return;
		 }
	}
	_gcodestr << ";GoTo\n";

	if(_drawingOn) disableDrawing();
	_gcodestr << "G1 X" << p->getX() << " Y" << p->getY() << " F" << _moveFeedrate << "\n";

	_lastCords = Cords(p->getX(), p->getY(), false);
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

void GcodeGenerator::generateGcode(std::vector<Line_ptr>* _lines) {
	_lastCords = Cords(0,0, _inInch);

	for(auto iter = _lines->begin(); iter != _lines->end(); ++iter){
		draw(*iter);
	}
}

void GcodeGenerator::draw(Line_ptr line){
	draw(line->getStart(), line->getEnd());

}

void GcodeGenerator::draw(Cords start, Cords end) {
	if(start != _lastCords){
		goTo(&start);
	}
	if(!_drawingOn) enableDrawing();

	_gcodestr << "G01 X" << end.getX() << " Y" << end.getY() << " F" << _XYFeedrate << "\n";
}

bool GcodeGenerator::writeData(const std::string& fileName) {
	util::writeFile(fileName, getGCode()); //TODO: move to base class
	return true;
}
