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
#include "shapes/Rectangle.h"
#include "shapes/Line.h"
#include "GerberGeometry.h"

#include <boost/geometry/geometries/concepts/check.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <vector>


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

void testGeometries(){
	/*
	Cords a(10, 10);
	Cords b(20, 20);
	Line line(new Cords(10, 15), new Cords(20,15), 10, false);

	Rectangle myRect(line);

	boost::geometry::model::box<Cords> box = boost::geometry::make<boost::geometry::model::box<Cords> > (10,10, 20,20);

	double boxArea = boost::geometry::area(box);
	double myArea = boost::geometry::area(myRect);

	if(boxArea != myArea){
		std::cerr << "Error Rectangle is wrong Expected: " << boxArea << " got: " << myArea << std::endl;
	}


  //  double d = boost::geometry::distance(a, b);


    boost::geometry::concept::check<Cords const>();
    boost::geometry::concept::check<Rectangle const>();

    boost::geometry::concept::check<boost::geometry::model::polygon<Cords> const>();


    boost::geometry::model::polygon<Cords> poly;
    boost::geometry::convert(myRect, poly);

    std::vector < boost::geometry::model::polygon<Cords> > output;

    boost::geometry::union_(poly, poly, output);

    if(output.size() != 1){
    	std::cout << "Unexpected merge!" << std::endl;
    }

    boost::geometry::model::polygon<Cords> poly2 = output[0];

    if(poly2.outer() != poly.outer()){
    	std::cout << "Unexpected merge!" << std::endl << "Original: " << std::endl;

    	for(auto iter = poly.outer().begin(); iter != poly.outer().end(); ++iter){
    		std::cout << "( " << iter->getX() << " " << iter->getY() << " ) ";
     	}

    	std::cout << std::endl << "Merge: " << std::endl;

    	for(auto iter = poly2.outer().begin(); iter != poly2.outer().end(); ++iter){
			std::cout << "( " << iter->getX() << " " << iter->getY() << " ) ";
		}



    }
*/
    Polygon polygon1;
    Polygon polygon2;

//    boost::geometry::read_wkt("POLYGON (( 38.928 34.102 , 38.928 35.118 , 46.294 35.118 , 46.294 34.102 , 38.928 34.102  ))", polygon1);
//    boost::geometry::read_wkt("POLYGON (( 42.8567 38.2847 , 42.8567 29.4113 , 42.1113 29.4113 , 42.1113 38.2847 , 42.8567 38.2847  ))", polygon2);

  //  boost::geometry::read_wkt("POLYGON (( 39 34 , 39 35 , 46 35 , 46 34 , 39 34  ))", polygon1);
 //   boost::geometry::read_wkt("POLYGON (( 43 38 , 43 29 , 42 29 , 42 38 , 43 38  ))", polygon2);

 //   boost::geometry::read_wkt("POLYGON (( 39 34 , 39 35 , 46 35 , 46 34 , 39 34  ))", polygon1);
  //boost::geometry::read_wkt("POLYGON (( 43 38 , 43 29 , 42 29 , 42 38 , 43 38  ))", polygon2);

    boost::geometry::read_wkt("POLYGON (( 0 5 , 0 6, 6 5, 5 5, 0 5 ))", polygon1);

    polygon2 = Rectangle(Line(Cords(3,0), Cords(3,10), 0.025)).getPolygon();

   // boost::geometry::read_wkt("LINESTRING ( 5 0, 5 10 )", polygon2);



    std::vector< Polygon > output2;

    boost::geometry::intersection(polygon1, polygon2, output2);

	for(auto poly = output2.begin(); poly != output2.end(); ++poly){
		std::cout << "POLYGON (( ";
		for(auto iter = poly->outer().begin(); iter != poly->outer().end(); ++iter){
					std::cout << iter->getX() << " " << iter->getY() << " , ";
		}

		std::cout << " ))" << std::endl;
	}




	    SVGExport exporter2("/tmp/test2.svg", 50, 50);
	//    exporter2.addPolygons(&outputtest);
	    exporter2.addPolygons(&output2, 255);

	    exporter2.addGeometry(polygon1);
	    exporter2.addGeometry(polygon2);
}


void testAll(){
	testOperators();
	testGeometries();

}
