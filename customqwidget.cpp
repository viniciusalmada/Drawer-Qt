#include <QtGui/QPainter>
#include <QtGui/QResizeEvent>
#include "customqwidget.h"

CustomQWidget::CustomQWidget(QWidget* parent) : QWidget(parent) {
	mPt0.setX(0);
	mPt0.setY(0);
	mPt1.setX(0);
	mPt1.setY(0);
	mButtonPressed = false;
	
	QPalette pal = palette();
	pal.setColor(QPalette::Background, Qt::white);
	setAutoFillBackground(true);
	setPalette(pal);
}

void CustomQWidget::resizeEvent(QResizeEvent* event) {
	this->mW = event->size().width();
	this->mH = event->size().height();
	
	mWindowStart = 0.0;
	mWindowWidth = mW / 10;
	mWindowBottom = 0.0;
	mWindowHeight = mH / 10;
}

void CustomQWidget::paintEvent(QPaintEvent* event) {
	qDebug("called");
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	painter.setViewport(0, 0, mW, mH);
	painter.setWindow(mWindowStart, mWindowBottom, mWindowWidth, mWindowHeight);
	painter.setPen(Qt::red);
	QPointF pt0U = convertPtCoordsToUniverse(mPt0);
	QPointF pt1U = convertPtCoordsToUniverse(mPt1);
	
	painter.drawLine(pt0U, pt1U);
}

void CustomQWidget::mouseMoveEvent(QMouseEvent* event) {
	if (!mButtonPressed) return;
	mPt1 = event->pos();
	update();
}

void CustomQWidget::mouseReleaseEvent(QMouseEvent* event) {
	mButtonPressed = false;
	mPt1 = event->pos();
	update();
}

void CustomQWidget::mousePressEvent(QMouseEvent* event) {
	mPt0 = mPt1 = event->pos();
	mButtonPressed = true;
}

QPointF CustomQWidget::convertPtCoordsToUniverse(QPoint pt) const {
	double mX = (double) pt.x() * (double) mWindowWidth / (double) mW;
	double mY = (double) pt.y() * (double) mWindowHeight / (double) mH;
	double x = mWindowStart + mX;
	double y = mWindowBottom + mY;
	return {x, y};
}

CustomQWidget::~CustomQWidget() = default;
