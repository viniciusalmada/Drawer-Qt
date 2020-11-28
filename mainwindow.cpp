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
	
	mEditGridX->setText(QString().setNum(1.0));
	mEditGridX->setFixedWidth(50);
	mEditGridY->setText(QString().setNum(1.0));
	mEditGridY->setFixedWidth(50);
	
	mSnapBox->setEnabled(false);
	mEditGridX->setEnabled(false);
	mEditGridY->setEnabled(false);
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
	
	bool gridChecked = state == Qt::CheckState::Checked;
	bool isSnapOn = mSnapBox->isChecked();
	bool ok;
	double dx = mEditGridX->text().toDouble(&ok);
	if (!ok) {
		dx = 1.0;
		mEditGridX->setText(QString().setNum(1.0));
	}
	double dy = mEditGridY->text().toDouble(&ok);
	if (!ok) {
		dy = 1.0;
		mEditGridY->setText(QString().setNum(1.0));
	}
	
	ui->widget->setGridSnapData(gridChecked, isSnapOn, dx, dy);
	
	mSnapBox->setEnabled(gridChecked);
	mEditGridX->setEnabled(gridChecked);
	mEditGridY->setEnabled(gridChecked);
	
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



