#include "CNetStreamDlg.h"
#include <QMessageBox>

CNetStreamDlg::CNetStreamDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	setFixedSize(this->width(), this->height());

	ui.btnStartPlay->setText(tr("start play"));
	ui.label_name->setText(tr("please intput url"));
}

CNetStreamDlg::~CNetStreamDlg()
{
}

QString CNetStreamDlg::getUrl() const
{
	return m_url;
}

void CNetStreamDlg::on_btnStartPlay_clicked()
{
	m_url = ui.lineEdit_url->text();
	if (m_url.isEmpty())
	{
		QMessageBox::information(this, tr("tips"), tr("url is empty"));
		return;
	}

	accept();
}
