#ifndef DRAWERQTPAINTER_CANVAS_QWIDGET_H
#define DRAWERQTPAINTER_CANVAS_QWIDGET_H


#include <QtWidgets/QWidget>

class CanvasQWidget : public QWidget {
Q_OBJECT
public:
	explicit CanvasQWidget(QWidget* parent);
	
	~CanvasQWidget() override;

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


#endif //DRAWERQTPAINTER_CANVASQWIDGET_H
