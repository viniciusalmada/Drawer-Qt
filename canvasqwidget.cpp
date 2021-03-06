#include <QtGui/QPainter>
#include <QtGui/QResizeEvent>
#include <geom/geomutils.h>
#include "canvasqwidget.h"

CanvasQWidget::CanvasQWidget(QWidget* parent) : QWidget(parent) {
	mCollector = CurveCollector::getInstance();
	
	QWidget::setMouseTracking(true);
	QWidget::setFocusPolicy(Qt::ClickFocus);
	
	QPalette pal = palette();
	pal.setColor(QPalette::Background, mBackgroundColor.color());
	setAutoFillBackground(true);
	setPalette(pal);
}

void CanvasQWidget::resizeEvent(QResizeEvent* event) {
	this->mW = event->size().width();
	this->mH = event->size().height();
	
	if (mModel != nullptr && !mModel->isEmpty()) {
		mWindowsBox = RectUtils::RectF(mModel->boundingBox());
		scaleWorldWindow(1.10);
	} else {
		mWindowsBox = RectUtils::RectF(-1.0, -1.0, 11, 11);
		scaleWorldWindow(1.0);
	}
}

/**
 * The transformation matrix follows:
 * x' = m11 * x + m21 * y + m31
 * y' = m12 * x + m22 * y + m32
 */
void CanvasQWidget::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	painter.setViewport(0, 0, mW, mH);
	qreal rlw = mWindowsBox.getWidth() / (double) mW;
	qreal bth = -mWindowsBox.getHeight() / (double) mH;
	mTransform = QTransform{};
	mTransform.setMatrix(
			rlw, 0, 0,
			0, bth, 0,
			mWindowsBox.getLeft(), mWindowsBox.getTop(), 1);
	mTransform = mTransform.inverted();
	
	painter.setWorldTransform(mTransform);
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
	
	showCoordinates(convertPtCoordsToUniverse(mPt1));
	
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
					return;
				
				if (mCollector->isCollecting()) {
					QPointF pt1 = convertPtCoordsToUniverse(mPt1);
					if (mViewGrid) {
						bool isSnap = mGrid.getSnapInfo();
						if (!mControlKeyPressed && isSnap ||
						    mControlKeyPressed && !isSnap) {
							mGrid.snapTo(pt1);
						}
					}
					
					if (mModel != nullptr && !mModel->isEmpty()) {
						double max = qMax(mWindowsBox.getWidth(), mWindowsBox.getHeight());
						double pickTol = max * mPickTolFactor;
						mModel->snapToCurve(pt1, pickTol);
					}
					showCoordinates(pt1);
					mCollector->addTempPoint(pt1);
					update();
				}
			}
			break;
	}
	
	update();
}

void CanvasQWidget::mouseReleaseEvent(QMouseEvent* event) {
	mButtonPressed = false;
	mPt1 = event->pos();
	
	QPointF pt1 = convertPtCoordsToUniverse(mPt1);
	QPointF pt0 = convertPtCoordsToUniverse(mPt0);
	
	if (mCurrentAction == ActionType::SELECTION) {
		if (mMouseButton == Qt::LeftButton) {
			if (mModel != nullptr && !mModel->isEmpty()) {
				if (PointUtils::dist(mPt0, mPt1) <= mMouseMoveTol) {
					double max = qMax(mWindowsBox.getWidth(), mWindowsBox.getHeight());
					double pickTol = max * mPickTolFactor;
					mModel->selectPick(pt1, pickTol, mShiftKeyPressed);
				} else {
					RectUtils::RectF r{QRectF{pt0, pt1}};
					mModel->selectFence(r, mShiftKeyPressed);
				}
			}
			update();
		}
		return;
	}
	
	if (mCurrentAction == ActionType::COLLECTION) {
		if (mMouseButton == Qt::LeftButton) {
			if (PointUtils::dist(mPt0, mPt1) <= mMouseMoveTol) {
				double max = qMax(mWindowsBox.getWidth(), mWindowsBox.getHeight());
				double pickTol = max * mPickTolFactor;
				if (mViewGrid) {
					bool isSnapOn = mGrid.getSnapInfo();
					if ((!mControlKeyPressed && isSnapOn) || (mControlKeyPressed && !isSnapOn))
						mGrid.snapTo(pt1);
				}
				if (mModel != nullptr && !mModel->isEmpty()) {
					mModel->snapToCurve(pt1, pickTol);
				}
				mCollector->insertPoint(pt1, pickTol);
			}
		}
		
		bool endCollection = false;
		if (mMouseButton == Qt::LeftButton) {
			if (mCollector->hasFinished())
				endCollection = true;
		} else if (mMouseButton == Qt::RightButton) {
			mCollector->reset();
			update();
		}
		
		if (endCollection) {
			Curve* curve = mCollector->getCollectedCurve();
			mModel->insertCurve(curve);
			mCollector->finishCollection();
			update();
		}
	}
	
}

void CanvasQWidget::mousePressEvent(QMouseEvent* event) {
	mButtonPressed = true;
	
	mPt0 = event->pos();
	mMouseButton = event->button();
	switch (mCurrentAction) {
		
		case ActionType::SELECTION:
			break;
		case ActionType::COLLECTION:
			if (!mCollector->isActive()) {
				if (mMouseButton == Qt::LeftButton)
					mCollector->startCollection();
			}
			break;
		case ActionType::UNDEF:
			break;
	}
}

QPointF CanvasQWidget::convertPtCoordsToUniverse(QPointF pt) const {
	return mTransform.inverted().map(pt);
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
		mCollector->reset();
	
	update();
}

void CanvasQWidget::setCurveType(CurveType type) {
	if (mCurrentAction == ActionType::COLLECTION && mCollector->getCurveType() == type)
		return;
	
	mCollector->reset();
	mCollector->setCurveType(type);
	update();
}

void CanvasQWidget::setGridSnapData(bool viewGrid, bool isSnapOn, double dx, double dy) {
	mGrid.setSnapData(isSnapOn, dx, dy);
	mViewGrid = viewGrid;
	update();
}

bool CanvasQWidget::getGridSnapInfo(double& dx, double& dy) {
	auto space = mGrid.getGridSpace();
	dx = space.first;
	dy = space.second;
	return mGrid.getSnapInfo();
}

void CanvasQWidget::delSelectedEntities() {
	if (mModel != nullptr && !mModel->isEmpty()) {
		mModel->delSelectedCurves();
		mModel->delSelectedRegions();
		update();
	}
	
}

void CanvasQWidget::makeDisplayModel(QPainter& painter) {
	const std::vector<Curve*>& curves = mModel->curves();
	for (const Curve* c : curves) {
		QVector<QLineF> lines = c->getPointsToDraw();
		if (c->isSelected())
			mPen.setBrush(mSelectionColor);
		else
			mPen.setBrush(mCurveColor);
		
		painter.setPen(mPen);
		painter.drawLines(lines);
		
		mPen.setBrush(mVertexColor);
		painter.setPen(mPen);
		painter.drawPoint(lines.first().p1());
		painter.drawPoint(lines.last().p2());
	}
	
	const std::vector<Region>& regions = mModel->regions();
	for (const Region& reg : regions) {
		if (reg.isSelected())
			mPen.setBrush(mSelectionColor);
		else
			mPen.setBrush(mRegionColor);
		
		painter.setPen(mPen);
		QPainterPath path = reg.pathToFill();
		painter.fillPath(path, mPen.brush());
		
		if (reg.getMesh() == nullptr)
			continue;
		
		mPen.setBrush(mGridColor);
		painter.setPen(mPen);
		for (const QPolygonF& pol : *reg.getMesh()) {
			painter.drawPolygon(pol);
		}
	}
}

void CanvasQWidget::drawSelectionFence(QPainter& painter) {
	if (!mButtonPressed)
		return;
	
	if (mPt0 == mPt1)
		return;
	
	mPen.setBrush(mSelectionColor);
	painter.setPen(mPen);
	QPointF pt0 = convertPtCoordsToUniverse(mPt0);
	QPointF pt1 = convertPtCoordsToUniverse(mPt1);
	QRectF fence{pt0, pt1};
	
	painter.drawRect(fence);
}

void CanvasQWidget::drawCollectedCurve(QPainter& painter) {
	if (!mCollector->isActive() || !mCollector->isCollecting())
		return;
	
	mPen.setBrush(mCollectingColor);
	painter.setPen(mPen);
	
	QVector<QLineF> lines = mCollector->getDrawPoints();
	painter.drawLines(lines);
	
	QVector<QPointF> ctrlPts = mCollector->getPoints();
	painter.drawPoints(ctrlPts);
}

void CanvasQWidget::scaleWorldWindow(double scaleFactor) {
	double vpr = (double) mH / (double) mW;
	
	double cx = mWindowsBox.centerX();
	double cy = mWindowsBox.centerY();
	
	double sizeX = mWindowsBox.getWidth() * scaleFactor;
	double sizeY = mWindowsBox.getHeight() * scaleFactor;
	double wr = sizeY / sizeX;
	
	if (wr > vpr)
		sizeX = sizeY / vpr;
	else
		sizeY = sizeX * vpr;
	mWindowsBox.setLeft(cx - sizeX / 2.0);
	mWindowsBox.setRight(cx + sizeX / 2.0);
	mWindowsBox.setBot(cy - sizeY / 2.0);
	mWindowsBox.setTop(cy + sizeY / 2.0);
	
	qreal penSize = qMin(mWindowsBox.getWidth(), mWindowsBox.getHeight()) * 0.01;
	mPen.setWidthF(penSize);
}

void CanvasQWidget::makeDisplayGrid(QPainter& painter) {
	std::pair<double, double> gridSpace = mGrid.getGridSpace();
	double gX = gridSpace.first;
	double gY = gridSpace.second;
	mPen.setBrush(mGridColor);
	painter.setPen(mPen);
	int c = 0;
	qreal x = GeomUtils::ceil2(mWindowsBox.getLeft(), gX);
	while (x <= mWindowsBox.getRight()) {
		qreal y = GeomUtils::ceil2(mWindowsBox.getBot(), gY);
		while (y <= mWindowsBox.getTop()) {
			painter.drawPoint(QPointF{x, y});
			y += gY;
		}
		x += gX;
	}
	
	painter.drawLine(QPointF(-gX * 0.5, 0.0), QPointF(gX * 0.5, 0.0));
	painter.drawLine(QPointF(0.0, -gY * 0.5), QPointF(0.0, gY * 0.5));
}

void CanvasQWidget::keyPressEvent(QKeyEvent* event) {
	mShiftKeyPressed = (event->key() == Qt::Key_Shift);
	mControlKeyPressed = (event->key() == Qt::Key_Control);
}

CanvasQWidget::~CanvasQWidget() = default;

void CanvasQWidget::keyReleaseEvent(QKeyEvent* event) {
	mShiftKeyPressed = false;
	mControlKeyPressed = false;
}

void CanvasQWidget::fitWorldToViewport() {
	if (mModel == nullptr || mModel->isEmpty())
		return;
	
	mWindowsBox = mModel->boundingBox();
	scaleWorldWindow(1.10);
	update();
}

void CanvasQWidget::zoomIn() {
	scaleWorldWindow(0.90);
	update();
}

void CanvasQWidget::zoomOut() {
	scaleWorldWindow(1.10);
	update();
}

void CanvasQWidget::showCoordinates(const QPointF& pt) {
	if (mLabelCoordinates == nullptr)
		return;
	
	QString str;
	str.sprintf("(%1.3f,\t%1.3f)", pt.x(), pt.y());
	mLabelCoordinates->setText(str);
}

void CanvasQWidget::createRegion() {
	if (mModel != nullptr && !mModel->isEmpty()) {
		mModel->createRegion();
		update();
	}
}

void CanvasQWidget::genDelaunay(int sub) {
	if (mModel != nullptr && !mModel->isEmpty()) {
		mModel->genDelaunay(sub);
	}
}
