#include "CTitleBar.h"
#include <QStyleOption>
#include <QPainter>
#include <QMenu>
#include <QMouseEvent>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#pragma comment(lib, "user32.lib")
#endif

static const int TITLEBAR_FEXED_HEIGHT = 50;



CTitleBar::CTitleBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setStyleSheet("QWidget{background-color:rgb(17,17,17);}");

	this->setFixedHeight(TITLEBAR_FEXED_HEIGHT);

	QString logo_button_qss = "QPushButton{font-family:Microsoft YaHei;font-size:18px; color:rgb(255,255,255);}"
		"QPushButton::menu-indicator:open{"
		"image:url(:/titleBar/resources/titleBar/down_arrow.svg);"
		"subcontrol-position:right center;"
		"subcontrol-origin:padding;border:none;}"
		"QPushButton::menu-indicator:closed{"
		"image:url(:/titleBar/resources/titleBar/up_arrow.svg);"
		"subcontrol-position:right center;"
		"subcontrol-origin:padding;border:none;}";

	QString menu_qss = "QMenu{background-color:rgb(253,253,253);}"
		"QMenu::item{"
		"font:16px;"
		"background-color:rgb(253,253,253);"
		"padding:8px 32px;"
		"margin:0px 8px;"
		"border-bottom:1px solid #DBDBDB;}"
		/*选择项设置*/
		"QMenu::item:selected{background-color: #FFF8DC;}";

	ui.btnLogo->setText(u8"QQ影音");
	ui.btnLogo->setFlat(true);
	ui.btnLogo->setFixedSize(120, 34);

	QMenu* pMenu = new QMenu(this);
	pMenu->setStyleSheet(menu_qss);

	QAction* pAc1 = new QAction(u8"open file", this);
	QAction* pAc2 = new QAction(u8"open floder", this);
	QAction* pAc3 = new QAction(u8"about", this);
	QAction* pAc4 = new QAction(u8"exit", this);

	pMenu->addAction(pAc1);
	pMenu->addAction(pAc2);
	pMenu->addAction(pAc3);
	pMenu->addAction(pAc4);

	ui.btnLogo->setStyleSheet(logo_button_qss);
	ui.btnLogo->setMenu(pMenu);

	ui.btnMinimode->setFixedSize(32, 32);
	ui.btnMinimode->setText("");
	ui.btnMinimode->setFlat(true);
	ui.btnMinimode->setStyleSheet("QPushButton{background-image:url(:/titleBar/resources/titleBar/minimode.svg);brder:none}"
		"QPushButton:hover{"
		"background-image:url(:/titleBar/resources/titleBar/minimode_hover.svg);border:none;}");

	ui.btnSettop->setFixedSize(32, 32);
	ui.btnSettop->setText("");
	ui.btnSettop->setFlat(true);
	ui.btnSettop->setStyleSheet("QPushButton{background-image:url(:/titleBar/resources/titleBar/settop.svg);brder:none}"
		"QPushButton:hover{"
		"background-image:url(:/titleBar/resources/titleBar/settop_hover.svg);border:none;}");

	ui.btnMin->setFixedSize(32, 32);
	ui.btnMin->setText("");
	ui.btnMin->setFlat(true);
	ui.btnMin->setStyleSheet("QPushButton{background-image:url(:/titleBar/resources/titleBar/min.svg);brder:none}"
		"QPushButton:hover{"
		"background-image:url(:/titleBar/resources/titleBar/min_hover.svg);border:none;}");

	ui.btnMax->setFixedSize(32, 32);
	ui.btnMax->setText("");
	ui.btnMax->setFlat(true);
	ui.btnMax->setStyleSheet("QPushButton{background-image:url(:/titleBar/resources/titleBar/max.svg);brder:none}"
		"QPushButton:hover{"
		"background-image:url(:/titleBar/resources/titleBar/max_hover.svg);border:none;}");

	ui.btnClose->setFixedSize(32, 32);
	ui.btnClose->setText("");
	ui.btnClose->setFlat(true);
	ui.btnClose->setStyleSheet("QPushButton{background-image:url(:/titleBar/resources/titleBar/close.svg);brder:none}"
		"QPushButton:hover{"
		"background-image:url(:/titleBar/resources/titleBar/close_hover.svg);border:none;}");

	connect(ui.btnMinimode, SIGNAL(clicked(bool)), this, SLOT(on_Clicked()));
	connect(ui.btnSettop, SIGNAL(clicked(bool)), this, SLOT(on_Clicked()));
	connect(ui.btnMin, SIGNAL(clicked(bool)), this, SLOT(on_Clicked()));
	connect(ui.btnMax, SIGNAL(clicked(bool)), this, SLOT(on_Clicked()));
	connect(ui.btnClose, SIGNAL(clicked(bool)), this, SLOT(on_Clicked()));
}

CTitleBar::~CTitleBar()
{
}

int CTitleBar::getHeight()
{
	return TITLEBAR_FEXED_HEIGHT;
}

void CTitleBar::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
	//opt.init(this);
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(event);
}

void CTitleBar::mousePressEvent(QMouseEvent* event)
{
	if (ReleaseCapture())
	{
		QWidget* pWindow = this->window();
		if (pWindow->isTopLevel())
		{
			SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
		}
	}

	event->ignore();
}

void CTitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	emit ui.btnMax->clicked();
}

void CTitleBar::on_Clicked()
{
	QPushButton* pButton = qobject_cast<QPushButton*>(sender());

	QWidget* pWindow = this->window();

	if (pWindow->isTopLevel())
	{
		if (pButton == ui.btnMin)
		{
			pWindow->showMinimized();
		}
		else if (pButton == ui.btnMax)
		{
			pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
		}
		else if (pButton == ui.btnClose)
		{
			emit sig_close();
		}
	}
}