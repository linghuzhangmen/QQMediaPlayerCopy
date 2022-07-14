/*

主界面头文件


作者：令狐掌门
博客链接：https://blog.csdn.net/yao_hou/category_9275800.html
QQ群：894097803

*/

#pragma once

#include <QtWidgets/QWidget>
#include "ui_QQMediaPlayerCopy.h"
#include "CFrameLessWidgetBase.h"
#include "CTitleBar.h"
#include "CPlayCtrlBar.h"
#include "VideoWidget.h"
#include <queue>
#include <list>
#include <mutex>
#include <Windows.h>
#include "CPlayListWidget.h"

extern "C"
{
//ffmpeg头文件
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libavfilter/avfilter.h"
#include "libpostproc/postprocess.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"

//SDL头文件
#include "SDL.h"
}

using std::queue;
using std::list;
using std::mutex;

#define  MAX_VQUEUE 300                  //图片帧队列饱和值

// 视频播放状态
enum PLAY_TYPE
{
	PLAY_TYPE_PLAY,  //播放
	PLAY_TYPE_PAUSE, //暂停
	PLAY_TYPE_STOP,  //停止
	PLAY_TYPE_MAX
};

// 音频buffer
struct ST_ABUF
{
	Uint8* buf;
	int buf_len;
	int pts;    //当前播放声音的时间戳
};


class QQMediaPlayerCopy : public CFrameLessWidgetBase
{
    Q_OBJECT

public:
    QQMediaPlayerCopy(QWidget *parent = Q_NULLPTR);

private:
	void keyPressEvent(QKeyEvent* event) override;
	void play(const std::string& _fileName);
	
public:
	void initSDL();
	void SetPlayType(PLAY_TYPE type);
	PLAY_TYPE GetPlayType();
	void ClearQueue();
	void Release();
	void setTimeSliderPos(int pos);

protected slots: 
	void on_openFile(const QStringList& fileList);
	void on_openFloder(QString floderName);
	void on_openPlaylist();
	void On_Close();
	void on_play();
	void on_stop();
	void on_fullScreen();
	void on_UpdatePlay(const QString& fileName);

public:
	HANDLE           m_hSupendEvent;   // 暂停信号
	HANDLE           m_hDecodeEvent;   // 解码线程暂停标志
	HANDLE           m_hVideoEvent;    // VideoThread暂停标志
	HANDLE           m_hAudioEvent;    // AudioThread暂停标志
	HANDLE           m_hACallEvent;    // AudioCallPlay暂停标志
	HANDLE           m_hDecodeHandel;  // 解码线程句柄
	HANDLE           m_hVPlayHandel;   // 视频播放线程句柄
	HANDLE           m_hAPlayHandel;   // 音频播放线程句柄

	bool                       m_bReadPacketEnd;
	std::atomic<PLAY_TYPE>     m_PlayType; //播放状态

	AVFormatContext* m_pFormatCtx;

	//视频数据
	int              m_VideoStream_Index;  //视频流索引
	AVCodecContext*  m_pVideoCodecCtxOrg;  //原始的视频解码器上下文
	AVCodecContext*  m_pvCodecCtx;
	AVCodec*         m_pvCodec;
	int              m_FPS = 25;     //给个默认值25 
	queue<AVPacket>  m_VideoList;    //存放视频的AVPacket
	mutex            m_VMutex;       //对m_VideoList操作的互斥锁
	mutex            m_pSizeMutex;   //对改变窗口大小的互斥锁

	//音频数据
	int              m_AudioStream_Index;  //音频流索引
	AVCodecContext*  m_paCodecCtxOrg;
	AVCodecContext*  m_paCodecCtx;
	AVCodec*         m_paCodec;
	queue<ST_ABUF>   m_qAbuf;    //声音队列，存放解码后的数据
	list<AVPacket>   m_AudioList;//存放音频的AVPacket
	mutex            m_AMutex;   //对m_AudioList操作的互斥锁
	mutex            m_CMutex;   //对m_qAbuf操作的互斥锁

	//SDL数据
	SDL_Window*   m_SDL_Window;
	SDL_Renderer* m_SDL_Render;
	SDL_Texture*  m_SDL_Texture;

private:
    Ui::QQMediaPlayerCopyClass ui;

	CTitleBar*       m_pTitle = nullptr;
	CPlayCtrlBar*    m_pPlayCtrlBar = nullptr;
	VideoWidget*     m_pVideoWidget = nullptr;
	CPlayListWidget* m_pPlaylistWidget = nullptr;
	bool          m_bPlaylistIsOpen = false;
	QStringListModel* m_pStringListModel = nullptr;
	std::string        m_curPlayFileName;
};
