
#ifndef __LineElements_h_
#define __LineElements_h_

#include "Tools/Math/Vector2.h"



/**
* @class LineElements
* This class contains all the lineelments and nonlineselments (=white regions which are no lines)
*/
class LineElements
{
public:

	/**
	* @class LineElement
	* A class that represents a spot that's an indication of a line.
	*/
	class LineElement
	{
	public:
		int x_s, /**< "schwerpunkt_x" */
			y_s; /**< "schwerpunkt_y" */
		double alpha, /**< the direction/rotation of the region    | */
			alpha_len, /**< "ausbreitung entlang alpha"         |-> Haupttraegheitsachsenbla *///主轴的一半
			alpha_len2; /**< "ausbreitung orthogonal zu alpha"  | *///辅轴的一半
		Vector2<int> p1, p2; /**< The starting/end point of this linespot in image coordinates*/

	};

	/**
	* @class NonLineElement
	* This class represents a white region which is no line
	*/
	class NonLineElement
	{
	public:
		Vector2<int> p1, p2; /**< start/end point of this spot in image coordinates */
		int size; /**< The size of the coresponding region in the image */
	};	
};
#endif 
