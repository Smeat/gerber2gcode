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
#include "../Cords.h"



void testOperators(){
	Cords a(1.45, 0.002, false);
	Cords b(3.124, 0.02, false);
	Cords* p = new Cords(1.25, 3.5, false);

	if(a == b){
		std::cerr << "ERROR a == b!!!" << std::endl;
	}
	if(a != b){
		std::cout << "OK a != b" << std::endl;
	}

	if(a != a){
		std::cerr << "ERROR a != a!!!!" << std::endl;
	}

	if(a == a){
		std::cout << "OK a ==a" << std::endl;
	}

	if(a == *p){
		std::cerr << "ERROR a == p!!!" << std::endl;

	}

	if(a != *p){
		std::cout << "OK a != p" << std::endl;

	}

	delete p;
}

void testAll(){
	testOperators();

}
