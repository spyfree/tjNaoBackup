// TJImage.cpp: implementation of the TJImage class.
//
//////////////////////////////////////////////////////////////////////

#include "TJImage.h"
#include "Platform/GetSetting.h"
#include <iostream>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef WIN32
static const char* RadiusTableMSHfname = "tables/RadiusTableMSH.dat";
static const char* correctionTableMSHfname = "tables/correctionTableMSH.dat" ;
#else
static const char* RadiusTableMSHfname = "/MS/CCtables/RadiusTableMSH.dat";
static const char* correctionTableMSHfname = "/MS/CCtables/correctionTableMSH.dat" ;
#endif
TJImage::TJImage()
{
	frameNumber = 0;
	memset(unCorrectedRaw, 0, IMAGE_WIDTH * IMAGE_HEIGHT * 3);
	TJlut=NULL;

    lut_=new unsigned char[64*64*64];
#ifdef NAO_DEBUG
    //std::string path = std::string(GetSetting::GetConfigDir()) +"TJLut.dat";
    //loadLUT(const_cast<char*>(path.c_str()),lut_);
	//loadLUT("E:\\TJArkNaoPro\\Make\\..\/Config\\TJLut.dat",lut_);
	// std::string path = std::string(GetSetting::CovertRoot()) +"\\TJLut.dat";
    // loadLUT(const_cast<char*>(path.c_str()),lut_);
	 //setLUT(lut_);
#else
	loadLUT("/home/nao/naoqi/Config/TJLut.dat",lut_);
#endif
    setLUT(lut_);
#ifdef TJClassfiedPic

	classifiedPic=NULL;
	classifiedPic=(char*)malloc(IMAGE_HEIGHT*IMAGE_WIDTH*sizeof(char));
#endif

	imageSize=IMAGE_WIDTH * IMAGE_HEIGHT * 3;
	nbLayers=3;
	width=IMAGE_WIDTH;
	height=IMAGE_HEIGHT;
	colorSpace=10;//kYUVColorSpace is 10
#ifdef GT_CORRECTION
	LoadRCTables();
#endif


}

TJImage::~TJImage()
{
#ifdef TJClassfiedPic
	//if (classifiedPic!=NULL)
	//{
	//	delete[] classifiedPic;
	//	classifiedPic=NULL;
	//}
	free(classifiedPic);
#endif
	TJlut = NULL;

}


void TJImage::loadLUT(char* fileName, unsigned char* lutable)
{
  FILE* lutfile=fopen(fileName,"r");
  if (lutfile==NULL)
  {
	  std::cout<<"fail to open TJLut"<<std::endl;
	  return;
  }
  //std::cout<<"in TJImage::loadLUT succeed in load lut"<<std::endl;
  //obtain file size
  long lutsize;
  fseek(lutfile,0,SEEK_END);
  lutsize=ftell(lutfile);
  rewind(lutfile);

  fread(lutable,1,lutsize,lutfile);
  fclose(lutfile);

}

void TJImage::setLUT(const unsigned char* lut)
{

	TJlut = lut;
}
#ifdef GT_CORRECTION
void TJImage::LoadRCTables()
{
	//Load RadiusTableMSH
	FILE* streamFile = fopen(RadiusTableMSHfname,"rb");
	if(streamFile !=NULL){
		fread(radiusTable,1,IMAGE_HEIGHT*IMAGE_WIDTH,streamFile);
		fclose(streamFile);
	}
	else{
		disable();
	}

	streamFile = fopen(correctionTableMSHfname,"rb");
	if(streamFile !=NULL){
		fread(correctionTable,1,(maxRadius + 1)*256*3,streamFile);
/*		for(int i=0;i<maxRadius + 1;i++){
			for(int c=0;c<256;c++){
				for(int j=0;j<3;j++){
					fread(&(correctionTable[i][c][j]),1,1,streamFile);
				}
			}
		}*/
		fclose(streamFile);
	}
	else{
		disable();
	}

}
void TJImage::disable()
{

  for(int y = 0; y < IMAGE_HEIGHT; ++y)
    for(int x = 0; x < IMAGE_WIDTH; ++x)
     radiusTable[y][x] = maxRadius;

}
#endif
/************************************************************************/
/*       Math tools                                                     */
/************************************************************************/
Vector2<double> TJImage::CoordinateImage2HA(const Vector2<double> &ImageCoordinate, const Geometry::GTLine &horizen) const
{
	Vector2<double> newpoint(0,0);
	newpoint.x = ImageCoordinate.x * horizen.direction.x
		+ImageCoordinate.y * horizen.direction.y;
	newpoint.y = - ImageCoordinate.x * horizen.direction.y
		+ ImageCoordinate.y * horizen.direction.x;
	return newpoint;

}
/*Vector2<double> TJImage::CoordinateImage2HA(const Vector2<int> &ImageCoordinate, const Geometry::GTLine &horizen){
	Vector2<double> newpoint(0,0);
	Vector2<double> ImagePoint(0,0);
	ImagePoint.x = static_cast<double>(ImageCoordinate.x);
	ImagePoint.y = static_cast<double>(ImageCoordinate.y);
	newpoint.x = ImagePoint.x * horizen.direction.x
		+ImagePoint.y * horizen.direction.y;
	newpoint.y = - ImagePoint.x * horizen.direction.y
		+ ImagePoint.y * horizen.direction.x;
	return newpoint;

}*/

////////////////////////

Vector2<double> TJImage::CoordinateHA2Image(const Vector2<double> &HACoordinate, const Geometry::GTLine &horizen) const
{
	Vector2<double> newpoint(0,0);
	newpoint.x = HACoordinate.x * horizen.direction.x
		- HACoordinate.y * horizen.direction.y;
	newpoint.y = HACoordinate.x * horizen.direction.y
		+ HACoordinate.y * horizen.direction.x;
	return newpoint;
}
/*Vector2<double> TJImage::CoordinateHA2Image(const Vector2<int> &HACoordinate, const Geometry::GTLine &horizen){
	Vector2<double> newpoint(0,0);
	Vector2<double> HAPoint(0,0);
	HAPoint.x = static_cast<double>(HACoordinate.x);
	HAPoint.y = static_cast<double>(HACoordinate.y);
	newpoint.x = HAPoint.x * horizen.direction.x
		- HAPoint.y * horizen.direction.y;
	newpoint.y = HAPoint.x * horizen.direction.y
		+ HAPoint.y * horizen.direction.x;
	return newpoint;
}*/
Vector2<int> TJImage::CoordinateHA2ImageInt(const Vector2<double> &HACoordinate, const Geometry::GTLine &horizen) const
{
	Vector2<double> newpoint= CoordinateHA2Image( HACoordinate, horizen);
	Vector2<int> point2(0,0);
	point2.x = (int)(newpoint.x);
	point2.y = (int)(newpoint.y);
	return point2;

}




unsigned char TJImage::getHighResY(int x, int y) const
{
	if ((x & 1) == 0)
		if ((y & 1) == 0)
		{
			// Pixel Top Left				= LL - LH - HL + HH
			y=y>>1;x=x>>1;
			return (unsigned char) ((int)unCorrectedRaw[y][0][x] - (int)unCorrectedRaw[y][3][x] - 128
				- (int)unCorrectedRaw[y][4][x] - 128 + (int)unCorrectedRaw[y][5][x] - 128);
		}
		else
		{
			// Pixel Bottom Left		= LL + LH - HL - HH
			y=y>>1;x=x>>1;
			return (unsigned char)((int)unCorrectedRaw[y][0][x] + (int)unCorrectedRaw[y][3][x] - 128
				- (int)unCorrectedRaw[y][4][x] - 128 - (int)unCorrectedRaw[y][5][x] - 128);
		}
	else
		if ((y & 1) == 0)
		{
			// Pixel Top Right			= LL - LH + HL - HH
			y=y>>1;x=x>>1;
			return (unsigned char)((int)unCorrectedRaw[y][0][x] - (int)unCorrectedRaw[y][3][x] - 128
				+ (int)unCorrectedRaw[y][4][x] - 128 - (int)unCorrectedRaw[y][5][x] - 128);
		}
		else
		{
			// Pixel Bottom Right		= LL + LH + HL + HH
			y=y>>1;x=x>>1;
			return (unsigned char)((int)unCorrectedRaw[y][0][x] + (int)unCorrectedRaw[y][3][x] - 128
				+ (int)unCorrectedRaw[y][4][x] - 128 + (int)unCorrectedRaw[y][5][x] - 128);
		}
}



#ifdef TJClassfiedPic
void TJImage::genclassifiedPic(TJImage &theTJImage)
{
	//classifiedPic=(char*)malloc(IMAGE_HEIGHT*IMAGE_WIDTH);
	int h = 0;
	int w = 0;
	unsigned char y,u,v;

	while (h < FULL_IMAGE_HEIGHT) {
		w = 0;
		//std::cout<<"w is "<<std::endl;
		while (w < FULL_IMAGE_WIDTH) {

			y=/*imagetj->*/theTJImage.unCorrectedRaw[h][w][0];
			u=/*imagetj->*/theTJImage.unCorrectedRaw[h][w][1];
			v=/*imagetj->*/theTJImage.unCorrectedRaw[h][w][2];
			//std::cout<<"y is "<<(int) y<<std::endl;
			char color = (char) getClassifyedColor(y, u, v);
			theTJImage.classifiedPic[h*FULL_IMAGE_WIDTH+w]=color;
			w++;

		}
		h++;
		//std::cout<<"h is "<<h<<std::endl;
	}
}
#endif
