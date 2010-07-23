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

public class Cords
{
			public float x,y;
			boolean inInch=false;
			
			public Cords(float x, float y, boolean inch)
			{
				this.x = x;
				this.y = y;
				
				if(inch)
				{
					this.inInch = true;
					inchToMM();
				}
			}
			
			private void inchToMM()
			{
				if(inInch)
				{
					int offsetY=30;
					int offsetX=30;
					
					x = x*25.4f+offsetX;
					y = y*25.4f+offsetY;
					
					inInch=false;
				}
					//System.out.println(num+" INCH in MM "+mm);
			}
			
			public boolean equals(Cords b)
			{
				
				if(Float.compare(x, b.x) == 0 && Float.compare(y, b.y) == 0)
					return true;
				
				return false;
				
			}
			
			
			public void add(Cords b)
			{
				x += b.x;
				y += b.y;
			}
			
			public void sub(Cords b)
			{
				x -= b.x;
				y -= b.y;
			}
			
			public Cords mul(float f)
			{
				x = x*f;
				y = y*f;
				
				return this;
			}
			
			public float length()
			{
				return (float) Math.sqrt(Math.pow(x, 2)+Math.pow(y, 2));
			}
			
			public float distance(Cords ob)
			{
				Cords b = ob.clone();
				b.sub(this);
				
				return b.length();				
			}
			
			public Cords clone()
			{
				return new Cords(x,y, false);
			}
			
}