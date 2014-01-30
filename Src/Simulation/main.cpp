#include "mainwindow.h"
#include <QApplication>
int main(int argc,char *argv[])
{
	Q_INIT_RESOURCE(Simulation);
	QApplication app(argc,argv); 
	MainWindow mainwindow;
	//mainwindow.show();		
	mainwindow.showMaximized();
	return app.exec();
}
