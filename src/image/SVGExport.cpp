/*
 * SVGExport.cpp
 *
 *  Created on: 07.07.2014
 *      Author: kevin
 */

#include "SVGExport.h"

SVGExport::SVGExport(const std::string &filename, int width, int height) {
	_filename = filename;

	_string << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << '\n'
			<< "<svg" << '\n'
			<< "xmlns:dc=\"http://purl.org/dc/elements/1.1/\"" << '\n'
			<<  "xmlns:cc=\"http://creativecommons.org/ns#\"" << '\n'
			<<   "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"" << '\n'
			<<   "xmlns:svg=\"http://www.w3.org/2000/svg\"" << '\n'
			<<   "xmlns=\"http://www.w3.org/2000/svg\"" << '\n'
			<<   "version=\"1.1\"" << '\n'
			<<   "width=\"" << width << "mm\"" << '\n'
			<<   "height=\"" << height << "mm\"" << '\n'
			<<   "id=\"svg2\">" << '\n'
			<<  "<defs" << '\n'
			<<   "  id=\"defs4\" />" << '\n'
			<<  "<metadata" << '\n'
			<<  "  id=\"metadata7\">" << '\n'
			<<  " <rdf:RDF>" << '\n'
			<<  "    <cc:Work" << '\n'
			<<  "      rdf:about=\"\">" << '\n'
			<<  "      <dc:format>image/svg+xml</dc:format>" << '\n'
			<<  "      <dc:type" << '\n'
			<<  "         rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />" << '\n'
			<<  "     <dc:title></dc:title>" << '\n'
			<<  "    </cc:Work>" << '\n'
			<<    "</rdf:RDF>" << '\n'
			<<  "</metadata>" << '\n'
			<<  "<g" << '\n'
			<<     "id=\"layer1\">" << '\n' ;
}

SVGExport::~SVGExport() {
	// TODO Auto-generated destructor stub
}

void SVGExport::addLine(Cords &start, Cords &end, double width){
		_string << "<path" << '\n'
					<< "d=\"M " << start.getX() + 20 << "," << start.getY() + 20 << " " << end.getX()+ 20 << "," << end.getY()+20 << "\""  << '\n'
					<<  "style=\"fill:none;stroke:#000000;stroke-width:" << width/10 << "mm;stroke-linecap:round;stroke-linejoin:round;stroke-opacity:1\" />"  << '\n';
}

void SVGExport::write(){
	_string <<   "</g>" << '\n'
			<<	 "</svg>" << '\n';

	std::ofstream of (_filename.c_str(), std::ofstream::out);

	if(of){
		of << _string.str();

	}

	of.flush();
	of.close();

}

