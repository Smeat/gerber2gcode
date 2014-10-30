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

#include "GerberGeometry.h"

typedef boost::geometry::model::polygon<Cords> Polygon;

Aperture::Aperture(int num, double width, char type, bool inInch){
	this->_num = num;
	this->_width = width;
	this->_height = width;
	this->_type = type;

	if(inInch)
	{
		this->_width = this->_width*25.4f;
	}
}

Aperture::Aperture(int num, double width, double height, char type, bool inInch){

	this->_num = num;
	this->_width = width;
	this->_height = height;
	this->_type = type;

	if(inInch)
	{
		this->_width = this->_width*25.4f;
		this->_height = this->_height*25.4f;
	}
}

Aperture::~Aperture(){

}

GerberGeometry::GerberGeometry() : _exporter("/tmp/test.svg", 1000, 1000), _exporterErr("/tmp/error.svg", 1000, 1000) {
	_curAperture = NULL;
	_inInch = false;

	enableAbsolute();

	_polyMode = false;

}

GerberGeometry::~GerberGeometry() {

}

void GerberGeometry::selectAperture(int aperture) {
	for(auto iter = _apertures.begin(); iter != _apertures.end(); ++iter){
		if((*iter)->getNum() == aperture){
			_curAperture = *iter;
			return;
		}
	}
	std::cerr << "Could not select aperture!!!!!!" << std::endl;
}

void GerberGeometry::addCircleAperture(int apertureNum, double width) {
	_apertures.push_back(new Aperture(apertureNum, width, 'C', _inInch));
}

void GerberGeometry::addRectangleAperture(int apertureNum, double width,
		double height) {
	_apertures.push_back(new Aperture(apertureNum, width, height, 'R', _inInch));
}

void GerberGeometry::addLine(Cords_ptr start, Cords_ptr end) {

	//Cords vec;

	_shapes.push_back(Line_ptr(new Line(start.get(), end.get(), _curAperture->getWidth())));

	exposePoint(start);
	exposePoint(end);

}

void GerberGeometry::addLine(Cords_ptr end) {
	if(_polyMode){
		_curPoly.outer().push_back(*end);
	}
	else{
		addLine(Cords_ptr(new Cords(_lastCords)), end);
	}
}

void GerberGeometry::addPolygon(Polygon poly){
	_polygons.push_back(poly);
}

void GerberGeometry::goTo(Cords_ptr p) {
	if(p != 0){
		if(_lastCords == *p){
			 return;
		 }
	}
	_lastCords = Cords(p->getX(), p->getY(), false);

	if(_polyMode){
		addLine(p, false);
	}
}

void GerberGeometry::exposePoint(Cords_ptr p) {
	if(_curAperture->getType() == 'C')
		createCircle(p);
	else
		createRec(p);
}

//FIXME: rotation!
void GerberGeometry::createRec(Cords_ptr p) {
	double recWidth = _curAperture->getWidth()/2.0f;
	double recHeight = _curAperture->getHeight()/2.0f;


	Cords start(p->getX()-recWidth, p->getY() - recHeight );
	Cords end(p->getX() + recWidth, p->getY() + recHeight);

	_shapes.push_back(Rectangle_ptr(new Rectangle(start, end)));

	//addLine(start, end, recHeight);

/*
	startX = p->getX()-recWidth;
	startY = p->getY()-recHeight;
	endX = p->getX()+recWidth;
	endY = p->getY()-recHeight;
	//addLine(new Cords(startX, startY, false));
	goTo(new Cords(startX, startY, false));
	//  ---->
	//  |   |
	addLine(new Cords(endX, endY, false), false);

	endX = p->getX()+recWidth;
	endY = p->getY()+recHeight;
	//   ____
	//  |    :
	addLine(new Cords(endX, endY, false), false);

	endX = p->getX()-recWidth;
	endY = p->getY()+recHeight;
	//  |    |
	//  <-----
	addLine(new Cords(endX, endY, false), false);

	endX = p->getX()-recWidth;
	endY = p->getY()-recHeight;
	//  ^     |
	//  :_____|
	addLine(new Cords(endX, endY, false), false);*/
}

void GerberGeometry::createCircle(Cords_ptr p) {
		_shapes.push_back(Shape_ptr(new Circle(p.get(), _curAperture->getWidth()/2)));

}

//TODO: performance is _VERY_ bad
void GerberGeometry::generatePolygons(){

	std::deque<Polygon> polygons;

	/// Convert lines to Polygons
	for(auto shapeiter = _shapes.begin(); shapeiter != _shapes.end(); ++shapeiter){
		if(Line_ptr line = boost::dynamic_pointer_cast<Line>(*shapeiter)){
			boost::geometry::model::polygon<Cords> polygon;
			polygon = Rectangle(line).getPolygon();
			polygons.push_back(polygon);
			//_exporter.addGeometry(polygon, 255, 0, 0);
		}
		if(Circle_ptr circle = boost::dynamic_pointer_cast<Circle>(*shapeiter)){
			boost::geometry::model::polygon<Cords> polygon;
			polygon = Rectangle(circle).getPolygon();
			polygons.push_back(polygon);
		//	_exporter.addGeometry(polygon, 0, 0, 255);
		}
		if(Rectangle_ptr rect = boost::dynamic_pointer_cast<Rectangle>(*shapeiter)){
			boost::geometry::model::polygon<Cords> polygon = rect->getPolygon();
			polygons.push_back(polygon);
		//	_exporter.addGeometry(polygon, 0, 0, 255);
		}
	}

	//Copy already existing polygons to temporary space TODO: better variant std::copy or similar
	for(auto iter = _polygons.begin(); iter != _polygons.end(); ++iter){
		polygons.push_back(*iter);
	}

	_polygons.clear();
#ifdef POLY_OUTPUT
	printf("------------ Input Polygons ---------------\n");
	for(auto poly = polygons.begin(); poly != polygons.end(); ++poly){
		std::cout << "POLYGON (( ";
		for(auto iter = poly->outer().begin(); iter != poly->outer().end(); ++iter){
					std::cout << iter->getX() << " " << iter->getY() << " , ";
		}

		std::cout << " ))" << std::endl;
	}
	printf("------------ Input Polygons ---------------\n");
#endif

	printf("Converted all shapes to polygons\n"); fflush(stdout);

	//boost::geometry::unique(polygons);

	mergePolygons(polygons);
}
/*
void GerberGeometry::mergePolygons(std::deque<Polygon>& polygons){


	MultiPolygon multiPoly;

	//polygons.clear();
	Polygon p1, p2;
	 boost::geometry::read_wkt("POLYGON (( 10.6172 14.3688 , 10.6064 14.2866 , 10.5747 14.21 , 10.5242 14.1443 , 10.4584 14.0938 , 10.3819 14.0621 , 10.2997 14.0513 , 10.2175 14.0621 , 10.1409 14.0938 , 10.0752 14.1443 , 10.0247 14.21 , 9.99302 14.2866 , 9.9822 14.3688 , 9.99302 14.451 , 10.0247 14.5275 , 10.0752 14.5933 , 10.1409 14.6437 , 10.2175 14.6755 , 10.2997 14.6863 , 10.3819 14.6755 , 10.4584 14.6437 , 10.5242 14.5933 , 10.5747 14.5275 , 10.6064 14.451 , 10.6172 14.3688 ))", p1);
	boost::geometry::read_wkt("POLYGON ((10.6172 14.351 , 10.6172 3.09372 , 9.9822 3.09372 , 9.9822 14.2698 , 9.97524 14.2866 , 9.96442 14.3688 , 9.97524 14.451 , 10.007 14.5275 , 10.0574 14.5933 , 10.1232 14.6437 , 10.1997 14.6755 , 10.2819 14.6863 , 10.3641 14.6755 , 10.4407 14.6437 , 10.5064 14.5933 , 10.5064 14.5933 , 10.5242 14.5755 , 10.5242 14.5755 , 10.5747 14.5097 , 10.6064 14.4332 , 10.6149 14.3688 , 10.6172 14.3688 , 10.6172 14.351 ))", p2);

	//polygons.push_back(p1);
	//polygons.push_back(p2);

	for(auto iter = polygons.begin() ; iter != polygons.end(); ++iter){
		//std::cout << *iter << std::endl;
		//_exporter.addGeometry(*iter);
		Polygon poly = *iter;

		poly = util::copyPolygon(poly);

		//FIXME: why do I need to recreate the polygon? see copy-bug.gbr file
		//multiPoly = util::copyMultiPolygon(multiPoly);

		boost::geometry::correct(poly);
		boost::geometry::correct(multiPoly);

	//	_exporter.addGeometry(poly);

		//not intersecting -> add it to multipoly
		if(!(boost::geometry::intersects(multiPoly, poly))){
			multiPoly.push_back(poly);
		}
		else { //intersecting need to merge
			MultiPolygon output;


			boost::geometry::union_(poly, multiPoly, output);

			if(output.size() == 0){
				std::cerr << "-------------------ERROR!!!!----------------------------" << std::endl;
				_exporterErr.addGeometry(poly, 255, 0,0,10);
				_exporterErr.addGeometry(multiPoly);

				std::cerr << poly << std::endl << multiPoly << std::endl;
				std::cerr << p1 << std::endl << p2 << std::endl;
				return;
			}

			multiPoly = output;
		}

	}


	boost::geometry::correct(multiPoly);
	boost::geometry::unique(multiPoly);

	_exporter.addGeometry(multiPoly, 0, 255, 0, 10);







}
/**/

void GerberGeometry::mergePolygons(std::deque<Polygon>& polygons){

	bool merged = false;
	int mergeCount = 0;
	//FIXME: endless loop?
	while(polygons.size() != 0){

		merged = false;
		Polygon poly = polygons.front();
		polygons.pop_front();
		int size = polygons.size();
		for(int i = 0; i < size; ++i){
			Polygon poly2 = polygons.front();
			polygons.pop_front();

			if(boost::geometry::intersects(poly, poly2)){

				size_t size_before = polygons.size();
				boost::geometry::union_(poly, poly2, polygons);

				merged = true;
				++mergeCount;

				if(size_before == polygons.size()){
					std::cerr << "----------------SOMETHING WENT WRONG!!! MERGING FAILED!!!---------------\n";
					std::cerr << "Tried to Merge " << poly << " \n and " << poly2 << std::endl;


					_exporterErr.addGeometry(poly, 255);
					_exporterErr.addGeometry(poly2, 0, 0, 255);

					std::cerr << "Adding bigger polygon..." << std::endl;

					double area = boost::geometry::area(poly);
					double area2 =  boost::geometry::area(poly2);

					if(area > area2){
						polygons.push_back(poly);
					}
					else {
						polygons.push_back(poly2);
					}

				}


				break;
			}
			else {
				polygons.push_back(poly2);
			}
		}
		if(!merged || polygons.size() == 0){
			printf("Merged Polygons. %lu Polygons Remaining\r", polygons.size()); fflush(stdout);
			_polygons.push_back(poly);
			_exporter.addGeometry(poly);
		}
	}

//	_exporter.addPolygons(&_polygons);




	printf("Generated %lu polygons with %i merges\n", _polygons.size(), mergeCount);
}
/**/

/**
 *
 * "Algorithm":
 *
 * 1. Find bounding box
 * 2. Generate vertical lines based on tool width
 * 3. Calculate difference between each line and all polygons
 *
 *
 *	Todo: don't cross perimeter!!! convert line to boxes and use within or covered_by to check if they are within
 */
void GerberGeometry::generateInfill(){

	double pen_width = 0.1;
	///Find bounding box
	boost::geometry::model::box<Cords> box;
	boost::geometry::envelope(_polygons, box);

	std::vector<Linestring> lines;

	_exporter.addGeometry(box, 255);

	///generate vertical lines
	double y = box.min_corner().getY();
	double x = box.min_corner().getX();
	double width = std::abs(box.min_corner().getX() - box.max_corner().getX());
	double height = std::abs(box.min_corner().getY() - box.max_corner().getY());

	for(double i = 0; i < width; i+=pen_width){
		Cords a(x + i, y);
		Cords b(x + i, y + height);
		Linestring line;
		line.push_back(a);
		line.push_back(b);

		boost::geometry::intersection(line, _polygons, lines);

	}

	//TODO: check for perimeter crossing!!


	//TODO: sort lines

	_exporter.addGeometries(lines);


}












void GerberGeometry::enablePolygonMode(){
	_polyMode = true;
	_curPoly.clear();
}

void GerberGeometry::disablePolygonMode(){
	_polyMode = false;
	boost::geometry::correct(_curPoly);
	addPolygon(_curPoly);
	_curPoly.clear();
}




