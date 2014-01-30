#include "mainWindow.h"
#include <QApplication>
int main(int argc,char *argv[])
{
	QApplication app(argc,argv); 
	mainWindow mainwindow;
	mainwindow.show();		
	return app.exec();
}
