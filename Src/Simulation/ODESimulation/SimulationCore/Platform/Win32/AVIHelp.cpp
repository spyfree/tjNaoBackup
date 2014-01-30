#include "AVIHelp.h"
#include <iostream>
AVIHelp::AVIHelp(const char* aviFile, int fps)
:fileName(aviFile), opened(false), frame(0),pfile(0), skip(0),skipCounter(0)
{
	int hr;
	//int a1 = AVIERR_BADFORMAT, a2 = AVIERR_FILEREAD, a3 = AVIERR_FILEOPEN, a4 = REGDB_E_CLASSNOTREG;
	AVIFileInit();
	if((hr = AVIFileOpenA(&pfile,reinterpret_cast<LPCSTR>(aviFile), OF_WRITE | OF_CREATE, NULL)) != 0)
	{
		opened = false;
		std::cout<<"AVI File "<<aviFile<<" Open Failed!"<<std::endl;
	}else
	{
		memset(&strhdr, 0, sizeof(strhdr));
		strhdr.fccType = streamtypeVIDEO;
		strhdr.fccHandler = 0;
		strhdr.dwScale = 1;
		skip =2; //fps / 24;
		strhdr.dwRate = 24;// 24 fps;
		//strhdr.dwSuggestedBufferSize = 
		hr = AVIFileCreateStream(pfile, &ps, &strhdr);
		if(hr != 0){
			opened = false;
			std::cout<<"AVIFileCreateStream Error"<<std::endl;
		}else
		{
			opened = true;
		}
	}
}
AVIHelp::~AVIHelp()
{
	if(ps){
		AVIStreamRelease(ps);
		ps = 0;
	}
	if(pfile){
		AVIFileRelease(pfile);
		pfile = 0;
	}
	AVIFileExit();
}
bool AVIHelp::addFrame(const unsigned char* data, unsigned int width, unsigned int height, unsigned int BPP)
{
	if(skip > 0)
	{
		if(++skipCounter < skip)
			return true;
		else
			skipCounter = 0;
	}
	unsigned int dstPixelBytes = 3;
	unsigned int srcPixelBytes = BPP /8;
	BMPInfoHeader infoheader;
	infoheader.width     = width/4*4;
	infoheader.height    = height/4*4;
	infoheader.imagesize =infoheader.width  * infoheader.height * dstPixelBytes;
	infoheader.bits = dstPixelBytes*8;
	//===Compressed
	HRESULT hr = AVIStreamSetFormat(ps, frame, &infoheader, sizeof(infoheader));
	if(hr != 0)
	{
		std::cout<<"AVIStreamSetFormat error"<<std::endl;
		return false;
	}
	//===
	
	unsigned char* tmpBuf = new unsigned char[infoheader.imagesize];
	memset(tmpBuf, 0, sizeof(tmpBuf));
	for (int y = 0 ; y < infoheader.height ; y++) {
		for (int x = 0; x < infoheader.width; x++) {
			unsigned int offset  = (y * infoheader.width + x) * dstPixelBytes;
			unsigned int srcOffset = (y * width + x) * srcPixelBytes;
			tmpBuf[offset] = data[srcOffset+2];
			tmpBuf[offset+1] = data[srcOffset+1];
			tmpBuf[offset+2] = data[srcOffset];
		}
	}
	
	hr = AVIStreamWrite(ps, frame, 1, tmpBuf, infoheader.imagesize, 
		AVIIF_KEYFRAME, NULL, NULL);
	frame++;
	if(tmpBuf)
	{
		delete[] tmpBuf;
		tmpBuf = 0;
	}
	return true;
}
bool AVIHelp::finish()
{
	if(pfile){
		AVIFileRelease(pfile);
		pfile = 0;
	}
	frame = 0;
	return true;
}