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
	
	void onActionLineCallback();
	
	void onActionFitCallback();
	
	void onActionZoomInCallback();
	
	void onActionZoomOutCallback();
};

#endif // MAINWINDOW_H
