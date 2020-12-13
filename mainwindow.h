#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <manager/model.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	
	~MainWindow() override;

private:
	Ui::MainWindow* ui;
	Model* mModel;
	
	QWidget* mStatusBarWidget;
	QCheckBox* mGridBox;
	QCheckBox* mSnapBox;
	QLabel* mLabelGridX;
	QLabel* mLabelGridY;
	QLineEdit* mEditGridX;
	QLineEdit* mEditGridY;
	
	QLabel* mLabelCoords;
	
	void onActionSelectCallback();
	
	void onActionGridBoxCallback(int state);
	
	void onActionSnapBoxCallback(int state);
	
	void onReturnPressedCallback();
	
	void onActionLineCallback();
	
	void onActionCurveCallback();
	
	void onActionArcCallback();
	
	void onActionRegionCallback();
	
	void onActionDelaunayCallback();
	
	void onActionPartitionCallback();
	
	void onActionFitCallback();
	
	void onActionZoomInCallback();
	
	void onActionZoomOutCallback();
	
	void onActionDeleteCallback();
};

#endif // MAINWINDOW_H
