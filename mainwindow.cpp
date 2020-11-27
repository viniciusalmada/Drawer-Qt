#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent), ui(new Ui::MainWindow) {
	
	ui->setupUi(this);
	
	mModel = new Model();
	ui->widget->setModel(mModel);
	
	mStatusBarWidget = new QWidget();
	mGridBox = new QCheckBox("Enable Grid");
	mSnapBox = new QCheckBox("Snap to Grid");
	mLabelGridX = new QLabel("Grix X: ");
	mEditGridX = new QLineEdit();
	mLabelGridY = new QLabel("Grix Y: ");
	mEditGridY = new QLineEdit();
	
	auto* layout = new QHBoxLayout(mStatusBarWidget);
	layout->addWidget(mGridBox);
	layout->addWidget(mSnapBox);
	layout->addWidget(mLabelGridX);
	layout->addWidget(mEditGridX);
	layout->addWidget(mLabelGridY);
	layout->addWidget(mEditGridY);
	ui->statusBar->addWidget(mStatusBarWidget);
	
	ui->actionSelect->setChecked(true);
	ui->actionLine->setChecked(false);
	
	ui->widget->setMouseAction(ActionType::SELECTION);
	
	connect(ui->actionSelect, &QAction::triggered, this, &MainWindow::onActionSelectCallback);
	connect(ui->actionLine, &QAction::triggered, this, &MainWindow::onActionLineCallback);
	connect(ui->actionFit, &QAction::triggered, this, &MainWindow::onActionFitCallback);
	connect(ui->actionZoomIn, &QAction::triggered, this, &MainWindow::onActionZoomInCallback);
	connect(ui->actionZoomOut, &QAction::triggered, this, &MainWindow::onActionZoomOutCallback);
	
	connect(mGridBox, &QCheckBox::stateChanged, this, &MainWindow::onActionGridBoxCallback);
}

MainWindow::~MainWindow() {
	delete ui;
	delete mGridBox;
	delete mSnapBox;
	delete mLabelGridX;
	delete mEditGridX;
	delete mLabelGridY;
	delete mEditGridY;
	delete mStatusBarWidget;
}

void MainWindow::onActionSelectCallback() {
	ui->actionSelect->setChecked(true);
	ui->actionLine->setChecked(false);
	ui->widget->setMouseAction(ActionType::SELECTION);
}

void MainWindow::onActionGridBoxCallback(int state) {
	
	if (state == Qt::CheckState::Checked) {
		double dx = 0.0;
		double dy = 0.0;
		bool isSnapOn = ui->widget->getGridSnapInfo(dx, dy);
		
		QString sdx;
		sdx.setNum(dx);
		mEditGridX->clear();
		mEditGridX->insert(sdx);
		
		QString sdy;
		sdy.setNum(dy);
		mEditGridY->clear();
		mEditGridY->insert(sdy);
		
		mSnapBox->setChecked(isSnapOn);
		
		
	}
}

void MainWindow::onActionLineCallback() {
	ui->actionSelect->setChecked(false);
	ui->actionLine->setChecked(true);
	ui->widget->setMouseAction(ActionType::COLLECTION);
	ui->widget->setCurveType(CurveType::LINE);
}

void MainWindow::onActionFitCallback() {
	ui->widget->fitWorldToViewport();
}

void MainWindow::onActionZoomInCallback() {
	ui->widget->zoomIn();
}

void MainWindow::onActionZoomOutCallback() {
	ui->widget->zoomOut();
}



