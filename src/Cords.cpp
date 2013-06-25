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

Cords::Cords(float x, float y, bool inch) : _x(x), _y(y), _inInch(inch) {
	if(inch){
		inchToMM();
	}
}

Cords::Cords() {
	
}

Cords::~Cords() {
	// TODO Auto-generated destructor stub
}

void Cords::inchToMM(){
	if(_inInch){
		_x = _x*25.4f;
		_y = _y*25.4f;

		_inInch=false;
	}
}
float Cords::getLength() const{
	return (float) sqrt(pow(_x, 2)+pow(_y, 2));
}

float Cords::getDistance(const Cords& ob) const{
	Cords b = ob;
	b -= *this;

	return b.getLength();
}
