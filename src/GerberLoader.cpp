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

#include "GerberLoader.h"

GerberLoader::GerberLoader(){
	
	_formatX = "23";
	_formatY ="23";

	_offsetX = -20;
	_offsetY = -20;
	_inInch = false;

}

GerberLoader::~GerberLoader(){
	
}

bool GerberLoader::readLine(std::string* line) {

	if(_fileStream.is_open() && _fileStream.good()){
		std::getline(_fileStream, *line);
		return true;
	}

	return false;
}

bool GerberLoader::generateGeometry() {
	std::string line;

	while(readLine(&line)){

		//Remove G01
		if(boost::starts_with(line, "G01")){
			line = line.substr(3, line.length());
		}

		if(boost::starts_with(line, "%FSLA")){
			_formatX = line.substr(6, 8);
			_formatY = line.substr(9, 11);
		}
		else
		if(boost::starts_with(line, "%ADD")){
			std::string apertureNum, apertureType, apertureSize;

			std::vector<std::string> splitline;


			line = line.substr(4, line.length()-2);
			apertureNum = line.substr(0, 2);

			line = line.substr(2);
			util::split(&splitline, line, ",");

			apertureType = splitline[0];
			apertureSize = splitline[1];

			if(apertureType == "C")	{
				double size = atof(apertureSize.c_str());
				_geo.addCircleAperture(atoi(apertureNum.c_str()), size);
			}
			else
			if(apertureType == "R")	{
				std::vector<std::string> rectSides;

				util::split(&rectSides, apertureSize, "X");

				_geo.addRectangleAperture(atoi(apertureNum.c_str()), atof(rectSides[0].c_str()), atof(rectSides[1].c_str()));
			}
			else
			if(apertureType == "OC8"){
				_geo.addCircleAperture(atoi(apertureNum.c_str()), atof(apertureSize.c_str()));
			}
			else{
				std::cerr << " [-] aparture type: " << apertureType << " not supported [" << line << "]\n";
				//System.exit(-1);
			}

		}
		else
		if(boost::starts_with(line, "G90")){
			_geo.enableAbsolute();
		}
		else
		if(boost::starts_with(line, "G91"))
		{
			_geo.enableRelative();
		}
		else
		if(boost::starts_with(line, "G70") || boost::starts_with(line, "%MOIN*%"))
		{
			if(!_inInch){
				_geo.setImperial();
				_inInch = true;

				_offsetX = _offsetX/25.4f;
				_offsetY = _offsetY/25.4f;
			}
		}
		else
		if(boost::starts_with(line, "G71"))
		{
			_geo.setMetric();
		}
		else
		if(boost::starts_with(line, "G54"))
		{
			int aperture;

			aperture = atoi(line.substr(4, line.length()-1).c_str()); //TODO: trim?
			_geo.selectAperture(aperture);

		}
		else
		if(boost::starts_with(line, "X"))
		{
			double x, y;
			int d;
			int divFactorX = (int)std::pow(10.0,atoi(_formatX.substr(1).c_str()));
			int divFactorY = (int)std::pow(10.0,atoi(_formatY.substr(1).c_str()));

			x = atof(line.substr(1, line.find("Y")).c_str())/divFactorX; //TODO: check if correct (indexof -> find)
			y = atof(line.substr(line.find("Y")+1, line.find("D")).c_str())/divFactorY;
			d = atoi(line.substr(line.find("D")+1, line.find("D")+3).c_str());

			x += _offsetX;
			y += _offsetY;

			//std::cout <<" X: "<<x<<" Y:"<<y<<" D:"<<d<<std::endl;

			if(d==1)
			{
				_geo.addLine(new Cords(x, y, _inInch));
			}
			else
			if(d==2)
			{
				_geo.goTo(new Cords(x, y, _inInch));
			}
			else
			if(d==3)
			{
				_geo.exposePoint(new Cords(x, y, _inInch));
			}
		}
		else
		if(boost::starts_with(line, "D"))
		{
			int aperture;

			aperture = atoi(line.substr(1, 3).c_str());
			_geo.selectAperture(aperture);
		}

	}

	closeFile();

	return true;
}
