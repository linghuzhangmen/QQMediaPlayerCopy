#include "CTitleBar.h"
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QMenu>
#include <QMouseEvent>
#include "qss.h"
#include <Windows.h>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QFileDialog>
#include "CNetStreamDlg.h"

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
	//��ֹ������Ӱ���Ӵ�����ʽ
	setAttribute(Qt::WA_StyledBackground);
	int w = 20;
	int h = 20;
	int margin = 5;
	this->setFixedHeight(h + margin * 2);
	this->setStyleSheet("background-color:rgb(54,54,54)");

	m_pLogoBtn = new QPushButton(this);
	m_pLogoBtn->setMinimumWidth(120);
	m_pLogoBtn->setText(tr("Media Player"));
	m_pLogoBtn->setStyleSheet(logo_button_qss);

	QMenu* pMenu = new QMenu(this);
	pMenu->setStyleSheet(menu_qss);

	QAction* pAc1 = new QAction(tr("open file"), this);
	QAction* pAc2 = new QAction(tr("open floder"), this);
	QAction* pAc3 = new QAction(tr("open net stream"), this);
	QAction* pAc4 = new QAction(tr("about"), this);
	QAction* pAc5 = new QAction(tr("exit"), this);

	pMenu->addAction(pAc1);
	pMenu->addAction(pAc2);
	pMenu->addAction(pAc3);
	pMenu->addAction(pAc4);
	pMenu->addAction(pAc5);

	m_pLogoBtn->setMenu(pMenu);

	connect(pAc1, &QAction::triggered, this, &CTitleBar::openFile);
	connect(pAc2, &QAction::triggered, this, &CTitleBar::openFloder);
	connect(pAc3, &QAction::triggered, this, &CTitleBar::openNetStream);
	connect(pAc4, &QAction::triggered, this, &CTitleBar::about);
	connect(pAc5, &QAction::triggered, this, &CTitleBar::exit);

	m_pFileNameLabel = new QLabel(this);
	m_pFileNameLabel->setMinimumWidth(60);
	m_pFileNameLabel->setAlignment(Qt::AlignCenter);
	m_pFileNameLabel->hide();  // ��Ƶ�ļ����⣬Ĭ������
	m_pFileNameLabel->setStyleSheet("QLabel{font-family:Microsoft YaHei; \
		font-size:14px;color:#FFFFFF;background-color:#363636;}");

	m_pMinimodeBtn = new QPushButton(this);
	m_pMinimodeBtn->setObjectName("m_pMinimodeBtn");
	m_pMinimodeBtn->setText("");
	m_pMinimodeBtn->setFixedSize(w, h);
	m_pMinimodeBtn->setStyleSheet(minimode_qss);

	m_pSettopBtn = new QPushButton(this);
	m_pSettopBtn->setObjectName("m_pSettopBtn");
	m_pSettopBtn->setText("");
	m_pSettopBtn->setToolTip(tr("set to top window"));
	m_pSettopBtn->setFixedSize(w, h);
	m_pSettopBtn->setStyleSheet(settop_qss);

	m_pMinBtn = new QPushButton(this);
	m_pMinBtn->setFixedSize(w, h);
	m_pMinBtn->setStyleSheet(min_qss);

	m_pMaxBtn = new QPushButton(this);
	m_pMaxBtn->setObjectName("m_pMaxBtn");
	m_pMaxBtn->setText("");
	m_pMaxBtn->setFixedSize(w, h);
	m_pMaxBtn->setStyleSheet(max_normal_qss);

	m_pCloseBtn = new QPushButton(this);
	m_pCloseBtn->setObjectName("m_pCloseBtn");
	m_pCloseBtn->setText("");
	m_pCloseBtn->setFixedSize(w, h);
	m_pCloseBtn->setStyleSheet(close_qss);

	QHBoxLayout* pHlay = new QHBoxLayout(this);
	pHlay->addWidget(m_pLogoBtn);
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

	pHlay->setContentsMargins(margin, margin, margin, margin);

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
			m_pMaxBtn->setStyleSheet(max_normal_qss);
		}
		else
		{
			pWindow->showMaximized();
			m_pMaxBtn->setStyleSheet(maxed_qss);
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
			m_pSettopBtn->setStyleSheet(settop_qss);
			m_pSettopBtn->setToolTip(tr("set to top window"));
		}
		else
		{
			// The window is not topmost.
			::SetWindowPos(HWND(pWindow->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pSettopBtn->setStyleSheet(settoped_qss);
			m_pSettopBtn->setToolTip(tr("cancal top window"));
		}
	}
	else if (pButton == m_pCloseBtn)
	{
		emit sig_close();
	}
}

void CTitleBar::openFile()
{
	QString cfgPath = "HKEY_CURRENT_USER\\Software\\CMainWindow";
	QSettings settings(cfgPath, QSettings::NativeFormat);
	QString lastPath = settings.value("openfile_path").toString();  // ��ע����ȡ·��

	if (lastPath.isEmpty())
	{
		lastPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);  //��ȡĬ�ϵ��ĵ�·�� 
	}

	//����ͬʱ�򿪶���ļ�
	QStringList filelist = QFileDialog::getOpenFileNames(
		this,
		tr("select video file"),
		lastPath,
		tr("video file (*.flv *.rmvb *.avi *.mp4);; all files(*.*);; "));

	if (filelist.isEmpty())
	{
		return;
	}

	int end = filelist[0].lastIndexOf("/");
	QString tmppath = filelist[0].left(end + 1);
	settings.setValue("openfile_path", tmppath);  // ����ǰ�򿪵�·��д�뵽ע���

	emit sig_openfile(filelist);
}

void CTitleBar::openFloder()
{
	QString cfgPath = "HKEY_CURRENT_USER\\Software\\CMainWindow";
	QSettings settings(cfgPath, QSettings::NativeFormat);
	QString lastPath = settings.value("openfile_path").toString();  // ��ע����ȡ·��

	if (lastPath.isEmpty())
	{
		lastPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);  //��ȡĬ�ϵ��ĵ�·�� 
	}

	QString path = QFileDialog::getExistingDirectory(this,
		tr("choose video directory"), lastPath);

	if (path.isEmpty())
	{
		return;
	}

	QDir dir(path);

	QStringList nameFilters;
	nameFilters << "*.mp4" << "*.flv" << "*.rmvb" << "*.avi";

	//�����˺���ļ����ƴ��뵽files�б���
	QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);

	int _size = files.size();
	if (_size == 0)
	{
		QMessageBox::information(this, tr("Warn"), tr("this floder no video files"));
		return;
	}

	QStringList fileList;
	for (int i = 0; i < _size; i++)
	{
		QString fileName = path + "/" + files[i];
		fileList << fileName;
	}

	settings.setValue("openfile_path", path);  // ����ǰ�򿪵�·��д�뵽ע���

	emit sig_openfile(fileList);
}

void CTitleBar::openNetStream()
{
	CNetStreamDlg dlg;
	if (dlg.exec() == QDialog::Accepted)
	{
		QString strText = dlg.getUrl();
		emit sig_openUrl(strText);
	}
}

void CTitleBar::about()
{
	QMessageBox::information(this, tr("tips"), tr("Qt libvlc mediaplayer"));
}

void CTitleBar::exit()
{
	::exit(0);
}
