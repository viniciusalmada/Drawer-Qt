//
// Created by vini on 11/20/20.
//

#ifndef DRAWERQTPAINTER_CUSTOMQWIDGET_H
#define DRAWERQTPAINTER_CUSTOMQWIDGET_H


#include <QtWidgets/QWidget>

class CustomQWidget : public QWidget {
Q_OBJECT
public:
	explicit CustomQWidget(QWidget* parent);
	
	~CustomQWidget() override;

protected:
	void paintEvent(QPaintEvent* event) override;
	
	void mouseMoveEvent(QMouseEvent* event) override;
	
	void mouseReleaseEvent(QMouseEvent* event) override;
	
};


#endif //DRAWERQTPAINTER_CUSTOMQWIDGET_H
