#include "AboutDlg.h"
#include <QDesktopServices>
#include <QUrl>
#include <QLabel>
#include <QMessageBox>

AboutDlg::AboutDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	setWindowTitle(tr("About the Project"));
	setFixedSize(450, 178);

	this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

	ui.label_author->setText("<a href=\"https://blog.csdn.net/yao_hou?type=blog\">https://blog.csdn.net/yao_hou?type=blog</a>");
	ui.label_author->setOpenExternalLinks(true);
	ui.label_author->setTextFormat(Qt::RichText);
}

AboutDlg::~AboutDlg()
{}

void AboutDlg::on_btnLook_clicked()
{
	QMessageBox::information(this, tr("title"), tr("read readme.md"));
}
