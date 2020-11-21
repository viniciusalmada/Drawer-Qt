//
// Created by vini on 11/20/20.
//

#include <QtGui/QPainter>
#include "customqwidget.h"

CustomQWidget::CustomQWidget(QWidget* parent) : QWidget(parent){

}

void CustomQWidget::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.setPen(Qt::blue);
	painter.setFont(QFont("Roboto", 30));
	painter.drawText(rect(), Qt::AlignCenter, "Qt");
}

CustomQWidget::~CustomQWidget() = default;
