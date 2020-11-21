#include <QtGui/QPainter>
#include "customqwidget.h"

CustomQWidget::CustomQWidget(QWidget* parent) : QWidget(parent){

}

void CustomQWidget::paintEvent(QPaintEvent* event) {
	qDebug("called");
	QPainter painter(this);
	painter.setPen(Qt::blue);
	painter.setFont(QFont("Roboto", 30));
	painter.drawText(rect(), Qt::AlignCenter, "Qt");
}

void CustomQWidget::mouseMoveEvent(QMouseEvent* event) {
	printf("moving\n");
}

void CustomQWidget::mouseReleaseEvent(QMouseEvent* event) {
	update();
}

CustomQWidget::~CustomQWidget() = default;
