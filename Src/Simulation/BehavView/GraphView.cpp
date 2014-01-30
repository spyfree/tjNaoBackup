#include "GraphView.h"

SvgWidget::SvgWidget(const QString &file ,QWidget *parent/* =0 */):QWidget(parent)
{
	doc = new QSvgRenderer(file, this);
	connect(doc, SIGNAL(repaintNeeded()),
		this, SLOT(poluteImage()));
}

void SvgWidget::paintEvent(QPaintEvent *)
{

	QPainter p(this);
	//QBrush brush(Qt::SolidPattern);
	//QColor color(0,0,0);
	//brush.setColor(color);
	//p.setBackground(brush);
	p.setViewport(0, 0, width(), height());
	doc->render(&p);
}

QSize SvgWidget::sizeHint() const
{
	if (doc)
		return doc->defaultSize();
	return QWidget::sizeHint();
}

void SvgWidget::poluteImage()
{
	update();
}

void SvgWidget::wheelEvent(QWheelEvent *e)
{
	const double diff = 0.1;
	QSize size = doc->defaultSize();
	int width  = size.width();
	int height = size.height();
	if (e->delta() > 0) {
		width = int(this->width()+this->width()*diff);
		height = int(this->height()+this->height()*diff);
	} else {
		width  = int(this->width()-this->width()*diff);
		height = int(this->height()-this->height()*diff);
	}

	resize(width, height);
}

SvgWindow::SvgWindow()
{
	/*setMinimumSize(500,500);*/
	QWidget *view = new QWidget(this);
	layout=new QHBoxLayout;
	setWidget(view);
}


void SvgWindow::openFile(const QString &file)
{
	currentPath=file;
	setRenderer();

}

void SvgWindow::setRenderer()
{

	QWidget *view;
	view = new SvgWidget(currentPath, this);
	setWidget(view);
	view->show();
}