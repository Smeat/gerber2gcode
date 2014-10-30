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
#include "GcodeGenerator.h"

#include <vector>
#include <boost/program_options.hpp>
#include "shapes/Rectangle.h"

#include "test/TestCords.h"
#include "Cords.h"
#include "shapes/Line.h"
#include "image/SVGExport.h"

namespace po = boost::program_options;
po::options_description desc("Allowed options");

//#define TEST


void showHelp(bool exit, int code = 0){
	std::cout << desc << "\n";

	if(exit){
		::exit(code);
	}
}

int main(int argc, char** argv){
#ifdef TEST
	testAll();


#else

	std::string input, output;
	double penWidth, drawingHeight, moveHeight;
	int xyFeedrate, moveFeedrate, zFeedrate;



	// Declare the supported options.
	desc.add_options()
	    ("help,h", "produce help message")
	    ("input-file,i", po::value<std::string>(&input), "set input-file")
	    ("output-file,o", po::value<std::string>(&output), "set output-file")
	    ("pen-width,w", po::value<double>(&penWidth)->default_value(0.5), "set pen-width")
	    ("drawing-height", po::value<double>(&drawingHeight)->default_value(2), "set drawing height")
		("move-height", po::value<double>(&moveHeight)->default_value(3), "set move height")
		("draw-feedrate", po::value<int>(&xyFeedrate)->default_value(60*60), "set drawing feedrate (mm/min)")
		("move-feedrate", po::value<int>(&moveFeedrate)->default_value(60*100), "set movement feedrate (mm/min)")
		("z-feedrate", po::value<int>(&zFeedrate)->default_value(70), "set z feedrate (mm/min)")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	//test();
	//exit(0);

	if (vm.count("help")) {
	    showHelp(true);
	}

	if (!vm.count("input-file")) {
		std::cout << "No input-file specified!\n";
		showHelp(true, 1);
	}

	if (!vm.count("output-file")) {
		std::cout << "No output-file specified!\n";
		showHelp(true, 1);
	}/**/

	GerberLoader gerb;

	gerb.openFile(input.c_str());
	gerb.generateGeometry();

	//GcodeGenerator gcode(xyFeedrate, zFeedrate, moveFeedrate, drawingHeight, moveHeight, penWidth, false);

	//GcodeGenerator gcode;

//	gcode.generateGcode(gerb.getLines());
	//gcode.writeData(output);

	GcodeGenerator gcodepolygon(xyFeedrate, zFeedrate, moveFeedrate, drawingHeight, moveHeight, penWidth, false);

	gcodepolygon.generateGcode(gerb.getPolygons());
	gcodepolygon.writeData(output);

	//SVGExport exporter("/tmp/test.svg", 50, 50);

	//exporter.addPolygons(gerb.getPolygons());










#endif

}

