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

Cords::Cords(const double x, const double y, bool inch) : _x(x), _y(y), _inInch(inch) {
	if(inch){
		inchToMM();
	}
}

Cords::Cords(double const& x, double const& y) : _x(x), _y(y), _inInch(false) {
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
	return ::sqrt(::pow(_x, 2.0) + ::pow(_y, 2.0));
}

double Cords::getDistance(const Cords& ob) const{
	Cords b = ob;
	// (x1-x2), (y1-y2)
	b -= *this;

	return b.getLength();
}
std::ostream& operator<<(std::ostream& os, const Cords& c){
	os << "( " << c.getX() << " " << c.getY() << " )";
	return os;
}
