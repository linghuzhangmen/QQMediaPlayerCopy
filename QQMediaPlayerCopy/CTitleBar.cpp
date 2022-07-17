#include "CTitleBar.h"
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QMenu>
#include <QMouseEvent>
#include "qss.h"
#include <Windows.h>

#include <qt_windows.h>
#pragma comment(lib, "user32.lib")




CTitleBar::CTitleBar(QWidget *parent)
	: QWidget(parent)
{
	this->setAttribute(Qt::WA_DeleteOnClose);
	setMouseTracking(true);
	initUI();
}

CTitleBar::~CTitleBar()
{
}

void CTitleBar::setFileNameLabelText(QString name)
{
	m_pFileNameLabel->show();
	m_pFileNameLabel->setText(name);
}

void CTitleBar::initUI()
{
	//禁止父窗口影响子窗口样式
	setAttribute(Qt::WA_StyledBackground);
	this->setFixedHeight(32 + 5 * 2);
	this->setStyleSheet("background-color:rgb(54,54,54)");

	m_pLogoBtn = new QPushButton(this);
	m_pLogoBtn->setMinimumWidth(138);
	m_pLogoBtn->setText("Media Player");
	m_pLogoBtn->setStyleSheet(QString::fromStdString(logo_button_qss));

	QMenu* pMenu = new QMenu(this);
	pMenu->setStyleSheet(QString::fromStdString(menu_qss));

	QAction* pAc1 = new QAction("open file", this);
	QAction* pAc2 = new QAction("open floder", this);
	QAction* pAc3 = new QAction("about", this);
	QAction* pAc4 = new QAction("exit", this);

	pMenu->addAction(pAc1);
	pMenu->addAction(pAc2);
	pMenu->addAction(pAc3);
	pMenu->addAction(pAc4);

	m_pLogoBtn->setMenu(pMenu);

	m_pFileNameLabel = new QLabel(this);
	m_pFileNameLabel->setMinimumWidth(60);
	m_pFileNameLabel->setAlignment(Qt::AlignCenter);
	m_pFileNameLabel->hide();  // 视频文件标题，默认隐藏
	m_pFileNameLabel->setStyleSheet("QLabel{font-family:Microsoft YaHei; \
		font-size:14px;color:#FFFFFF;background-color:#363636;}");

	m_pMinimodeBtn = new QPushButton(this);
	m_pMinimodeBtn->setObjectName("m_pMinimodeBtn");
	m_pMinimodeBtn->setText("");
	m_pMinimodeBtn->setFixedSize(32, 32);
	m_pMinimodeBtn->setStyleSheet(QString::fromStdString(minimode_qss));

	m_pSettopBtn = new QPushButton(this);
	m_pSettopBtn->setObjectName("m_pSettopBtn");
	m_pSettopBtn->setText("");
	m_pSettopBtn->setToolTip(tr("set to top window"));
	m_pSettopBtn->setFixedSize(32, 32);
	m_pSettopBtn->setStyleSheet(QString::fromStdString(settop_qss));

	m_pMinBtn = new QPushButton(this);
	m_pMinBtn->setFixedSize(32, 32);
	m_pMinBtn->setStyleSheet(QString::fromStdString(min_qss));

	m_pMaxBtn = new QPushButton(this);
	m_pMaxBtn->setObjectName("m_pMaxBtn");
	m_pMaxBtn->setText("");
	m_pMaxBtn->setFixedSize(32, 32);
	m_pMaxBtn->setStyleSheet(QString::fromStdString(max_qss));

	m_pCloseBtn = new QPushButton(this);
	m_pCloseBtn->setFixedSize(32, 32);
	m_pCloseBtn->setStyleSheet(QString::fromStdString(close_qss));

	QHBoxLayout* pHlay = new QHBoxLayout(this);
	pHlay->addWidget(m_pLogoBtn);
	//pHlay->addWidget(m_pFileNameLabel, Qt::AlignCenter); // 这种不是那么得居中，直接用move方法设置居中
	pHlay->addStretch(1);
	pHlay->addWidget(m_pMinimodeBtn);
	QSpacerItem* pItem = new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
	pHlay->addSpacerItem(pItem);

	pHlay->addWidget(m_pSettopBtn);
	QSpacerItem* pItem1 = new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
	pHlay->addSpacerItem(pItem1);

	pHlay->addWidget(m_pMinBtn);
	QSpacerItem* pItem2 = new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
	pHlay->addSpacerItem(pItem2);
	
	pHlay->addWidget(m_pMaxBtn);
	QSpacerItem* pItem3 = new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
	pHlay->addSpacerItem(pItem3);  

	pHlay->addWidget(m_pCloseBtn);

	pHlay->setContentsMargins(5, 5, 5, 5);

	connect(m_pMinBtn, &QPushButton::clicked, this, &CTitleBar::onClicked);
	connect(m_pMaxBtn, &QPushButton::clicked, this, &CTitleBar::onClicked);
	connect(m_pCloseBtn, &QPushButton::clicked, this, &CTitleBar::onClicked);
	connect(m_pMinimodeBtn, &QPushButton::clicked, this, &CTitleBar::onClicked);
	connect(m_pSettopBtn, &QPushButton::clicked, this, &CTitleBar::onClicked);
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
	emit sig_ShowFullFcreen();
}

void CTitleBar::resizeEvent(QResizeEvent* event)
{
	int x = (this->width() - m_pFileNameLabel->width()) / 2;
	int y = (this->height() - m_pFileNameLabel->height()) / 2;
	m_pFileNameLabel->move(x, y);
}

void CTitleBar::onClicked()
{
	QPushButton* pButton = qobject_cast<QPushButton*>(sender());

	QWidget* pWindow = this->window();

	if (pButton == m_pMinBtn)
	{
		pWindow->showMinimized();
	}
	else if (pButton == m_pMaxBtn)
	{
		if (pWindow->isMaximized())
		{
			pWindow->showNormal();
			m_pMaxBtn->setStyleSheet("QPushButton{background-image:url(:/titleBar/resources/titleBar/max.svg);border:none}" \
				"QPushButton:hover{" \
				"background-color:rgb(99, 99, 99);" \
				"background-image:url(:/titleBar/resources/titleBar/max_hover.svg);border:none;}");
		}
		else
		{
			pWindow->showMaximized();
			m_pMaxBtn->setStyleSheet("QPushButton{background-image:url(:/titleBar/resources/titleBar/normal.svg);border:none}" \
				"QPushButton:hover{" \
				"background-color:rgb(99, 99, 99);" \
				"background-image:url(:/titleBar/resources/titleBar/normal_hover.svg);border:none;}");
		}
	}
	else if (pButton == m_pMinimodeBtn)
	{
		emit sig_showMiniMode();
	}
	else if (pButton == m_pSettopBtn)
	{
		HWND hwnd = (HWND)(pWindow->winId());
		if (::GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOPMOST)
		{
			// The window is topmost.
			::SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pSettopBtn->setStyleSheet(QString::fromStdString(settop_qss));
			m_pSettopBtn->setToolTip(tr("set to top window"));
		}
		else
		{
			// The window is not topmost.
			::SetWindowPos(HWND(pWindow->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pSettopBtn->setStyleSheet(QString::fromStdString(settoped_qss));
			m_pSettopBtn->setToolTip(tr("cancal top window"));
		}
	}
	else if (pButton == m_pCloseBtn)
	{
		emit sig_close();
	}
}

