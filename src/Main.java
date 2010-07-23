/*  Gerber RS274X to GCoder Converter for RepRap's
 *  Copyright (C) 2010 Gerrit Wyen <gerrit@ionscale.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;


public class Main {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		GerberGCode gerberGcode; 
		String[] splitline;
		boolean debug = false;
		
		BufferedReader in;
		String line;
		String formatX = "23", formatY="23";
		boolean inInch = false;
		

		// Config
		float penWidth = 0.7f;		
		float offsetX=40;
		float offsetY=40;
		int XYFeedrate = 1000;
		int ZFeedrate = 70;
		float drawingHeight = 1.8f;
		float freemoveHeight = 3.8f;//1.7f;
		// Config end
		
		String outputFilename, inputFilename;

		System.out.println("Gerber RS274X to GCoder Converter for RepRap's\n");

		if(args.length < 2)
		{
			System.out.println("Usage: java Main [Gerber filename] [GCode filename]");
			System.exit(-1);
		}
		inputFilename = args[0];
		outputFilename = args[1];
		
		System.out.println("Input: " + inputFilename);
		System.out.println("Output: " + outputFilename+"\n");
		System.out.println("Pen Width: " + penWidth + " mm");
		System.out.println("Offset X: " + offsetX + " mm");
		System.out.println("Offset Y: " + offsetY + " mm");
		System.out.println("Drawing Height: " + drawingHeight + " mm");
		System.out.println("Freemove Height: " + freemoveHeight + " mm\n");
		
		gerberGcode = new GerberGCode(penWidth, drawingHeight, freemoveHeight, XYFeedrate, ZFeedrate); 
		
		// processing Gerber file
		try {
			in = new BufferedReader(new FileReader(inputFilename));
			
			
			
			while((line = in.readLine()) != null)
			{
				if(debug) System.out.println(line);
				
				if(line.startsWith("%FSLA"))
				{
					
					formatX = line.substring(6, 8);
					formatY = line.substring(9, 11);
					if(debug) System.out.println("Format X: " + formatX + " Format Y: " + formatY);
				}
				else
				if(line.startsWith("%ADD"))
				{
					String apertureNum, apertureType, apertureSize;
					
					
					line = line.substring(4, line.length()-2);
					apertureNum = line.substring(0, 2); 
					
					line = line.substring(2);
					splitline = line.split(",");
					apertureType = splitline[0];
					apertureSize = splitline[1]; 
							
					if(debug) 
					{
						System.out.println("\n\nAparture: " + apertureNum);
						System.out.println("Type: " + apertureType);
						System.out.println("Size: " + apertureSize);
					}
					
					if(apertureType.equals("C"))
					{
						gerberGcode.addCircleAperture(Integer.parseInt(apertureNum), Float.parseFloat(apertureSize));
					}
					else
					if(apertureType.equals("R"))
					{
						
						String rectSides[] = apertureSize.split("X");
						
						gerberGcode.addRectangleAperture(Integer.parseInt(apertureNum), Float.parseFloat(rectSides[0]), Float.parseFloat(rectSides[1]));
					}
					else
					if(apertureType.equals("OC8"))
					{
						gerberGcode.addCircleAperture(Integer.parseInt(apertureNum), Float.parseFloat(apertureSize));							
					}
					else
					{
						System.out.println(" [-] aparture type: " + apertureType + " not supported [" + line+"]\n");
						//System.exit(-1);
					}
					
				}
				else
				if(line.startsWith("G90"))
				{
					gerberGcode.enableAbsolute();
				}
				else
				if(line.startsWith("G91"))
				{
					gerberGcode.enableRelative();
				}
				else
				if(line.startsWith("G70"))
				{
					gerberGcode.setImperial();
					inInch = true;
					
					offsetX = offsetX/25.4f;
					offsetY = offsetY/25.4f;
				}
				else
				if(line.startsWith("G71"))
				{
					gerberGcode.setMetric();			
				}
				else
				if(line.startsWith("G54"))
				{
					int aperture;
					
					aperture = Integer.valueOf(line.substring(4, line.length()-1).trim());
					gerberGcode.selectAperture(aperture);
									
				}
				else
				if(line.startsWith("X"))
				{
					float x, y;
					int d;
					int divFactorX = (int)Math.pow(10.0,Integer.parseInt(formatX.substring(1)));
					int divFactorY = (int)Math.pow(10.0,Integer.parseInt(formatY.substring(1)));

					x = Float.valueOf(line.substring(1, line.indexOf("Y")))/divFactorX;
					y = Float.valueOf(line.substring(line.indexOf("Y")+1, line.indexOf("D")))/divFactorY;
					d = Integer.valueOf(line.substring(line.indexOf("D")+1, line.indexOf("D")+3));
				
					x += offsetX;
					y += offsetY;
					
					if(debug)  System.out.println(" X: "+x+" Y:"+y+" D:"+d);
					
					if(d==1)
					{
						gerberGcode.addLine(new Cords(x, y, inInch));
					}
					else
					if(d==2)
					{
						gerberGcode.goTo(new Cords(x, y, inInch));
					}	
					else
					if(d==3)
					{
						gerberGcode.exposePoint(new Cords(x, y, inInch));
					}
				}
				else
				if(line.startsWith("D"))
				{
					int aperture;
					
					aperture = Integer.valueOf(line.substring(1, 3));
					gerberGcode.selectAperture(aperture);
				}
			}

			
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		//System.out.println(gerberGcode.getGCode());
		
		BufferedWriter outfile;
		try {
			outfile = new BufferedWriter(new FileWriter(outputFilename));
			outfile.write(gerberGcode.getGCode());
			outfile.close();
		} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
		}
		
		System.out.println("GCode file generated succesfully !");
	}

}
