/*
 * SVGExport.h
 *
 *  Created on: 07.07.2014
 *      Author: kevin
 */

#ifndef SVGEXPORT_H_
#define SVGEXPORT_H_

#include "Cords.h"
#include <sstream>
#include <iostream>
#include <fstream>


#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/io/svg/svg_mapper.hpp>



typedef boost::geometry::model::polygon<Cords> Polygon;

class SVGExport {
private:
	std::string _filename;
	std::stringstream _string;
	boost::geometry::svg_mapper<Cords>* _mapper;
	std::ofstream _filestream;
public:
	SVGExport(const std::string &filename, int width, int height);
	virtual ~SVGExport();

	void addPolygons(std::vector<Polygon>* polys, int red = 0, int green = 0, int blue = 0);

	template <typename Geometry>
		void addGeometries(const std::vector<Geometry>& geo, int red = 0, int green = 0, int blue = 0){

		for(auto iter = geo.begin(); iter != geo.end(); ++iter){
			addGeometry(*iter);
		}

	}

	template <typename Geometry>
	void addGeometry(const Geometry& geo, int red = 0, int green = 0, int blue = 0, int strokewidth = 2){
		_mapper->add(geo);

		std::stringstream ss;

		ss << "fill-opacity:0;stroke:rgb(" << red << "," << green << ","   << blue << ");stroke-width:" << strokewidth;
		_mapper->map(geo, ss.str(), 5);

	}
	void write();
};

#endif /* SVGEXPORT_H_ */
