//
// Created by vini on 12/11/20.
//

// You may need to build the project (run Qt uic code generator) to get "ui_asdfasdf.h" resolved

#include "subdivisiondialog.h"
#include "ui_subdivisiondialog.h"

SubdivisionDialog::SubdivisionDialog(QWidget* parent) :
		QDialog(parent), ui(new Ui::SubdivisionDialog) {
	ui->setupUi(this);
}

SubdivisionDialog::~SubdivisionDialog() {
	delete ui;
}
