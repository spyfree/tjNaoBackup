// TJImage.h: interface for the TJImage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __TJIMAGE__
#define __TJIMAGE__
// #include "ring.h"
#include "Module/Common/Geometry.h"
#include "Module/Common/VisionHeader.h"
#include"memory.h"
////////ring correction switch///////////
////Ring.h可以用,使用correctRing.java产生.但还不知道产生原理.需要研究!!!//////
//#define USE_RING_CORRECTION
#define GT_CORRECTION

#ifdef NAO_DEBUG
#define TJPIC//used for sending vision results to VisionResult window while debugging
#else
#define TJClassfiedPic//used for sending vision results through TCP/IP
#endif

class TJImage
{
public:
	TJImage();
	virtual ~TJImage();
public: //tool functions


/**
	* The function get the classifyed color
	* @param: corrected y, u, v values
	*/
	inline unsigned char getClassifyedColor(const unsigned char yy, const unsigned char uu, const unsigned char vv) const
	{
		if(TJlut){
			int offset = (yy >> 2 << 12)+(uu >> 2 << 6)+(vv >> 2);
			return *(TJlut + offset);
		}
		else
		{
			return 0;
		}
	}
	/**return the classifyed color for the given (x,y) position in the image*/
	inline unsigned char getClassifyedColor(int x, int y) const
	{
		if (TJlut)
		{
			unsigned char yy = unCorrectedRaw[y][x][0];
			unsigned char uu = unCorrectedRaw[y][x][1];
			unsigned char vv = unCorrectedRaw[y][x][2];
			int offset = (yy >> 2 << 12)+(uu >> 2 << 6)+(vv >> 2);
			return *(TJlut + offset);
		}
		else
		{
			return 0;
		}
	}
	void loadLUT(char* fileName, unsigned char* lutable);
	/**
	* Set the Color LUT(look up table)
	*/
	void setLUT(const unsigned char* lut);
		///////////////////From GT2005////////////////////
		/** Returns the high resolution y value of a pixel
   *  @param x The x coord. of the pixel in high resolution
   *  @param y The y coord. of the pixel in high resolution
	 *  @return The y value of the pixel
	 */
	unsigned char getHighResY(int x, int y) const;

	//////////////////From UNSW05 RingCorrection/////////////
	inline unsigned char Gy(int x, int y,int yuvY) const{
#ifdef USE_RING_CORRECTION
		return sliceY[yuvY][(xLUT[y][abs(x-cyx)])];
#endif
#ifdef GT_CORRECTION
	if ((x >= 0) && (y >= 0))
 		return correctionTable[radiusTable[y][x]][yuvY][0];
	else
		return 0;
#endif
		return yuvY;
	}


	inline unsigned char Gu(int x, int y,int yuvU) const{
#ifdef USE_RING_CORRECTION
		return sliceU[yuvU][(xLUT[y][abs(x-cux)])];
#endif
#ifdef GT_CORRECTION
		if ((x >= 0) && (y >= 0))
 			return correctionTable[radiusTable[y][x]][yuvU][1];
		else
			return 0;
#endif
		return yuvU;
	}


	inline unsigned char Gv(int x, int y,int yuvV) const{
#ifdef USE_RING_CORRECTION
		return sliceV[yuvV][(xLUT[y][abs(x-cvx)])];
#endif
#ifdef GT_CORRECTION
		if ((x >= 0) && (y >= 0))
 			return correctionTable[radiusTable[y][x]][yuvV][2];
		else
			return 0;
#endif
		return yuvV;
        }
////////////////////////////////////////////////////
	/**
	* The function transform a point in the image coordinate to
	* Horizontal-Aligned coordinate system
	* Be Careful of the following facts:
	*  - The coordinates of the top left corner is (0, 0) in the horizon-aligned coordinate
	*    system, so we only use rotation transformation.
	* @param ImageCoordinate, point in the image coordinate system
	* @return point in the horizontal-aligned coordinate system
	*/

	Vector2<double> CoordinateImage2HA(const Vector2<double> &ImageCoordinate, const Geometry::GTLine &horizen) const;

	//Vector2<double> CoordinateImage2HA(const Vector2<int> &ImageCoordinate, const Geometry::GTLine &horizen) const;

	/**
	* The function transform a point in the HA coordinate
	* to a point in the image coordinate, refer to CoordinateImage2HA()
	* @param HACoordinate, point in the HA coordinate system
	* @return point in the horizontal-aligned coordinate system
	*/

	Vector2<double> CoordinateHA2Image(const Vector2<double> &HACoordinate, const Geometry::GTLine &horizen) const;

	//Vector2<double> CoordinateHA2Image(const Vector2<int> &HACoordinate, const Geometry::GTLine &horizen) const;
    Vector2<int> CoordinateHA2ImageInt(const Vector2<double> &HACoordinate, const Geometry::GTLine &horizen) const;

public: //variables
	/** representation for an unclassifyed raw image
	* this image is directly from camera, without ring correction!
	* height color width
	* point of origin is the upper left corner, height is positive downwards
	* and width positive to the right
	* the color values are in the order Y,U,V, 3xY in a higher resolution
    */
	unsigned char unCorrectedRaw[IMAGE_HEIGHT][IMAGE_WIDTH][3];

#ifdef TJClassfiedPic
	char *classifiedPic;
	void genclassifiedPic(TJImage& theTJImage);
#endif

	/** the frame number of that image */
	unsigned long frameNumber;

	/**Color LUT(look up table)*/
	unsigned char * lut_;
	const unsigned char* TJlut;

	int imageSize;
	int nbLayers;
	int colorSpace;
	int width;
	int height;


private://variables

  /************************************************************************/
  /* GT  Image Correction                                                 */
  /************************************************************************/
#ifdef GT_CORRECTION
  enum {maxRadius = 140, centerRadius = 10};
  unsigned char radiusTable[IMAGE_HEIGHT][IMAGE_WIDTH]; /**< The radius table. Per (simulated) robot. */
  unsigned char correctionTable[maxRadius + 1][256][3]; /**< The correction table. */
  ///////////////////////////////////////////////////////////////////////////
  /**
  * Loads the calibration image and computes the lookup table
  */
  void LoadRCTables();

  /**
  * The function disables the color correction.
  * The table is cleared.
  */
  void disable();
#endif
};

#endif //__TJIMAGE__
