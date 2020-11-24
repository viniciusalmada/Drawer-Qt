#include <QtGui/QPainter>
#include <QtGui/QResizeEvent>
#include "canvasqwidget.h"

CanvasQWidget::CanvasQWidget(QWidget* parent) : QWidget(parent) {
	QWidget::setMouseTracking(true);
	QWidget::setFocusPolicy(Qt::ClickFocus);
	
	QPalette pal = palette();
	pal.setColor(QPalette::Background, Qt::white);
	setAutoFillBackground(true);
	setPalette(pal);
}

void CanvasQWidget::resizeEvent(QResizeEvent* event) {
	this->mW = event->size().width();
	this->mH = event->size().height();
	
	if (mModel != nullptr && !mModel->isEmpty()) {
		mWindowsBox = mModel->boundingBox();
		scaleWorldWindow(1.10);
	} else {
		scaleWorldWindow(1.0);
	}
}

void CanvasQWidget::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	painter.setViewport(0, 0, mW, mH);
	painter.setWindow(mWindowsBox.xMin, mWindowsBox.yMin, mWindowsBox.width(), mWindowsBox.height());
	
	if (mModel != nullptr && !mModel->isEmpty()) {
		makeDisplayModel(painter);
	}
	
	if (mViewGrid) {
		makeDisplayGrid(painter);
	}
	
	switch (mCurrentAction) {
		
		case ActionType::SELECTION:
			drawSelectionFence(painter);
			break;
		case ActionType::COLLECTION:
			drawCollectedCurve(painter);
			break;
		default:
			break;
	}
	painter.restore();
}

void CanvasQWidget::mouseMoveEvent(QMouseEvent* event) {
	mPt1 = event->pos();
	
	switch (mCurrentAction) {
		
		default:
			break;
		case ActionType::SELECTION:
			if (mMouseButton == Qt::LeftButton && mButtonPressed)
				update();
			break;
		case ActionType::COLLECTION:
			if (mMouseButton == Qt::LeftButton && !mButtonPressed) {
				if (PointUtils::dist(mPt0, mPt1) <= mMouseMoveTol)
					break;
				
				if (mCollector.isCollecting()) {
					QPointF pt1 = convertPtCoordsToUniverse(mPt1);
					if (mViewGrid) {
						bool isSnap = mGrid.getSnapInfo();
						if (!mControlKeyPressed && isSnap ||
						    mControlKeyPressed && !isSnap) {
							mGrid.snapTo(pt1);
						}
					}
					
					if (mModel != nullptr && !mModel->isEmpty()) {
						double max = mWindowsBox.max();
						double pickTol = max * mPickTolFactor;
						mModel->snapToCurve(pt1, pickTol);
					}
				}
			}
			break;
	}
	
	update();
}

void CanvasQWidget::mouseReleaseEvent(QMouseEvent* event) {
	mButtonPressed = false;
	mPt1 = event->pos();
	update();
}

void CanvasQWidget::mousePressEvent(QMouseEvent* event) {
	mButtonPressed = true;
	
	mPt0 = event->pos();
	mMouseButton = event->button();
	switch (mCurrentAction) {
		
		case ActionType::UNDEF:
			break;
		case ActionType::SELECTION:
			break;
		case ActionType::COLLECTION:
			if (!mCollector.isActive()) {
				if (mMouseButton == Qt::LeftButton)
					mCollector.startCollection();
			}
			break;
	}
}

QPointF CanvasQWidget::convertPtCoordsToUniverse(QPointF pt) const {
	double mX = (double) pt.x() * (double) mWindowsBox.width() / (double) mW;
	double mY = (double) pt.y() * (double) mWindowsBox.height() / (double) mH;
	double x = mWindowsBox.xMin + mX;
	double y = mWindowsBox.yMin + mY;
	return {x, y};
}

void CanvasQWidget::setModel(Model* model) {
	mModel = model;
}

void CanvasQWidget::resetModelDisplay() {
	if (mModel == nullptr)
		return;
	update();
}

void CanvasQWidget::setMouseAction(ActionType action) {
	if (mCurrentAction == action)
		return;
	
	mCurrentAction = action;
	if (action == ActionType::SELECTION)
		mCollector.reset();
	
	update();
}

void CanvasQWidget::setCurveType(CurveType type) {
	if (mCurrentAction == ActionType::COLLECTION && mCollector.getCurveType() == type)
		return;
	
	mCollector.reset();
	mCollector.setCurveType(type);
	update();
}

void CanvasQWidget::setGridSnapData(bool viewGrid, bool isSnapOn, double dx, double dy) {
	mGrid.setSnapData(isSnapOn, dx, dy);
	mViewGrid = viewGrid;
	update();
}

bool CanvasQWidget::getGridSnapInfo(double* dx, double* dy) {
	auto space = mGrid.getGridSpace();
	*dx = space.first;
	*dy = space.second;
	return mGrid.getSnapInfo();
}

void CanvasQWidget::delSelectedEntities() {
	if (mModel != nullptr && mModel->isEmpty()) {
		mModel->delSelectedCurves();
		update();
	}
	
}

void CanvasQWidget::makeDisplayModel(QPainter& painter) {
	painter.setPen(mPen);
	std::vector<Curve*> curves = mModel->curves();
	mPen.setWidthF(1.0);
	for (Curve* c : curves) {
		QVector<QPointF> pts = c->getPointsToDraw();
		if (c->isSelected())
			mPen.setBrush(mSelectionColor);
		else
			mPen.setBrush(mCurveColor);
		
		painter.drawLines(pts);
		
		mPen.setWidthF(5.0);
		mPen.setBrush(mVertexColor);
		painter.drawPoint(pts.first());
		painter.drawPoint(pts.last());
	}
}

void CanvasQWidget::drawSelectionFence(QPainter& painter) {
	if (!mButtonPressed)
		return;
	
	if (mPt0 == mPt1)
		return;
	
	if (mModel == nullptr || mModel->isEmpty())
		return;
	
	painter.setPen(mPen);
	mPen.setWidthF(1.0);
	mPen.setBrush(mSelectionColor);
	QPointF pt0 = convertPtCoordsToUniverse(mPt0);
	QPointF pt1 = convertPtCoordsToUniverse(mPt1);
	QRectF fence{pt0, pt1};
	
	painter.drawRect(fence);
}

void CanvasQWidget::drawCollectedCurve(QPainter& painter) {
	if (!mCollector.isActive() || !mCollector.isCollecting())
		return;
	
	painter.setPen(mPen);
	mPen.setWidthF(0.5);
	mPen.setBrush(mCollectingColor);
	
	QVector<QPointF> pts = mCollector.getDrawPoints();
	painter.drawLines(pts);
	
	mPen.setWidthF(5.0);
	QVector<QPointF> ctrlPts = mCollector.getPoints();
	painter.drawPoints(ctrlPts);
}

void CanvasQWidget::scaleWorldWindow(double scaleFactor) {
	double vpr = (double) mH / (double) mW;
	
	double cx = mWindowsBox.centerX();
	double cy = mWindowsBox.centerY();
	
	double sizeX = mWindowsBox.width() * scaleFactor;
	double sizeY = mWindowsBox.height() * scaleFactor;
	
	if (vpr < sizeY / sizeX) {
		mWindowsBox.xMin = cx - (sizeY / vpr) / 2.0;
		mWindowsBox.xMax = cx + (sizeY / vpr) / 2.0;
		mWindowsBox.yMin = cy - (sizeY / 2.0);
		mWindowsBox.yMax = cy + (sizeY / 2.0);
	} else {
		mWindowsBox.xMin = cx - sizeX / 2.0;
		mWindowsBox.xMax = cx + sizeX / 2.0;
		mWindowsBox.yMin = cy - (sizeX * vpr) / 2.0;
		mWindowsBox.yMax = cy + (sizeX * vpr) / 2.0;
	}
}

void CanvasQWidget::makeDisplayGrid(QPainter& painter) {
	std::pair<double, double> gridSpace = mGrid.getGridSpace();
	double gX = gridSpace.first;
	double gY = gridSpace.second;
	mPen.setBrush(mGridColor);
	mPen.setWidthF(1.5);
	double x = -((int) (-mWindowsBox.xMin / gX) * gX) - gX;
	while (x <= mWindowsBox.xMax) {
		double y = -((int) (-mWindowsBox.yMin / gY) * gY) - gY;
		while (y <= mWindowsBox.yMax) {
			painter.drawPoint(QPointF{x, y});
			y += gY;
		}
		x += gX;
	}
}

void CanvasQWidget::keyPressEvent(QKeyEvent* event) {
	QWidget::keyPressEvent(event);
}

CanvasQWidget::~CanvasQWidget() = default;

void CanvasQWidget::keyReleaseEvent(QKeyEvent* event) {
	QWidget::keyReleaseEvent(event);
}
