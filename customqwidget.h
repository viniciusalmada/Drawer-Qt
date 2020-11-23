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
	int mW;
	int mH;
	int mWindowStart;
	int mWindowWidth;
	int mWindowBottom;
	int mWindowHeight;
	bool mButtonPressed;
	
	QPoint mPt0;
	QPoint mPt1;
	
	QPointF convertPtCoordsToUniverse(QPoint pt) const;
	
	void resizeEvent(QResizeEvent* event) override;
	
	void paintEvent(QPaintEvent* event) override;
	
	void mousePressEvent(QMouseEvent* event) override;
	
	void mouseMoveEvent(QMouseEvent* event) override;
	
	void mouseReleaseEvent(QMouseEvent* event) override;
	
};


#endif //DRAWERQTPAINTER_CUSTOMQWIDGET_H
