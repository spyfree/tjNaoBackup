#include "ClassifyImageDialog.h"
#define uMultiply 1//原始图片放大倍数
#define vOffset  10//垂直两张图间距
#define cMultiply 2//色表图片放大倍数
#define  hOffset 10//水平两张图的间距
ClassifyImageDialog::ClassifyImageDialog(QWidget *parent)
:QWidget(parent)
{
	setWindowTitle(tr("Classify Image Window"));
	setWindowIcon(QIcon(":/icons/Simulator.png"));
	setMouseTracking(1);
	setFocusPolicy(Qt::ClickFocus);
	initImageParament();
}
ClassifyImageDialog::~ClassifyImageDialog()
{
	if (unclassifiedImage!=NULL)
	{
		delete unclassifiedImage;
		unclassifiedImage=NULL;
	}
	if (classifiedImage!=NULL)
	{
		delete classifiedImage;
		classifiedImage=NULL;
	}
	if (pureImage!=NULL)
	{
		delete pureImage;
		pureImage= NULL;
	}
	if (tempClassifiedImage!=NULL)
	{
		delete tempClassifiedImage;
		tempClassifiedImage=NULL;
	}
	if (unclassifyed!=NULL)
	{
		delete[] unclassifyed;
		unclassifyed=NULL;
	}
	if (lut_!=NULL)
	{
		delete[] lut_;
		lut_ = NULL;
	}
	if (undoLut!=NULL)
	{
		delete[] undoLut;
		undoLut=NULL;
	}
	if (HSI_lut!=NULL)
	{
		delete[] HSI_lut;
		HSI_lut=NULL;
	}
}
void ClassifyImageDialog::initImageParament()
{
	//////////parament init
	lastFileName = tr("D:/Netpics/");
	lastLutName= tr("D:/Netpics/");
	normalSize = FULL_IMAGE_WIDTH*FULL_IMAGE_HEIGHT*3;
	currentIndex=0;
	currentImageWidth_=FULL_IMAGE_WIDTH;
	currentImageHeight_=FULL_IMAGE_HEIGHT;
	currentpixelY=0;
	currentpixelX=0;
	currentY=0;
	 currentU=0;
	 currentV=0;
	 ySensitivity=10;
	 uSensitivity=10;
	 vSensitivity=10;
	 currentColour=1;
	 DontReplace=true;
	 Replace =  false;
	 Delete = false;
	 hotkeyEnabled=false;
	 hsiEnabled=false;
	 r_min=0;
	 r_max=23;
	 o_min=23;
	 o_max=49;
	 y_min=53;
	 y_max=70;
	 f_min=78;
	 f_max=120;
	 c_min=120;
	 c_max=166;
	 w_min_Y=200;
	 w_max_Y=0;
	////////pointer init
	 unclassifiedImage=NULL;
	 unclassifiedImage=new QImage(FULL_IMAGE_WIDTH, FULL_IMAGE_HEIGHT, QImage::Format_RGB32);


	classifiedImage=NULL;
	classifiedImage=new QImage(FULL_IMAGE_WIDTH, FULL_IMAGE_HEIGHT, QImage::Format_RGB32);

	tempClassifiedImage=NULL;
	tempClassifiedImage=new QImage(FULL_IMAGE_WIDTH, FULL_IMAGE_HEIGHT, QImage::Format_RGB32);

	pureImage=NULL;
	pureImage=new QImage(FULL_IMAGE_WIDTH, FULL_IMAGE_HEIGHT, QImage::Format_RGB32);

	imageBuffer=NULL;

	tempimageBuffer=NULL;

	unclassifyed=NULL;
	unclassifyed=new unsigned char[normalSize];
	memset(unclassifyed,0,currentImageWidth_*currentImageHeight_*sizeof(unsigned char));

	lut_=NULL;
	lut_ = new unsigned char[64*64*64]; // Stores the look up table
	memset(lut_,0,64*64*64);

	undoLut=NULL;
	undoLut = new unsigned char[64*64*64];
	memset(undoLut,0,64*64*64);

	HSI_lut=NULL;
	HSI_lut=new unsigned char[64*64*64];
	memset(HSI_lut,0,64*64*64);
}
void ClassifyImageDialog::openPic()
{
	QString filename = QFileDialog::getOpenFileName(this,tr("open lut"),lastFileName,tr("Images (*.raw)"));
	currentDir=QFileInfo(filename).absoluteDir();
	currentPath=QFileInfo(filename).absolutePath();
	currentList=currentDir.entryList(QStringList(tr("*.raw")),QDir::Files,QDir::Name);
	currentIndex=currentList.indexOf(QFileInfo(filename).fileName());
	if (!filename.isEmpty())
	{
		lastFileName = filename;
		//rawString= filename.toStdString();
		rawString = filename; 
		LoadRawStream();
		GenUnclassifyImage(unclassifyed,unclassifiedImage);
		if(lut_ != NULL)
		{
			GenClassifyImage(lut_,unclassifyed,classifiedImage);
		}
	}		
	update();
	show();
	raise();
	activateWindow();
}
void ClassifyImageDialog::openLut()
{
	QString filename = QFileDialog::getOpenFileName(this,tr("open image"),lastLutName, tr("Lut (*.dat)"));
	if (!filename.isEmpty())
	{
		lastLutName = filename;
		//lutString = filename.toStdString();
		lutString = filename;
		LoadLUT(lutString);
		if (unclassifyed!=NULL)
		{		
			GenClassifyImage(lut_,unclassifyed,classifiedImage);
			emit enableSave(true);
			emit updatePath(filename);
		}
	}		
	update();
	show();
	raise();
	activateWindow();
}
void ClassifyImageDialog::preImage()
{
	if (currentIndex-1>=0)
	{
		QString nextfile(currentPath);
		QString currentFile_onlyname=currentList.at(currentIndex--);
		nextfile.append(tr("/")).append(currentFile_onlyname);
		lastFileName=nextfile;
		//rawString= nextfile.toStdString();
		rawString = nextfile;
		LoadRawStream();
		GenUnclassifyImage(unclassifyed,unclassifiedImage);
		if(lut_ != NULL)
		{
			GenClassifyImage(lut_,unclassifyed,classifiedImage);
		}
		update();
		show();
		raise();
		activateWindow();
	}
	else
		QMessageBox::information(this,tr("Warning"),tr("TO THE START"));
} 
void ClassifyImageDialog::nextImage()
{
	if (currentIndex+1<currentList.size())
	{
		QString nextfile(currentPath);
		QString currentFile_onlyname=currentList.at(currentIndex++);
		nextfile.append(tr("/")).append(currentFile_onlyname);
		lastFileName=nextfile;
		//rawString= nextfile.toStdString();
		rawString = nextfile;
		LoadRawStream();
		GenUnclassifyImage(unclassifyed,unclassifiedImage);
		if(lut_ != NULL)
		{
			GenClassifyImage(lut_,unclassifyed,classifiedImage);
		}
		update();
		show();
		raise();
		activateWindow();
	}
	else
		QMessageBox::information(this,tr("Warning"),tr("TO THE END"));
}
void ClassifyImageDialog::saveAsLut()
{
	QString filename = QFileDialog::getSaveFileName(this,tr("save Lut"),lastLutName, tr("Lut (*.dat)"));
	if (!filename.isEmpty())
	{
		lastLutName=filename;
		//lutString=filename.toStdString();
		lutString = filename;
		/*FILE* f = fopen(lutString.c_str(),"wb");
		fwrite(lut_,1,64*64*64,f);
		fclose(f);*/
		QFile f(lutString);
		f.open(QIODevice::WriteOnly);
		f.write((char*)lut_, 64*64*64);
		f.close();
		emit enableSave(true);
		emit updatePath(filename);
	}
	update();
	show();
	raise();
	activateWindow();
}
void ClassifyImageDialog::saveLut()
{
	/*FILE* f = fopen(lutString.c_str(),"wb");
	fwrite(lut_,1,64*64*64,f);
	fclose(f);*/
	QFile f(lutString);
	f.open(QIODevice::WriteOnly);
	f.write((char*)lut_, 64*64*64);
	f.close();

	update();
	show();
	raise();
	activateWindow();
}
void ClassifyImageDialog::pushUndo()
{
	memcpy(lut_, undoLut, 64*64*64);
   emit enableUnDo(FALSE);
	if(lut_ != NULL)
	{
		GenClassifyImage(lut_,unclassifyed,classifiedImage);
	}
	update();
	show();
	raise();
	activateWindow();
}
void ClassifyImageDialog::updateYSensitivity(const QString &str)
{
	ySensitivity=str.toInt();	
	//////////////增加临时色表的处理
	GenTempClassifyImage(unclassifyed,tempClassifiedImage,pureImage);
	update();
	show();
	raise();
	activateWindow();
}
void ClassifyImageDialog::updateUSensitivity(const QString &str)
{
	uSensitivity=str.toInt();	
	//////////////增加临时色表的处理
	GenTempClassifyImage(unclassifyed,tempClassifiedImage,pureImage);
	update();
	show();
	raise();
	activateWindow();
}
void ClassifyImageDialog::updateVSensitivity(const QString &str)
{
	vSensitivity=str.toInt();
	//////////////增加临时色表的处理
	GenTempClassifyImage(unclassifyed,tempClassifiedImage,pureImage);
	update();
	show();
	raise();
	activateWindow();
}

void ClassifyImageDialog::updateCurrentcolor(int index)
{
	currentColour=index+1;
	GenTempClassifyImage(unclassifyed,tempClassifiedImage,pureImage);
	update();
	show();
	raise();
	activateWindow();
}
void ClassifyImageDialog::updateHsiEnabled(bool HsiEnabled)
{
	hsiEnabled=HsiEnabled;
	if (!hsiEnabled)
	{
		int r=QMessageBox::warning(this,tr("HSI"),tr("the hsiLut has been modified.\n""Do you want to save hsiLut?"),QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
			if (r==QMessageBox::Yes)
			{
				QString filename=QFileDialog::getSaveFileName(this,tr("save HSI_LUT"),lastLutName,tr("Lut (*.dat)"));
				if (!filename.isEmpty())
				{

						saveLutString = filename;
						QFile f(saveLutString);
						f.open(QIODevice::WriteOnly);
						f.write((char*)HSI_lut, 64*64*64);
						f.close();
				}
				memset(HSI_lut,0,64*64*64);
			}
			else if (r==QMessageBox::No)
			{
				memset(HSI_lut,0,64*64*64);
			}
	}
}
void ClassifyImageDialog::updateHSIRange(const QString &str_rmin,const QString &str_rmax,const QString &str_omin,const QString &str_omax,const QString &str_ymin,const QString &str_ymax,const QString &str_fmin,const QString &str_fmax,const QString &str_cmin,const QString &str_cmax,const QString &str_wymin,const QString &str_wymax)
{
	r_min=str_rmin.toInt();
	r_max=str_rmax.toInt();
	o_min=str_omin.toInt();
	o_max=str_omax.toInt();
	y_min=str_ymin.toInt();
	y_max=str_ymax.toInt();
	f_min=str_fmin.toInt();
	f_max=str_fmax.toInt();
	c_min=str_cmin.toInt();
	c_max=str_cmax.toInt();
	w_min_Y=str_wymin.toInt();
	w_max_Y=str_wymax.toInt();
}
// void ClassifyImageDialog::update_r_min(const QString &str)
// {
// 	r_min=str.toInt();
// }
// void ClassifyImageDialog::update_r_max(const QString &str)
// {
// 	r_max=str.toInt();
// }
// void ClassifyImageDialog::update_o_min(const QString &str)
// {
// 	o_min=str.toInt();
// }
// void ClassifyImageDialog::update_o_max(const QString &str)
// {
// 	o_max=str.toInt();
// }
// void ClassifyImageDialog::update_y_min(const QString &str)
// {
// 	y_min=str.toInt();
// }
// void ClassifyImageDialog::update_y_max(const QString &str)
// {
// 	y_max=str.toInt();
// }
// void ClassifyImageDialog::update_f_min(const QString &str)
// {
// 	f_min=str.toInt();
// }
// void ClassifyImageDialog::update_f_max(const QString &str)
// {
// 	f_max=str.toInt();
// }
// void ClassifyImageDialog::update_c_min(const QString &str)
// {
// 	c_min=str.toInt();
// }
// void ClassifyImageDialog::update_c_max(const QString &str)
// {
// 	c_max=str.toInt();
// }
// void ClassifyImageDialog::update_w_min_Y(const QString &str)
// {
// 	w_min_Y=str.toInt();
// }
// void ClassifyImageDialog::update_w_max_Y(const QString &str)
// {
// 	w_max_Y=str.toInt();
// }
void ClassifyImageDialog::updateHotkey(bool Hotkeyenabled)
{
	hotkeyEnabled=Hotkeyenabled;
}
void ClassifyImageDialog::updateReplace(bool state)
{
	Replace = state;
}
void ClassifyImageDialog::updateDontReplace(bool state)
{
	DontReplace=state;
}
void ClassifyImageDialog::updateDelete(bool state)
{
	Delete=state;
}
void ClassifyImageDialog::LoadLUT(const QString& fileName)
{
	/*FILE* lutFile = fopen(fileName, "r"); // Open the file which contains the LUT for read access
	long lSize;

	if (lutFile==NULL) {
		QMessageBox::information(this,tr("lut"),tr("Unable to load lookup table!"));
		return;
	}

	// obtain file size.
	fseek (lutFile , 0 , SEEK_END);
	lSize = ftell (lutFile);
	rewind (lutFile);

	// copy the file into the LUT. this is the fast ver..
	fread (lut_,1,lSize,lutFile);

	// terminate
	fclose (lutFile);
*/
	QFile lutFile(fileName);
	if(!lutFile.open(QIODevice::ReadOnly))
	{
		QMessageBox::information(this,tr("lut"),tr("Unable to load lookup table!"));
		return;
	}
	long lSize = lutFile.size();
	lutFile.read((char*)lut_, lSize);
	lutFile.close();
}
void ClassifyImageDialog::LoadRawStream()
{
	if (imageBuffer != NULL) {
		delete[] imageBuffer;
		imageBuffer=NULL;
	}
	/*FILE* streamFile = fopen(rawString.c_str(),"rb");
	if (streamFile == NULL) {
		m_CurrentFileIndex=0;
		QMessageBox::information(this,tr("file"),tr("File not found"));
		return;
	}
	fseek(streamFile,0,SEEK_END);
	int filesize = ftell(streamFile);
	rewind(streamFile);
	imageBuffer=new char[filesize];
	if (imageBuffer == NULL) {
		imageBuffer = NULL;
		QMessageBox::information(this,tr("file"),tr("Not enough memory to load file"));
		return;
	}
	fread(imageBuffer,1,filesize,streamFile);
	fclose(streamFile);*/
	QFile file(rawString);
	if(!file.open(QIODevice::ReadOnly))
	{
		m_CurrentFileIndex=0;
		QMessageBox::information(this,tr("file"),tr("File not found"));
		return;
	}
	emit updateWindowTitle(tr("Classify Image    ").append(rawString));
	int filesize = file.size();
	imageBuffer=new char[filesize];
	if (imageBuffer == NULL) {
		imageBuffer = NULL;
		QMessageBox::information(this,tr("file"),tr("Not enough memory to load file"));
		return;
	}
	//QDataStream in(&file);
	//in >> imageBuffer;
	file.read(imageBuffer, filesize);
	file.close();
	//nao来了要去掉下面genTempImageBuffer（）函数和tempimageBuffer这个量，将两个memcpy（）中tempimageBuffer替换为imageBuffer
	//tempimageBuffer = new char[filesize];

	//genTempImageBuffer(tempimageBuffer,imageBuffer);
	memcpy(unclassifyed, imageBuffer, normalSize);
	//delete[] tempimageBuffer;
	if (hsiEnabled)
	{
		HSI_Process();
	}
}
void ClassifyImageDialog::genTempImageBuffer(char *to,char *from)
{
	for (int n=0;n<normalSize/3;n++)
	{
		to[n*3]=from[normalSize-n*3-2-1];
		to[n*3+1]=from[normalSize-n*3-1-1];
		to[n*3+2]=from[normalSize-n*3-1];
	}
}
void ClassifyImageDialog::paintEvent ( QPaintEvent * event ) 
{
	QPainter painter(this);
	painter.drawImage(QRect(0,0,FULL_IMAGE_WIDTH*uMultiply,FULL_IMAGE_HEIGHT*uMultiply),*unclassifiedImage,QRect(0,0,FULL_IMAGE_WIDTH,FULL_IMAGE_HEIGHT));
	painter.drawImage(QRect(FULL_IMAGE_WIDTH*uMultiply+hOffset,0,FULL_IMAGE_WIDTH*uMultiply,FULL_IMAGE_HEIGHT*uMultiply),*tempClassifiedImage,QRect(0,0,FULL_IMAGE_WIDTH,FULL_IMAGE_HEIGHT));
	painter.drawImage(QRect(0,FULL_IMAGE_HEIGHT*uMultiply+vOffset,FULL_IMAGE_WIDTH*cMultiply,FULL_IMAGE_HEIGHT*cMultiply),*classifiedImage,QRect(0,0,FULL_IMAGE_WIDTH,FULL_IMAGE_HEIGHT));

}
void ClassifyImageDialog::mousePressEvent(QMouseEvent * event)
{
	switch (event->button ())
	{
	case Qt::LeftButton:
		onLeftButton(event->x(),event->y());break;
	case Qt::RightButton:
		onRightButton(event->x(),event->y());break;
	default:break;
	}
}
void ClassifyImageDialog::onLeftButton(int pointX,int pointY)
{
	if (pointX>=0&&pointX<currentImageWidth_*uMultiply)  {
		if (pointY>=0&&pointY<currentImageHeight_*uMultiply)	  {
			int x = (int)((pointX - 0)/uMultiply);
			int y = (int)((pointY - 0)/uMultiply);
			currentpixelX = x;
			currentpixelY = y;
		}
	}
	if (pointX >= 0&& pointX <  currentImageWidth_*cMultiply) {
		if (pointY >= currentImageHeight_*uMultiply+vOffset && pointY < currentImageHeight_*uMultiply+vOffset+currentImageHeight_*cMultiply) {
			int x = (int)((pointX - 0)/cMultiply);
			int y = (int)((pointY - (currentImageHeight_*uMultiply+vOffset))/cMultiply);
			currentpixelX = x;
			currentpixelY = y;
		}
	}
	unsigned char* yuv = unclassifyed;

			currentY = yuv[(currentpixelX+currentpixelY*currentImageWidth_)*3];
			currentU = yuv[(currentpixelX+currentpixelY*currentImageWidth_)*3+1];
			currentV = yuv[(currentpixelX+currentpixelY*currentImageWidth_)*3+2];
			GenTempClassifyImage(unclassifyed,tempClassifiedImage,pureImage);
			update();
			show();
			raise();
			activateWindow();


}
void ClassifyImageDialog::onRightButton(int pointX,int pointY)
{
	bool insideClassifyArea=false;
	if (pointX>=0&&pointX<currentImageWidth_*uMultiply)  {
		if (pointY>=0&&pointY<currentImageHeight_*uMultiply)	  {
			insideClassifyArea=true;
		}
	}

	if (pointX >= 0&& pointX <  currentImageWidth_*cMultiply) {
		if (pointY >= currentImageHeight_*uMultiply+vOffset && pointY < currentImageHeight_*uMultiply+vOffset+currentImageHeight_*cMultiply) {
			insideClassifyArea =  true;
		}
	}

	if(insideClassifyArea)
	{
		memcpy(undoLut,lut_,64*64*64);
		emit enableUnDo(true);
		for (int y = 0; y < FULL_IMAGE_HEIGHT; y++) 
		{
			for (int x = 0; x < FULL_IMAGE_WIDTH; x++) 
			{
				int yCol = unclassifyed[(x+y*currentImageWidth_)*3];
				int uCol = unclassifyed[(x+y*currentImageWidth_)*3+1];
				int vCol = unclassifyed[(x+y*currentImageWidth_)*3+2];
				if (abs(currentY-yCol) < ySensitivity && abs(currentU - uCol) <uSensitivity && abs(currentV - vCol) < vSensitivity) 
				{
					int index = (yCol >> 2 << 12)+(uCol >> 2 << 6)+(vCol >> 2); 
					if ((lut_[index] == c_UNKNOWN && DontReplace) || Replace) 
						lut_[index] = currentColour;
					if (Delete)
						lut_[index] = c_UNKNOWN;
				}
			}
		}
	}
	GenClassifyImage(lut_,unclassifyed,classifiedImage);
	update();
	show();
	raise();
	activateWindow();
}
void ClassifyImageDialog::activateWindow()
{
	emit activeThisWindow();
}

void ClassifyImageDialog::keyPressEvent(QKeyEvent * event)
{
	if (hotkeyEnabled)
	{
		switch (event->key())
		{
		case Qt::Key_F://FIELD
			emit sendCurrentColorToToolBox(c_FIELD_GREEN-1);break;
		case Qt::Key_W://WHITE
			emit sendCurrentColorToToolBox(c_WHITE-1);break;
		case Qt::Key_O://ORANGE
			emit sendCurrentColorToToolBox(c_BALL_ORANGE-1);break;
		case Qt::Key_R://RED
			emit sendCurrentColorToToolBox(c_ROBOT_RED-1);break;
		case Qt::Key_B://ROBOTBLUER
			emit sendCurrentColorToToolBox(c_ROBOT_BLUE-1);break;
		case Qt::Key_G://GRAY
			emit sendCurrentColorToToolBox(c_BEACON_GREEN-1);break;
		case Qt::Key_C://CYAN
			emit sendCurrentColorToToolBox(c_BEACON_BLUE-1);break;
		case Qt::Key_P://PINK
			emit sendCurrentColorToToolBox(c_BEACON_PINK-1);break;
		case Qt::Key_Y://YELLOW
			emit sendCurrentColorToToolBox(c_BEACON_YELLOW-1);break;
		default:
			break;			
		}
	}
}
void ClassifyImageDialog::GenTempClassifyImage(unsigned char *buffer,QImage *ToImage,QImage *FromImage)
{
	(*ToImage)=(*FromImage);
	for (int y=0;y<FULL_IMAGE_HEIGHT; y++)
	{
		for(int x = 0; x < FULL_IMAGE_WIDTH; x++)
		{
			int yCol = buffer[(x+y*currentImageWidth_)*3];
			int uCol = buffer[(x+y*currentImageWidth_)*3+1];
			int vCol = buffer[(x+y*currentImageWidth_)*3+2];
			if (abs(currentY-yCol) < ySensitivity && abs(currentU - uCol) <uSensitivity && abs(currentV - vCol) < vSensitivity) 
				ToImage->setPixel(x,y,LookupRgb(currentColour));
			else
				ToImage->setPixel(x,y,LookupRgb(c_UNKNOWN));
		}
	}

}
void ClassifyImageDialog::GenClassifyImage(unsigned char *lut,unsigned char *buffer,QImage* image)
{
	unsigned char* lutFile;
	lutFile = lut;
	unsigned char* y = buffer;
	unsigned char* u = buffer+1;
	unsigned char* v = buffer+2;

	int h = 0;
	int w = 0;
	while (h < FULL_IMAGE_HEIGHT) {
		w = 0;
		while (w < FULL_IMAGE_WIDTH) {
			int offset = (*y >> 2 << 12)+(*u>> 2 << 6)+(*v>> 2);
			int temp=*(lutFile+offset);
			image->setPixel(w,h,LookupRgb(temp));

			w++;
			y+=3;
			u+=3;
			v+=3;
		}
		h++;
	}

}
void ClassifyImageDialog::GenUnclassifyImage(unsigned char *buffer,QImage* image)
{
	QRgb value;
	unsigned char Y,Cb,Cr;
	int r,g,b;
	int index=0;
	for (int j=0;j<FULL_IMAGE_HEIGHT;j++){
		for (int i=0;i<FULL_IMAGE_WIDTH;i++){

			Y=buffer[index++];
			Cb=buffer[index++];
			Cr=buffer[index++];
			ClassifyImageDialog::ConvertYCbCrToRGB(Y,Cb,Cr,r,g,b);
			value = qRgb(r, g, b);
			image->setPixel(i,j,value);
		}
	}
}
QRgb ClassifyImageDialog::LookupRgb(int index)
{
	switch (index)
	{
	case c_UNKNOWN:
		return qRgb(0,0,0);break;
	case c_FIELD_GREEN:
		return qRgb(0,255,0);break;
	case c_WHITE:
		return qRgb(255,255,255);break;
	case c_BALL_ORANGE:
		return qRgb(220,150,0);break;
	case c_ROBOT_RED:
		return qRgb(255,0,0);break;
	case c_ROBOT_BLUE:
		return qRgb(50,0,160);break;
	case c_BEACON_GREEN:
		return qRgb(128,128,128);break;
	case c_BEACON_BLUE:
		return qRgb(57,134,210);break;
	case c_BEACON_PINK:
		return qRgb(220,60,220);break;
	case c_BEACON_YELLOW:
		return qRgb(245,235,50);break;
	default :
		break;
	}
	return qRgb(0,0,0);
}
void ClassifyImageDialog::ConvertYCbCrToRGB(unsigned char Y, unsigned char Cb, unsigned char Cr,int &finalR,int &finalG,int &finalB) 
{
	///////////////GT,it is right///////////////////////////
	int r = Y + ((1436 * (Cr - 128))>> 10),
		g = Y - ((354 * (Cb - 128) + 732 * (Cr - 128))>> 10),
		b = Y + ((1814 * (Cb - 128))>> 10);
	if(r < 0) r = 0; else if(r > 255) r = 255;
	if(g < 0) g = 0; else if(g > 255) g = 255;
	if(b < 0) b = 0; else if(b > 255) b = 255;
	finalR=r;
	finalG=g;
	finalB=b;
	//////////////////////////////////
	///////////sony,it is not right/////////////
	// 	int r,g,b;
	// 	//RGBColour ResultRGB;
	// 	double R = 255.0 * (Y + Cb);
	// 	double G = 255.0 * (Y - 0.51*Cb - 0.19*Cr);
	// 	double B = 255.0 * (Y + Cr);
	// 	r = ( int)R;
	// 	if (R > 255.0) {
	// 		r = 255;
	// 	} else if (R < 0.0) {
	// 		r = 0;
	// 	}
	// 	g = (int)G;
	// 	if (G > 255.0) {
	// 		g = 255;
	// 	} else if (G < 0.0) {
	// 		g = 0;
	// 	}
	// 	b = (int)B;
	// 	if (B > 255.0) {
	// 		b = 255;
	// 	} else if (B < 0.0) {
	// 		b = 0;
	// 	}
	// 	finalR=r;
	// 	finalG=g;
	// 	finalB=b;
}
void ClassifyImageDialog::ConverRGBtoHSI(int R, int G, int B, double& H, double& S, double& I)
{
	double sum1;
	sum1= double(R+G+B);
	double r,g,b,min3;
	r=(double)R/sum1;
	g=(double)G/sum1;
	b=(double)B/sum1;
	min3=std::min(std::min(r,g),b);
	I=sum1/(3.0*255.0);
	if (I==0)
	{
		S=0;
		H=0;
	}
	else
	{
		S=1-3*min3/(r+g+b);
		if (S==0)
			H=0;
		else
		{
			double temp_h=acos((2.0*r-g-b)/(2.0*sqrt((r-g)*(r-g)+(r-b)*(g-b))));
			if (b<=g)
				H=temp_h;
			else
				H=2*PI-temp_h;
		}	

	}
	//此时H为弧度值，要化成0到255的数
	H=H/2/PI*255-120/360*255;//此时H为标准的0到255的数了
	if (H<=0)
		H=H+256;


}
void ClassifyImageDialog::HSI_Process()
{
	int pixelX,pixelY;
	unsigned char* yuv = unclassifyed;
	unsigned char y,cb,cr;
	int r,g,b;
	double currentH,currentS,currentI;
	for (pixelY = 0; pixelY < FULL_IMAGE_HEIGHT; pixelY++) {
		for (pixelX = 0; pixelX < FULL_IMAGE_WIDTH; pixelX++) {
			y = yuv[(pixelX+pixelY*currentImageWidth_)*3];
			cb =yuv[(pixelX+pixelY*currentImageWidth_)*3+1];
			cr = yuv[(pixelX+pixelY*currentImageWidth_)*3+2];
			ConvertYCbCrToRGB(y,cb,cr,r,g,b);
			ConverRGBtoHSI(r,g,b,currentH,currentS,currentI);//此时H为弧度值，为化成0到255的数
			int index = (y >> 2 << 12)+(cb >> 2 << 6)+(cr >> 2); 
			//判断HSI是否附和某一个高斯，给定颜色值,参考BOTTONRdown的写法
			if (currentH>=r_min && currentH<=r_max)
				HSI_lut[index] = c_ROBOT_RED;
			else if(currentH>o_min && currentH<=o_max)
				HSI_lut[index] = c_BALL_ORANGE;
			else if(currentH>y_min && currentH<=y_max)
				HSI_lut[index] = c_BEACON_YELLOW;
			else if (currentH>f_min && currentH<=c_max
				&& currentY>w_min_Y)//加入Y值判断，区分蓝色白色绿色
				HSI_lut[index] = c_WHITE;
			else if(currentH>f_min && currentH<=f_max)
				HSI_lut[index] = c_FIELD_GREEN;
			else if(currentH>c_min && currentH<=c_max)
				HSI_lut[index] = c_BEACON_BLUE;
			else
				HSI_lut[index] = c_UNKNOWN;
		}
	}
}
