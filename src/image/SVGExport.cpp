/*
 * SVGExport.cpp
 *
 *  Created on: 07.07.2014
 *      Author: kevin
 */

#include "SVGExport.h"

SVGExport::SVGExport(const std::string &filename, int width, int height) {
	_filename = filename;


	_filestream.open(filename.c_str(), std::ofstream::out);

	 _mapper = new boost::geometry::svg_mapper<Cords>(_filestream, width, height);


}



SVGExport::~SVGExport() {
	delete _mapper;

	printf("Wrote svg to %s\n", _filename.c_str());
}


void SVGExport::addPolygons(std::vector<Polygon>* polys, int red, int green, int blue){
	for(auto iter = polys->begin(); iter != polys->end(); ++iter){
		addGeometry(*iter, red, green, blue);
	}
}
