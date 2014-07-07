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

class SVGExport {
private:
	std::string _filename;
	std::stringstream _string;
public:
	SVGExport(const std::string &filename, int width, int height);
	virtual ~SVGExport();

	void addLine(Cords &start, Cords &end, double width);
	void write();
};

#endif /* SVGEXPORT_H_ */
