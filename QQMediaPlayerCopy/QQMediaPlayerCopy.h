/*

������ͷ�ļ�


���ߣ��������
�������ӣ�https://blog.csdn.net/yao_hou/category_9275800.html
QQȺ��894097803

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
//ffmpegͷ�ļ�
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libavfilter/avfilter.h"
#include "libpostproc/postprocess.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"

//SDLͷ�ļ�
#include "SDL.h"
}

using std::queue;
using std::list;
using std::mutex;

#define  MAX_VQUEUE 300                  //ͼƬ֡���б���ֵ

// ��Ƶ����״̬
enum PLAY_TYPE
{
	PLAY_TYPE_PLAY,  //����
	PLAY_TYPE_PAUSE, //��ͣ
	PLAY_TYPE_STOP,  //ֹͣ
	PLAY_TYPE_MAX
};

// ��Ƶbuffer
struct ST_ABUF
{
	Uint8* buf;
	int buf_len;
	int pts;    //��ǰ����������ʱ���
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
	HANDLE           m_hSupendEvent;   // ��ͣ�ź�
	HANDLE           m_hDecodeEvent;   // �����߳���ͣ��־
	HANDLE           m_hVideoEvent;    // VideoThread��ͣ��־
	HANDLE           m_hAudioEvent;    // AudioThread��ͣ��־
	HANDLE           m_hACallEvent;    // AudioCallPlay��ͣ��־
	HANDLE           m_hDecodeHandel;  // �����߳̾��
	HANDLE           m_hVPlayHandel;   // ��Ƶ�����߳̾��
	HANDLE           m_hAPlayHandel;   // ��Ƶ�����߳̾��

	bool                       m_bReadPacketEnd;
	std::atomic<PLAY_TYPE>     m_PlayType; //����״̬

	AVFormatContext* m_pFormatCtx;

	//��Ƶ����
	int              m_VideoStream_Index;  //��Ƶ������
	AVCodecContext*  m_pVideoCodecCtxOrg;  //ԭʼ����Ƶ������������
	AVCodecContext*  m_pvCodecCtx;
	AVCodec*         m_pvCodec;
	int              m_FPS = 25;     //����Ĭ��ֵ25 
	queue<AVPacket>  m_VideoList;    //�����Ƶ��AVPacket
	mutex            m_VMutex;       //��m_VideoList�����Ļ�����
	mutex            m_pSizeMutex;   //�Ըı䴰�ڴ�С�Ļ�����

	//��Ƶ����
	int              m_AudioStream_Index;  //��Ƶ������
	AVCodecContext*  m_paCodecCtxOrg;
	AVCodecContext*  m_paCodecCtx;
	AVCodec*         m_paCodec;
	queue<ST_ABUF>   m_qAbuf;    //�������У���Ž���������
	list<AVPacket>   m_AudioList;//�����Ƶ��AVPacket
	mutex            m_AMutex;   //��m_AudioList�����Ļ�����
	mutex            m_CMutex;   //��m_qAbuf�����Ļ�����

	//SDL����
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
