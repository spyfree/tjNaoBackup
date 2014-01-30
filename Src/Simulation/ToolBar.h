#ifndef TOOL_BAR
#define TOOL_BAR
#include "MenuBar.h"
#include <QToolBar>
#include <QMenu>

class ToolBar:public QToolBar
{
public:
	ToolBar(QWidget *parent=0);
	
private:
	void CreatAction();
	QMenu* netMenu;
	QMenu* simMenu;
};
#endif