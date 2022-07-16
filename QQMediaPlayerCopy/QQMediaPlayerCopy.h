/*

主界面头文件


作者：令狐掌门
博客链接：https://blog.csdn.net/yao_hou/category_9275800.html
QQ群：894097803

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

namespace {
    const int kMouseRegionLeft = 5;
    const int kMouseRegionTop = 5;
    const int kMouseRegionButtom = 5;
    const int kMouseRegionRight = 5;
}

//鼠标区域,将窗口分成了9个部分
enum MousePosition
{
    kMousePositionLeftTop = 11,
    kMousePositionTop = 12,
    kMousePositionRightTop = 13,
    kMousePositionLeft = 21,
    kMousePositionMid = 22,
    kMousePositionRight = 23,
    kMousePositionLeftButtom = 31,
    kMousePositionButtom = 32,
    kMousePositionRightButtom = 33,
};

class QQMediaPlayerCopy : public QWidget
{
    Q_OBJECT

public:
    QQMediaPlayerCopy(QWidget *parent = Q_NULLPTR);

protected:
	void resizeEvent(QResizeEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
	
protected slots: 
    void On_Close();
    void On_ShowFullScreen();
    void On_ShowMiniMode();
	void on_openFile(const QStringList& fileList);
    void On_openRightPlaylist();
    void On_timer();

private:
    //根据鼠标的设置鼠标样式，用于拉伸
    void SetMouseCursor(int x, int y);

    //判断鼠标的区域，用于拉伸
    int GetMouseRegion(int x, int y);

private:
    QPoint           last_point_;             //记录放大之前的位置
    QPoint           last_position_;          //窗口上一次的位置
    bool             left_button_pressed_ = false;   //鼠标左键按下
    int              mouse_press_region_ = kMousePositionMid; //鼠标点击的区域
    bool             mouse_moved_ = false;
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
