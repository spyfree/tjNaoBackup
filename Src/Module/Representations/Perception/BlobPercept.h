/**
* @file Representations/Perception/BlobPercept.h
*
* Representention of the seen blobs
*
*/

#ifndef __BlobPercept_h_
#define __BlobPercept_h_

#include <vector>
#include "Tools/Math/Vector2.h"

#define MAX_SEGMENTS_COUNT 1200//1000
#define MAX_REGIONS_COUNT 250

#define GAUSS_SUM(x) ( x * ( x + 1 ) / 2 )
#define GAUSS_SUM2(x) ( x * ( x + 1 ) * ( 2 * x + 1 ) / 6 )


/**
* @class BlobPercept
*
*/
class BlobPercept
{
public:
	class Segment;
	/**
	* @class Blob
	* A class to store a Blob
	*/
	class Blob
	{
	public:
		/**
		*Default Constructor
		*/
		Blob()
		{
			childs.reserve(64);
		}

		/** 
		* If the Blob was merged with another one, this
		* function will return the Blob it was merged to.
		* @return the root Blob
		* */
		Blob* getRootBlob() const;

		/** Calculate the center of the Blob in the image.
		* @return the center of the Blob in the image
		*/
		Vector2<int> getCenter() const;

		/** Calculate the (p=)0th (q=)0th moment
		* @return the 0th0th moment
		* */
		int calcMoment00() const;

		/** Calculate the (p=)1th (q=)0th moment
		* @return the 1th0th moment
		* */
		int calcMoment10() const;

		/** Calculate the (p=)0th (q=)1th moment
		* @return the 0th1th moment
		* */
		int calcMoment01() const;

		/** Calculate the (p=)1th (q=)1th centralized moment
		* @param swp_x x-Coordinate of the "Schwerpunkt"
		* @param swp_y y-Coordinate of the "Schwerpunkt"
		* @return the 1th1th moment
		* */
		double calcCMoment11(int swp_x, int swp_y) const;

		/** Calculate the (p=)2th (q=)0th centralized moment
		* @param swp_x x-Coordinate of the "Schwerpunkt"
		* @return the 2th0th moment
		* */
		int calcCMoment20(int swp_x) const;

		/** Calculate the (p=)0th (q=)2th centralized moment
		* @param swp_y y-Coordinate of the "Schwerpunkt"
		* @return the 0th2th moment
		* */
		double calcCMoment02(int swp_y) const;

		/**
		* Merges another Blob to this one. This will be the parent (root)
		* region, the other one will be "dead".
		* @param other region to merge into this one
		*/
		void mergeWithBlob(Blob *other);

		std::vector<Segment*> childs; /**< The child segments of this Blob. */
		std::vector<Blob*> neighborBlobs; /**< The neighbor Blobs of this Blob. */
		int min_y, /**< The minimum y value of the childs. */
			max_y; /**< The maximum y value of the childs. */
		Blob* root; /**< The root Blob, if this Blob was merged to another one. */
		unsigned char color; /**< The color of this Blob. */
		int size; /**< The size in (explored) pixels of the this Blob. */
	};

	/**
	* @class Segment
	*
	* A class to store a Segment
	*/
	class Segment
	{
	public:
		/** Default constructor */
		Segment()
		{
			blob = NULL;
			link = NULL;
		}

		/** The < operator
		* @param s2 Segment to compare to
		* @return is this < s2?
		*/
		bool operator<(Segment *s2);

		int x, /**< The x coordinate of the segment. */
			y, /**< The y coordinate where the segment start. */
			length; /**< The length (in y direction) of the segment. */
		Segment* link; /**< Link to the previous segment in the region. */
		int explored_min_y; /**< The minimum explored y coordiante. */
		int explored_max_y; /**< The maximum explored y coordinate. */
		int explored_size; /**< Aproximated size of this segment. */
		Blob* blob; /**< The Blob this segment belongs to. */
		unsigned char color; /**< The color of this segment. */
	};
      Blob regions[MAX_REGIONS_COUNT];
	  int segmentsCounter, /**< A counter for the segments. This counter always points to index after
						   the last segment in segments. */
						   regionsCounter;  /**< A counter the the regions. This counter always points to the index after
											the last region in regions. */

	//std::vector<Segment> verticalPostSegments; /**< This vector stores all the vertical post / goal
	//										   segments (blue or yellow) found in the image */

	//int gridStepSize; /**< The number of pixels between the scanlines. */
	//std::vector<Vector2<int> > fieldBorders; /**< This vector stores for each scanline the point from
	//										 where the Regionizer started scanning (the fieldorder) */

	/** Constructor */
	BlobPercept() 
	{
		//fieldBorders.reserve(64);
		//horizontalPostSegments.reserve(200);
		//verticalPostSegments.reserve(200);
	}
};

#endif
