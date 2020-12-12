//
// Created by vini on 12/11/20.
//

#ifndef DRAWERQTPAINTER_SUBDIVISIONDIALOG_H
#define DRAWERQTPAINTER_SUBDIVISIONDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class SubdivisionDialog; }
QT_END_NAMESPACE

class SubdivisionDialog : public QDialog {
Q_OBJECT

public:
	explicit SubdivisionDialog(QWidget* parent = nullptr);
	
	~SubdivisionDialog() override;

private:
	Ui::SubdivisionDialog* ui;
};

#endif //DRAWERQTPAINTER_SUBDIVISIONDIALOG_H
