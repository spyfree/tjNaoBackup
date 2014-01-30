#ifndef __AVIHelp_h_
#define __AVIHelp_h_
#include <Windows.h>
#include <Vfw.h>
#include <string>
class AVIHelp
{
public:
	class BMPInfoHeader// 40 bytes (total)
	{
	public:
		unsigned int  size;             // Info header size
		int width;            // Width of image
		int height;           // Height of image
		short      planes;           // Number of color planes
		short      bits;             // Bits per pixel
		unsigned int  compression;      // Compression type
		unsigned int  imagesize;        // Image size
		int xresolution;      // X pixels per meter
		int yresolution;      // Y pixels per meter
		unsigned int  ncolors;          // Number of colors
		unsigned int  nimportantcolors; // Number of important colors
		BMPInfoHeader() {
			size             = 40;  // sizeof(BMPInfoHeader)
			width            = 0;
			height           = 0;
			planes           = 1;
			bits             = 24;
			compression      = 0;// No compression - RGB bitmap
			imagesize        = 0;
			xresolution      = 5706; // 144dpi
			yresolution      = 5706; // 144dpi
			ncolors          = 0;
			nimportantcolors = 0;
		}
	};
public:
	AVIHelp(const char* aviFile, int fps);
	~AVIHelp();
	bool addFrame(const unsigned char* data, unsigned int width, unsigned int height, unsigned int BPP);
	bool finish();
private:
	std::string fileName;
	bool opened;
	unsigned long frame;
	unsigned int skip;
	unsigned int skipCounter;
	AVISTREAMINFO strhdr;
	PAVIFILE pfile;
	PAVISTREAM ps;
};
#endif