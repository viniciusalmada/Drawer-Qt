#include <QtGui/QPainter>
#include "customqwidget.h"

CustomQWidget::CustomQWidget(QWidget* parent) : QWidget(parent){

}

void CustomQWidget::paintEvent(QPaintEvent* event) {
	qDebug("called");
	QPainter painter(this);
	painter.setPen(Qt::blue);
	int side = qMin(width(), height());
	int x = width() - side / 2;
	int y = height() - side / 2;
	painter.setViewport(x, y, side, side);
	painter.drawLine(0, 0, 50, 0);
	painter.setWindow(QRect(-50, -50, 100, 100));
	
	painter.setFont(QFont("Roboto", 30));
	painter.drawText(0, 0, "Qt");
}

void CustomQWidget::mouseMoveEvent(QMouseEvent* event) {
	printf("moving\n");
}

void CustomQWidget::mouseReleaseEvent(QMouseEvent* event) {
	update();
}

CustomQWidget::~CustomQWidget() = default;
