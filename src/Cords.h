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

#ifndef CORDS_H_
#define CORDS_H_

#include <cmath>
//#include <boost/unordered_set.hpp>
#include <boost/weak_ptr.hpp>


class Cords {
private:
	double _x, _y;
	bool _inInch;
	//static boost::unordered_set<boost::weak_ptr<Cords> > objects;

public:
	Cords(double x, double y, bool inch);
	Cords();
	virtual ~Cords();
public:

	//TODO: make gcode in inch
	void inchToMM();
	double getLength() const;
	double getDistance(const Cords& ob) const;
	
	double getX(){
		return _x;
	}
	
	double getY(){
		return _y;
	}


	//Operators

	bool operator==(const Cords& b) const{
		return ((_x == b._x) && (_y == b._y));
	}
	
	//Why do I need this function??
	bool operator!=(const Cords&b) const{
		return !(*this == b);
	}

	Cords& operator=(const Cords& b){
		_x = b._x;
		_y = b._y;
		return *this;
	}

	friend Cords operator+(const Cords& a, const Cords& b){
		Cords temp;
		temp._x = a._x + b._x;
		temp._y = a._y + b._y;

		return temp;
	}

	Cords& operator+=(const Cords&b){
		this->_x += b._x;
		this->_y += b._y;

		return *this;
	}

	Cords& operator-=(const Cords&b){
		this->_x -= b._x;
		this->_y -= b._y;

		return *this;
	}

	Cords& operator*=(const Cords&b){
		this->_x *= b._x;
		this->_y *= b._y;

		return *this;
	}

	Cords& operator*=(const double b){
		this->_x *= b;
		this->_y *= b;

		return *this;
	}

	friend Cords operator*(const Cords& a, const Cords& b){
		Cords temp;
		temp._x = a._x * b._x;
		temp._y = a._y * b._y;
		return temp;
	}
	
	friend Cords operator*(const Cords& a, const double b){
		Cords temp;
		temp._x = a._x * b;
		temp._y = a._y * b;
		return temp;
	}


	friend Cords operator-(const Cords& a, const Cords& b){
		Cords temp;
		temp._x = a._x - b._x;
		temp._y = a._y - b._y;
		return temp;
	}
};


typedef boost::shared_ptr<Cords> Cords_ptr;

#endif /* CORDS_H_ */
