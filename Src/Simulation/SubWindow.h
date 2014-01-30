/** 
* @file SimRobotGUI/SubWindow.h
* Declaration of class SubWindow.
* @author Colin Graf
*/

#ifndef SubWindow_H
#define SubWindow_H

#include <QMdiSubWindow>
class QMdiArea;

class SubWindow : public QMdiSubWindow
{
  Q_OBJECT

public:
  SubWindow(QMdiArea* mdiArea, const QString& object);

  virtual void updateObject() {};
  const QString& getObject() const;

  void show();
  bool close();
  void setWidget(QWidget* widget);

  virtual void connectMenuBar() {};
  virtual void disconnectMenuBar() {};

protected:
  QString object;

  virtual void restoreLayout() {};
  virtual void writeLayout() {};


 

  virtual void closeEvent(QCloseEvent* event);
  virtual void moveEvent(QMoveEvent* event);
  virtual void resizeEvent(QResizeEvent* event);

private:
  bool layoutWritten;
  bool layoutRestored;
  QRect normalGeometry; /**< The restored or last recoginzed, not maximized or minimized window geometry. */
};

#endif // SubWindow_H
