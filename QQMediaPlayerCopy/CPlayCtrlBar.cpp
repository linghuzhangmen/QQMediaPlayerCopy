#include "CPlayCtrlBar.h"
#include <QHBoxLayout>

CPlayCtrlBar::CPlayCtrlBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_StyledBackground);  // 禁止父窗口样式影响子控件样式

	setFixedHeight(80);
	setStyleSheet("QWidget{background-color:rgb(17,17,17);}");

	QString label_qss = "QLabel{font-family:Microsoft YaHei; font-size:18px; color:rgb(255,255,255);}";

	m_pCurPlayTimeLabel = new QLabel(this);
	m_pCurPlayTimeLabel->setText("00:00:00");
	m_pCurPlayTimeLabel->setAlignment(Qt::AlignRight);
	m_pCurPlayTimeLabel->setFixedSize(80, 32);
	m_pCurPlayTimeLabel->setStyleSheet(label_qss);

	m_pTotalTimeLabel = new QLabel(this);
	m_pTotalTimeLabel->setText("01:20:30");
	m_pTotalTimeLabel->setAlignment(Qt::AlignLeft);
	m_pTotalTimeLabel->setFixedSize(160, 32);
	m_pTotalTimeLabel->setStyleSheet(label_qss);

	m_pPlayButtonGroupWidget = new CPlayButtonGroupWidget(this);

	m_pToolBoxButton = new QPushButton(this);
	m_pToolBoxButton->setText("1");
	m_pToolBoxButton->setFixedSize(32, 32);
	m_pToolBoxButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/toolbox.svg);border:none;}"
		"QPushButton:hover{background-image:url(:/playCtrlBar/resources/playctrlBar/toolbox_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/toolbox.svg);border:none;}");

	m_pEffectButton = new QPushButton(this);
	m_pEffectButton->setText("2");
	m_pEffectButton->setFixedSize(32, 32);
	m_pEffectButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/effect.svg);border:none;}"
		"QPushButton:hover{background-image:url(:/playCtrlBar/resources/playctrlBar/effect_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/effect.svg);border:none;}");

	m_pFullscreenButton = new QPushButton(this);
	m_pFullscreenButton->setText("3");
	m_pFullscreenButton->setFixedSize(32, 32);
	m_pFullscreenButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/fullscreen.svg);border:none;}"
		"QPushButton:hover{background-image:url(:/playCtrlBar/resources/playctrlBar/fullscreen_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/fullscreen.svg);border:none;}");

	QHBoxLayout* pHLay = new QHBoxLayout(this);
	pHLay->addSpacing(3);
	pHLay->addWidget(m_pCurPlayTimeLabel);
	pHLay->addSpacing(1);
	pHLay->addWidget(m_pTotalTimeLabel);
	pHLay->addStretch();
	pHLay->addWidget(m_pPlayButtonGroupWidget);
	pHLay->addStretch();
	pHLay->addWidget(m_pToolBoxButton);
	pHLay->addSpacing(18);
	pHLay->addWidget(m_pEffectButton);
	pHLay->addSpacing(18);
	pHLay->addWidget(m_pFullscreenButton);

	setLayout(pHLay);

	showTimeLabel(false);

	connect(m_pPlayButtonGroupWidget, &CPlayButtonGroupWidget::sig_play, this, &CPlayCtrlBar::sig_play);
	connect(m_pPlayButtonGroupWidget, &CPlayButtonGroupWidget::sig_stop, this, &CPlayCtrlBar::sig_stop);
	connect(m_pFullscreenButton, &QPushButton::clicked, this, &CPlayCtrlBar::sig_fullScreen);
}

CPlayCtrlBar::~CPlayCtrlBar()
{
}

void CPlayCtrlBar::showTimeLabel(bool isShow)
{
	if (isShow)
	{
		m_pCurPlayTimeLabel->show();
		m_pTotalTimeLabel->show();
	}
	else
	{
		m_pCurPlayTimeLabel->hide();
		m_pTotalTimeLabel->hide();
	}
}

//设置当前播放时间
void CPlayCtrlBar::setCurPlayTime(const QString& curPlaytime)
{
	m_pCurPlayTimeLabel->setText(curPlaytime);
}

//设置总的播放时间
void CPlayCtrlBar::setTotalTime(const QString& totalTime)
{
	m_pTotalTimeLabel->setText("/" + totalTime);
}

void CPlayCtrlBar::resizeEvent(QResizeEvent* event)
{
	if (m_pPlayButtonGroupWidget)
	{
		int x = this->width() / 2 - m_pPlayButtonGroupWidget->width() / 2;
		int y = this->height() / 2 - m_pPlayButtonGroupWidget->height() / 2;
		m_pPlayButtonGroupWidget->move(x, y);
	}
}
