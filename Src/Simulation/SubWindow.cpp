
#include <QMdiArea>
#include <QCloseEvent>
#include <QLayout>
#include "SubWindow.h"

SubWindow::SubWindow(QMdiArea* mdiArea, const QString& object) : QMdiSubWindow(mdiArea),
  object(object), layoutWritten(false), layoutRestored(false) 
{
  //setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(object);
	setWindowIcon(QIcon("icons/No.png"));
}

void SubWindow::show()
{
  if(!layoutRestored)
  {
    layoutRestored = true;
    restoreLayout();
  }
  QMdiSubWindow::show();
}

bool SubWindow::close()
{
  if(layoutWritten)
    return false;
  layoutWritten = true;
  writeLayout();
  return QMdiSubWindow::close();
}

void SubWindow::closeEvent(QCloseEvent* event)
{
  if(!layoutWritten)
  {
    layoutWritten = true;
    writeLayout();
    //Document::document->closeObject(object);
  }
  QMdiSubWindow::closeEvent(event);
}

const QString& SubWindow::getObject() const
{
  return object;
}



void SubWindow::moveEvent(QMoveEvent* event)
{
  if(!(windowState() & (Qt::WindowMinimized | Qt::WindowMaximized | Qt::WindowFullScreen)))
    normalGeometry = geometry();
  
  QMdiSubWindow::moveEvent(event);
}

void SubWindow::resizeEvent(QResizeEvent* event)
{
  if(!(windowState() & (Qt::WindowMinimized | Qt::WindowMaximized | Qt::WindowFullScreen)))
    normalGeometry = geometry();

  QMdiSubWindow::resizeEvent(event);
}

void SubWindow::setWidget(QWidget* widget)
{
  if(widget && isVisible())
  { 
    widget->setParent(this);
    widget->resize(0, 0);
    widget->setVisible(true);
  }
  QMdiSubWindow::setWidget(widget);
}
