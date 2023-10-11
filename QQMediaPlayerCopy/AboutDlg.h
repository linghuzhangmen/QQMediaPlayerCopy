#pragma once

#include <QDialog>
#include "ui_AboutDlg.h"

class AboutDlg : public QDialog
{
	Q_OBJECT

public:
	AboutDlg(QWidget *parent = nullptr);
	~AboutDlg();

private slots:
	void on_btnLook_clicked();

private:
	Ui::AboutDlgClass ui;
};
