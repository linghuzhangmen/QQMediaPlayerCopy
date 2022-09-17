#pragma once

#include <QDialog>
#include "ui_CNetStreamDlg.h"

class CNetStreamDlg : public QDialog
{
	Q_OBJECT

public:
	CNetStreamDlg(QWidget *parent = Q_NULLPTR);
	~CNetStreamDlg();

	QString getUrl() const;

private slots:
	void on_btnStartPlay_clicked();

private:
	Ui::CNetStreamDlg ui;

	QString m_url;
};
