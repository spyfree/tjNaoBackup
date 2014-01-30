#ifndef GraphView_H
#define GraphView_H
#include <QSvgRenderer>
#include <QtGui>
class SvgWidget:public QWidget
{
	Q_OBJECT
public:
	SvgWidget(const QString &file,QWidget *parent=0);
	virtual QSize sizeHint() const;
	protected slots:
		void poluteImage();
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void wheelEvent(QWheelEvent *event);

private:
	QSvgRenderer *doc;
};
class SvgWindow: public QScrollArea
{
public :
	SvgWindow();
	void openFile(const QString &file);
	void setRenderer();
	QString currentPath;
	QHBoxLayout *layout;
};
#endif