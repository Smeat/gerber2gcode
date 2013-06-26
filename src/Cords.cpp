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

#include "Cords.h"

Cords::Cords(double x, double y, bool inch) : _x(x), _y(y), _inInch(inch) {
	if(inch){
		inchToMM();
	}
}

Cords::Cords() {
	_inInch = false;
	_x = 0;
	_y = 0;
}

Cords::~Cords() {
	//boost::weak_ptr<Cords> ptr (this);
	//objects.find(ptr);
}

void Cords::inchToMM(){
	if(_inInch){
		_x = _x*25.4f;
		_y = _y*25.4f;

		_inInch=false;
	}
}
double Cords::getLength() const{
	return (double) sqrt(pow(_x, 2)+pow(_y, 2));
}

double Cords::getDistance(const Cords& ob) const{
	Cords b = ob;
	b -= *this;

	return b.getLength();
}
