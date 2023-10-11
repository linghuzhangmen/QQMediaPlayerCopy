/*

主界面头文件


作者：令狐掌门
博客链接：https://blog.csdn.net/yao_hou/category_9275800.html

*/

#pragma once

#include <QtWidgets/QWidget>
#include "CTitleBar.h"
#include "CPlayCtrlBar.h"
#include "VideoWidget.h"
#include <queue>
#include <list>
#include <mutex>
#include <Windows.h>
#include "CPlayListWidget.h"
#include "CVlcKits.h"
#include <memory>
#include "CFrameLessWidgetBase.h"

class CMainWindow : public CFrameLessWidgetBase
{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = Q_NULLPTR);

private:
    void InitUI();

protected:
	void resizeEvent(QResizeEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
	
protected slots: 
    void On_Close();
    void On_ShowFullScreen();
    void On_ShowMiniMode();
	void on_openFile(const QStringList& fileList);
    void on_openUrl(const QString& url);
    void On_openRightPlaylist();
    void On_timer();
    void OnUpdateTimeText(const QString& str);
    void OnUpdateTimeSlider(const int& value);
    void OnSetPlayRate(double rate);
    void OnPlay(const QString& fileName);

private:
	CTitleBar*       m_pTitleBar = nullptr;
	CPlayCtrlBar*    m_pPlayCtrlBar = nullptr;
	VideoWidget*     m_pVideoWidget = nullptr;
	CPlayListWidget* m_pPlaylistWidget = nullptr;
	bool             m_bPlaylistIsOpen = false;
	QStringListModel* m_pStringListModel = nullptr;
	std::string        m_curPlayFileName;

	std::unique_ptr<CVlcKits> m_pVlc;
	bool m_isPlay = false;
    bool m_isMiniMode = false;
    QTimer* m_pTimer = nullptr;
    QPoint mouse_last_moved_pos_;
};
