#ifndef DRAWERQTPAINTER_CANVAS_QWIDGET_H
#define DRAWERQTPAINTER_CANVAS_QWIDGET_H

#include <QtWidgets/QWidget>
#include <QPen>
#include "manager/model.h"
#include "manager/curvecollector.h"
#include "utils/grid.h"
#include "utils/box.h"

enum class ActionType {
	UNDEF, SELECTION, COLLECTION
};

class CanvasQWidget : public QWidget {
Q_OBJECT
public:
	explicit CanvasQWidget(QWidget* parent);
	
	~CanvasQWidget() override;
	
	void setModel(Model* model);
	
	Model* getModel() { return mModel; };
	
	void resetModelDisplay();
	
	void setMouseAction(ActionType action);
	
	void setCurveType(CurveType type);
	
	void setGridSnapData(bool viewGrid, bool isSnapOn,
	                     double dx, double dy);
	
	bool getGridSnapInfo(double* dx, double* dy);
	
	void delSelectedEntities();
	
	void fitWorldToViewport();
	
	void zoomIn();
	
	void zoomOut();

protected:
	int mW = 0;
	int mH = 0;
	RectUtils::RectF mWindowsBox{-1.0, -1.0, 11.0, 11.0};
	bool mButtonPressed = false;
	
	QPointF mPt0{};
	QPointF mPt1{};
	
	Model* mModel = nullptr;
	CurveCollector* mCollector;
	Grid mGrid{};
	
	QBrush mBackgroundColor = Qt::white;
	QBrush mCollectingColor = Qt::darkMagenta;
	QBrush mCurveColor = Qt::darkYellow;
	QBrush mVertexColor = Qt::darkCyan;
	QBrush mSelectionColor = Qt::red;
	QBrush mGridColor = Qt::black;
	
	bool mViewGrid = true;
	
	ActionType mCurrentAction = ActionType::SELECTION;
	Qt::MouseButton mMouseButton = Qt::NoButton;
	
	double mPickTolFactor = 0.01;
	int mMouseMoveTol = 2;
	
	bool mShiftKeyPressed = false;
	bool mControlKeyPressed = false;
	
	QPen mPen{mCurveColor, 1.0};
	
	void resizeEvent(QResizeEvent* event) override;
	
	void paintEvent(QPaintEvent* event) override;
	
	void mousePressEvent(QMouseEvent* event) override;
	
	void mouseMoveEvent(QMouseEvent* event) override;
	
	void mouseReleaseEvent(QMouseEvent* event) override;
	
	void keyPressEvent(QKeyEvent* event) override;
	
	void keyReleaseEvent(QKeyEvent* event) override;
	
	QPointF convertPtCoordsToUniverse(QPointF pt) const;
	
	void makeDisplayGrid(QPainter& painter);
	
	void makeDisplayModel(QPainter& painter);
	
	void drawSelectionFence(QPainter& painter);
	
	void drawCollectedCurve(QPainter& painter);
	
	void scaleWorldWindow(double scaleFactor);
	
	void panWorldWindow(double panFacX, double panPacY);
	
	QTransform mTransform;
};


#endif //DRAWERQTPAINTER_CANVASQWIDGET_H
