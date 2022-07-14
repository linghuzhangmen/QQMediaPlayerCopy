#pragma execution_character_set("utf-8")

#include "QQMediaPlayerCopy.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <string>
#include <iostream>
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include <QStringListModel>

using namespace std;

DWORD WINAPI DecodeThread(LPVOID lpParam);
DWORD WINAPI VideoThread(LPVOID lpParam);
DWORD WINAPI AudioThread(LPVOID lpParam);

//防止中文乱码
string qstr2str(const QString& qstr)
{
	QByteArray cdata = qstr.toLocal8Bit();
	return string(cdata);
}

QQMediaPlayerCopy::QQMediaPlayerCopy(QWidget *parent)
    : CFrameLessWidgetBase(parent)
{
    ui.setupUi(this);

	setStyleSheet("QWidget{background-color:rgb(17,17,17);}");

	QVBoxLayout* pVLay = new QVBoxLayout(this);

	m_pTitle = new CTitleBar(this);
	m_pVideoWidget = new VideoWidget(this);
	m_pPlayCtrlBar = new CPlayCtrlBar(this);
	m_pPlaylistWidget = new CPlayListWidget(this);

	pVLay->addWidget(m_pTitle);

	QHBoxLayout* pHLay = new QHBoxLayout(this);
	pHLay->addWidget(m_pVideoWidget);
	pHLay->addWidget(m_pPlaylistWidget);
	pVLay->addLayout(pHLay);

	pVLay->addWidget(m_pPlayCtrlBar);
	pVLay->setContentsMargins(0, 0, 0, 0);
	setLayout(pVLay);

	m_pPlaylistWidget->hide();  //播放列表默认隐藏

	connect(m_pTitle, &CTitleBar::sig_close, this, &QQMediaPlayerCopy::On_Close);
	connect(m_pVideoWidget, &VideoWidget::sig_OpenFile, this, &QQMediaPlayerCopy::on_openFile);
	connect(m_pVideoWidget, &VideoWidget::sig_OpenFloder, this, &QQMediaPlayerCopy::on_openFloder);
	connect(m_pVideoWidget, &VideoWidget::sig_OpenPlaylist, this, &QQMediaPlayerCopy::on_openPlaylist);
	connect(m_pPlayCtrlBar, &CPlayCtrlBar::sig_play, this, &QQMediaPlayerCopy::on_play);
	connect(m_pPlayCtrlBar, &CPlayCtrlBar::sig_stop, this, &QQMediaPlayerCopy::on_stop);
	connect(m_pPlayCtrlBar, &CPlayCtrlBar::sig_fullScreen, this, &QQMediaPlayerCopy::on_fullScreen);
	connect(m_pPlaylistWidget, &CPlayListWidget::sig_fileName, this, &QQMediaPlayerCopy::on_UpdatePlay);

	m_pFormatCtx = nullptr;
	m_pVideoCodecCtxOrg = nullptr;
	m_pvCodecCtx = nullptr;
	m_pvCodec = nullptr;
	m_VideoStream_Index = -1;
	m_paCodecCtxOrg = nullptr;
	m_paCodecCtx = nullptr;
	m_paCodec = nullptr;

	m_SDL_Window = nullptr;
	m_SDL_Render = nullptr;
	m_SDL_Texture = nullptr;

	m_AudioStream_Index = -1;
	m_hSupendEvent = CreateEvent(NULL, TRUE, TRUE, NULL); 
	m_hDecodeEvent = CreateEvent(NULL, TRUE, TRUE, NULL); 
	m_hVideoEvent = CreateEvent(NULL, TRUE, TRUE, NULL); 
	m_hAudioEvent = CreateEvent(NULL, TRUE, TRUE, NULL); 
	m_hACallEvent = CreateEvent(NULL, TRUE, TRUE, NULL); 
	m_hSupendEvent = CreateEvent(NULL, TRUE, TRUE, NULL); 
	m_hDecodeHandel = INVALID_HANDLE_VALUE;
	m_hVPlayHandel = INVALID_HANDLE_VALUE;
	m_hAPlayHandel = INVALID_HANDLE_VALUE;

	initSDL();
}

void QQMediaPlayerCopy::on_openFile(const QStringList& fileList)
{
	m_pPlaylistWidget->show();

	// 创建数据模型
	m_pStringListModel = new QStringListModel(this);

	// 为模型设置StringList，会导入StringList的内容
	m_pStringListModel->setStringList(fileList);

	// 为listView设置数据模型
	m_pPlaylistWidget->setPlayList(m_pStringListModel);

	// 默认播放第一个文件
	string _fileName = qstr2str(fileList[0]);
	play(_fileName);
}

void QQMediaPlayerCopy::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Escape:
		{
			if (this->isFullScreen())
			{
				this->showNormal();
				m_pPlayCtrlBar->show();
				m_pTitle->show();
				m_pVideoWidget->hideCtrl(false);

				if (m_PlayType == PLAY_TYPE_PLAY)
				{
					m_pVideoWidget->ShowOpenFileButton(false);
				}
			}
		}
		break;

	default:
		QWidget::keyPressEvent(event);
	}
}

void QQMediaPlayerCopy::play(const std::string& _fileName)
{
	// 打开新的视频前，先释放之前的一些内存
	Release();

	m_curPlayFileName = _fileName;

	// 打开文件，读取文件头，将格式相关信息存放在AVFormatContext结构体中
	if (avformat_open_input(&m_pFormatCtx, _fileName.c_str(), nullptr, nullptr) != 0)
	{
		QMessageBox::information(this, u8"提示", u8"open file failed");
		return;
	}

	// 检测文件的流信息
	if (avformat_find_stream_info(m_pFormatCtx, nullptr) < 0)
	{
		QMessageBox::information(this, u8"提示", u8"no stream info");
		return;
	}

	//查找视频流 音频流
	for (int i = 0; i < m_pFormatCtx->nb_streams; i++)
	{
		if (m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			m_VideoStream_Index = i;
		else if (m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
			m_AudioStream_Index = i;
	}

	if (m_VideoStream_Index == -1)
	{
		QMessageBox::information(this, u8"提示", u8"没有找到视频流");
		return;
	}

	AVStream* vStream = m_pFormatCtx->streams[m_VideoStream_Index];
	m_pVideoCodecCtxOrg = vStream->codec;

	// 找到视频解码器
	m_pvCodec = avcodec_find_decoder(m_pVideoCodecCtxOrg->codec_id);
	if (!m_pvCodec)
	{
		QMessageBox::information(this, "Warn", u8"find video decoder failed");
		return;
	}

	// 不直接使用从AVFormatContext得到的CodecContext，要复制一个
	m_pvCodecCtx = avcodec_alloc_context3(m_pvCodec);
	if (avcodec_copy_context(m_pvCodecCtx, m_pVideoCodecCtxOrg) != 0)
	{
		QMessageBox::information(this, u8"提示", u8"Could not copy vcodec context!");
		return;
	}

	if (m_AudioStream_Index >= 0)
	{
		AVStream* aStream = m_pFormatCtx->streams[m_AudioStream_Index];
		m_paCodecCtxOrg = aStream->codec;

		// 找到音频解码器
		m_paCodec = avcodec_find_decoder(m_paCodecCtxOrg->codec_id);
		if (!m_paCodec)
		{
			QMessageBox::information(this, u8"提示", u8"查找音频解码器失败!");
			return;
		}

		m_paCodecCtx = avcodec_alloc_context3(m_paCodec);
		if (avcodec_copy_context(m_paCodecCtx, m_paCodecCtxOrg) != 0)
		{
			QMessageBox::information(this, u8"提示", u8"Could not copy acodec context!");
			return;
		}
	}

	double time = (double)(m_pFormatCtx->duration) / AV_TIME_BASE; //时长
	int ms = time * 1000;
	int tims_seconds = (int)time;

	int h = tims_seconds / 3600;
	int min = (tims_seconds - h * 3600) / 60;
	int sec = tims_seconds - h * 3600 - min * 60;

	m_pVideoWidget->setTimeSliderRange(ms);

	//设置时间
	QString totalTime = QString("%1:%2:%3").arg(h, 2, 10, QLatin1Char('0')).arg(min, 2, 10, QLatin1Char('0')).arg(sec, 2, 10, QLatin1Char('0'));

	if (vStream->r_frame_rate.den > 0)
		m_FPS = av_q2d(vStream->r_frame_rate);
	else if (m_pVideoCodecCtxOrg->framerate.den > 0)
		m_FPS = av_q2d(m_pVideoCodecCtxOrg->framerate);

	if (m_PlayType == PLAY_TYPE_PAUSE)
	{
		SetPlayType(PLAY_TYPE_PLAY);
	}
	else if (m_PlayType == PLAY_TYPE_STOP)
	{
		SetPlayType(PLAY_TYPE_PLAY);
		m_hDecodeHandel = CreateThread(NULL, NULL, DecodeThread, this, 0, 0);
	}

	m_pPlayCtrlBar->showTimeLabel(true);
	m_pPlayCtrlBar->setTotalTime(totalTime);

	m_pVideoWidget->setPlay(true);
	m_pVideoWidget->ShowOpenFileButton(false);
}

void QQMediaPlayerCopy::initSDL()
{
	//ffmpeg注册
	av_register_all();

	//设置视频播放状态
	SetPlayType(PLAY_TYPE_STOP);

	//SDL初始化：视频、音频、定时器都要初始化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
	{
		QMessageBox::information(this, "tips", "sdl init error");
		return;
	}

	//使用QWidget创建SDL窗口
	HWND picHwnd = (HWND)(m_pVideoWidget->playWidget()->winId());
	m_SDL_Window = SDL_CreateWindowFrom(picHwnd);
	if (!m_SDL_Window)
	{
		QMessageBox::information(this, "tips", "SDL_CreateWindowFrom error");
		return;
	}
}

void QQMediaPlayerCopy::SetPlayType(PLAY_TYPE type)
{
	m_PlayType = type;
	switch (type)
	{
	case PLAY_TYPE_PLAY:
	{
		SetEvent(m_hSupendEvent);
		break;
	}

	case PLAY_TYPE_PAUSE:
	{
		ResetEvent(m_hSupendEvent);
		break;
	}

	case PLAY_TYPE_STOP:
	{
		SetEvent(m_hSupendEvent);
		if (m_hDecodeHandel != INVALID_HANDLE_VALUE)
		{
			WaitForSingleObject(m_hDecodeHandel, INFINITE);
			m_hDecodeHandel = INVALID_HANDLE_VALUE;
		}

		if (m_hVPlayHandel != INVALID_HANDLE_VALUE)
		{
			WaitForSingleObject(m_hVPlayHandel, INFINITE);
			m_hVPlayHandel = INVALID_HANDLE_VALUE;
		}

		if (m_hAPlayHandel != INVALID_HANDLE_VALUE)
		{
			WaitForSingleObject(m_hAPlayHandel, INFINITE);
			m_hAPlayHandel = INVALID_HANDLE_VALUE;
		}

		ClearQueue();
		
		break;
	}
	}
}

PLAY_TYPE QQMediaPlayerCopy::GetPlayType()
{
	return m_PlayType;
}

void QQMediaPlayerCopy::ClearQueue()
{
	while (!m_qAbuf.empty())
	{
		ST_ABUF* stBuf = &m_qAbuf.front();
		delete[] stBuf->buf;
		m_qAbuf.pop();
	}

	while (!m_VideoList.empty())
	{
		AVPacket* pkt = &m_VideoList.front();
		av_free_packet(pkt);
		m_VideoList.pop();
	}

	while (!m_AudioList.empty())
	{
		AVPacket* pkt = &m_AudioList.front();
		av_free_packet(pkt);
		m_AudioList.pop_front();
	}
}

void  SDL_AudioCallPlay(void* udata, Uint8* stream, int len)
{
	QQMediaPlayerCopy* pThis = (QQMediaPlayerCopy*)udata;
	SDL_memset(stream, 0, len);

	std::lock_guard<mutex> mtx_locker(pThis->m_CMutex);
	if (!pThis->m_qAbuf.empty())
	{
		static int send_len = 0;

		ST_ABUF* st_buf = &pThis->m_qAbuf.front();
		int cp_len = (len > st_buf->buf_len ? st_buf->buf_len : len);
		if (cp_len > 0)
		{
			memcpy(stream, st_buf->buf + send_len, cp_len);
			send_len += cp_len;
			st_buf->buf_len -= cp_len;
			if (st_buf->buf_len == 0)
			{
				send_len = 0;
				delete[] st_buf->buf;
				pThis->m_qAbuf.pop();
			}
		}

		SetEvent(pThis->m_hACallEvent);
		WaitForSingleObject(pThis->m_hSupendEvent, INFINITE); //暂停后，此处会一直等待，直到SetEvent(m_hSupendEvent)
		ResetEvent(pThis->m_hACallEvent);
	}
}

int GetSampleCount(AVSampleFormat out_sample_fmt)
{
	int count = 0;
	switch (out_sample_fmt)
	{
	case AV_SAMPLE_FMT_S32:count = 4; break;
	case AV_SAMPLE_FMT_S16:count = 2; break;
	case AV_SAMPLE_FMT_U8: count = 1; break;
	}
	return count;
}

void QQMediaPlayerCopy::on_openFloder(QString floderName)
{

}

//打开播放列表
void QQMediaPlayerCopy::on_openPlaylist()
{
	m_bPlaylistIsOpen ? m_pPlaylistWidget->show() : m_pPlaylistWidget->hide();
	m_bPlaylistIsOpen = !m_bPlaylistIsOpen;
}

//关闭主界面
void QQMediaPlayerCopy::On_Close()
{
	SetPlayType(PLAY_TYPE_STOP);
	close();
}

//播放与暂停
void QQMediaPlayerCopy::on_play()
{
	if (m_PlayType == PLAY_TYPE_PLAY)
	{
		SetPlayType(PLAY_TYPE_PAUSE);
	}
	else if (m_PlayType == PLAY_TYPE_PAUSE)
	{
		SetPlayType(PLAY_TYPE_PLAY);
	}
	else if (m_PlayType == PLAY_TYPE_STOP)
	{
		//如果上一个状态是停止播放
		/*QStringList playlist = m_pPlaylistWidget->getPlaylist();
		if (playlist.size() > 0)
		{
			QString firstFileName = playlist[0];
		}*/

		play(m_curPlayFileName);
	}
}

void QQMediaPlayerCopy::on_stop()
{
	Release();
	m_pVideoWidget->update();
	m_pVideoWidget->ShowOpenFileButton(true);
}

void QQMediaPlayerCopy::on_fullScreen()
{
	if (this->isFullScreen())
	{
		this->showNormal();
	}
	else
	{
		this->showFullScreen();
		m_pPlayCtrlBar->hide();
		m_pTitle->hide();
		m_pPlaylistWidget->hide();
		m_pVideoWidget->hideCtrl(true);
	}
}

//双击播放列表后重新播放
void QQMediaPlayerCopy::on_UpdatePlay(const QString& fileName)
{
	string _fileName = qstr2str(fileName);
	play(_fileName);
}

void QQMediaPlayerCopy::Release()
{
	SetPlayType(PLAY_TYPE_STOP);

	m_VideoStream_Index = -1;
	m_AudioStream_Index = -1;
	if (m_pVideoCodecCtxOrg)
	{
		avcodec_close(m_pVideoCodecCtxOrg);
		m_pVideoCodecCtxOrg = nullptr;
	}

	if (m_pvCodecCtx)
	{
		avcodec_close(m_pvCodecCtx);
		m_pvCodecCtx = nullptr;
	}

	if (m_paCodecCtxOrg)
	{
		avcodec_close(m_paCodecCtxOrg);
		m_paCodecCtxOrg = nullptr;
	}

	if (m_paCodecCtx)
	{
		avcodec_close(m_paCodecCtx);
		m_paCodecCtx = nullptr;
	}

	if (m_pFormatCtx)
	{
		avformat_close_input(&m_pFormatCtx);
		m_pFormatCtx = nullptr;
	}

	if (m_SDL_Render)
	{
		SDL_DestroyRenderer(m_SDL_Render);
		m_SDL_Render = nullptr;
	}

	if (m_SDL_Texture)
	{
		SDL_DestroyTexture(m_SDL_Texture);
		m_SDL_Texture = nullptr;
	}
}

void QQMediaPlayerCopy::setTimeSliderPos(int pos)
{
	if (m_pVideoWidget)
	{
		m_pVideoWidget->setTimeSliderPos(pos);
	}
}

//解码线程，将解码后的AVPacket放入队列m_VideoList和m_AudioList中
DWORD WINAPI DecodeThread(LPVOID lpParam)
{
	QQMediaPlayerCopy* pThis = (QQMediaPlayerCopy*)lpParam;

	do
	{
		if (!pThis->m_pVideoCodecCtxOrg || !pThis->m_pvCodec)
		{
			return -1;
		}

		if (avcodec_open2(pThis->m_pvCodecCtx, pThis->m_pvCodec, nullptr) < 0)
		{
			return -1;
		}

		//创建视频解码显示，音频解码播放线程
		pThis->m_hVPlayHandel = CreateThread(NULL, NULL, VideoThread, pThis, 0, 0);
		pThis->m_hAPlayHandel = CreateThread(NULL, NULL, AudioThread, pThis, 0, 0);

		//播放前先seek到起点
		av_seek_frame(pThis->m_pFormatCtx, pThis->m_VideoStream_Index, 0, AVSEEK_FLAG_BACKWARD);

		while (true)
		{
			SetEvent(pThis->m_hDecodeEvent);  //若为暂停状态，执行到此处才表示该线程真正暂停了
			WaitForSingleObject(pThis->m_hSupendEvent, INFINITE);  //暂停后，此处会一直等待，直到SetEvent(m_hSupendEvent)
			ResetEvent(pThis->m_hDecodeEvent);
			if (pThis->GetPlayType() == PLAY_TYPE_STOP)
			{
				break;
			}

			{
				std::lock_guard<mutex> mtx_locker(pThis->m_VMutex);
				int v_size = pThis->m_VideoList.size();
				if (v_size >= MAX_VQUEUE)
				{
					//当队列饱和时等待播放线程消耗再继续解码放入，否则内存占用越来越大
					continue;
				}
			}

			AVPacket packet;
			if (av_read_frame(pThis->m_pFormatCtx, &packet) >= 0)
			{
				pThis->m_bReadPacketEnd = false;
				if (packet.stream_index == pThis->m_VideoStream_Index)
				{
					std::lock_guard<mutex> mtx_locker(pThis->m_VMutex);
					pThis->m_VideoList.push(packet);
				}
				else if (packet.stream_index == pThis->m_AudioStream_Index)
				{
					std::lock_guard<mutex> mtx_locker(pThis->m_AMutex);
					pThis->m_AudioList.push_back(packet);
				}
			}
			else
			{
				pThis->m_bReadPacketEnd = true; //若此时播放还未结束，将进度条向前拉，则m_bReadPacketEnd = false
			}
		}
	} while (0);

	return 0;
}

//图片播放线程，从队列m_qVbuf中取出图片帧数据通过SDL接口显示
DWORD WINAPI VideoThread(LPVOID lpParam)
{
	QQMediaPlayerCopy* pThis = (QQMediaPlayerCopy*)lpParam;

	if (!pThis->m_SDL_Render && !pThis->m_SDL_Texture)
	{
		//创建SDL渲染器和纹理
		pThis->m_SDL_Render = SDL_CreateRenderer(pThis->m_SDL_Window, -1, 0);
		pThis->m_SDL_Texture = SDL_CreateTexture(pThis->m_SDL_Render, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, pThis->m_pvCodecCtx->width, pThis->m_pvCodecCtx->height);
	}

	AVFrame* pFrame = av_frame_alloc();  //存放像素转换前的视频帧信息
	AVFrame* pFrameYUV420 = av_frame_alloc();

	// 使用的缓冲区的大小
	int numBytes = 0;
	uint8_t* buffer = nullptr;

	numBytes = avpicture_get_size(AV_PIX_FMT_YUV420P, pThis->m_pvCodecCtx->width, pThis->m_pvCodecCtx->height);
	buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));

	avpicture_fill((AVPicture*)pFrameYUV420, buffer, AV_PIX_FMT_YUV420P, pThis->m_pvCodecCtx->width, pThis->m_pvCodecCtx->height);

	struct SwsContext* sws_ctx = nullptr;
	sws_ctx = sws_getContext(pThis->m_pvCodecCtx->width, pThis->m_pvCodecCtx->height, pThis->m_pvCodecCtx->pix_fmt,
		pThis->m_pvCodecCtx->width, pThis->m_pvCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr, nullptr);

	while (true)
	{
		SetEvent(pThis->m_hVideoEvent);   //若为暂停状态，执行到此处才表示真正暂停了
		WaitForSingleObject(pThis->m_hSupendEvent, INFINITE);   //暂停后，此处会一直等待，直到SetEvent(m_hSupendEvent)
		ResetEvent(pThis->m_hVideoEvent);
		if (pThis->GetPlayType() == PLAY_TYPE_STOP)
		{
			break;
		}

		if (!pThis->m_VideoList.empty())
		{
			AVPacket* packet = &pThis->m_VideoList.front();
			if (packet->buf == NULL)
			{
				av_free_packet(packet);
				std::lock_guard<mutex> mtx_locker(pThis->m_VMutex);
				pThis->m_VideoList.pop();
				continue;
			}

			//视频解码
			int ret = avcodec_send_packet(pThis->m_pvCodecCtx, packet);
			if (ret < 0)
			{
				av_free_packet(packet);
				break;
			}

			while (ret >= 0)
			{
				ret = avcodec_receive_frame(pThis->m_pvCodecCtx, pFrame);
				if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
				{
					ret = 0;
					break;
				}
				else if (ret < 0)
					continue;

				//图像转换
				sws_scale(sws_ctx, (uint8_t const* const*)pFrame->data, pFrame->linesize, 0,
					pThis->m_pvCodecCtx->height, pFrameYUV420->data, pFrameYUV420->linesize);

				int pts = packet->pts > 0 ? packet->pts : packet->dts;

				//当前帧显示时间
				double sec = pts * av_q2d(pThis->m_pFormatCtx->streams[pThis->m_VideoStream_Index]->time_base);

				{
					std::lock_guard<mutex> mtx_locker(pThis->m_pSizeMutex);

					//通过SDL来显示
					SDL_UpdateTexture(pThis->m_SDL_Texture, NULL, pFrameYUV420->data[0], pFrameYUV420->linesize[0]);
					SDL_RenderClear(pThis->m_SDL_Render);
					SDL_RenderCopy(pThis->m_SDL_Render, pThis->m_SDL_Texture, NULL, NULL);
					SDL_RenderPresent(pThis->m_SDL_Render);
				}

				//更新进度条
				//pThis->PostMessageW(WM_UPDATE_SLIDER, NULL, sec * 1000);
				

				//每一帧显示时间,等待音频播放，音视频同步
				Sleep(1000 / pThis->m_FPS);

				pThis->setTimeSliderPos(sec * 1000);
			}

			av_free_packet(packet);
			std::lock_guard<mutex> mtx_locker(pThis->m_VMutex);
			pThis->m_VideoList.pop();
		}
		else if (pThis->m_bReadPacketEnd)
		{
			//解码结束且队列为空则播放完成
			break;
		}
	}

	av_free(buffer);
	av_frame_free(&pFrameYUV420);
	av_frame_free(&pFrame);

	if (pThis->GetPlayType() != PLAY_TYPE_STOP)
	{
		pThis->m_hVPlayHandel = INVALID_HANDLE_VALUE;
		pThis->SetPlayType(PLAY_TYPE_STOP);
	}

	return 0;
}

DWORD WINAPI AudioThread(LPVOID lpParam)
{
	QQMediaPlayerCopy* pThis = (QQMediaPlayerCopy*)lpParam;

	do
	{
	CoInitialize(NULL);
	if (!pThis->m_paCodecCtxOrg || !pThis->m_paCodec)
	{
		return -1;
	}
	if (avcodec_open2(pThis->m_paCodecCtx, pThis->m_paCodec, nullptr) != 0)
	{
		return -1;
	}

	//输入的采样格式
	enum AVSampleFormat in_sample_fmt = pThis->m_paCodecCtx->sample_fmt;

	//输出的采样格式 16bit PCM
	enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;

	//输入的采样率
	int in_sample_rate = pThis->m_paCodecCtx->sample_rate;

	//输出的采样率
	int out_sample_rate = in_sample_rate;

	//输入的声道布局
	uint64_t in_ch_layout = av_get_default_channel_layout(pThis->m_paCodecCtx->channels);

	//输出的声道布局
	uint64_t out_ch_layout = av_get_default_channel_layout(2);

	//输出通道数
	int out_channels = av_get_channel_layout_nb_channels(out_ch_layout);
	int out_nb_samples = pThis->m_paCodecCtx->frame_size;

	SDL_AudioSpec wanted_spec;
	wanted_spec.freq = out_sample_rate;
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.channels = out_channels;
	wanted_spec.silence = 0;
	wanted_spec.samples = out_nb_samples;
	wanted_spec.callback = SDL_AudioCallPlay;   //设置音频回调
	wanted_spec.userdata = pThis;

	//打开音频设备
	if (SDL_OpenAudio(&wanted_spec, NULL) < 0)
	{
		return -1;
	}

	struct SwrContext* au_convert_ctx = swr_alloc();

	au_convert_ctx = swr_alloc_set_opts(
		au_convert_ctx,
		out_ch_layout,
		out_sample_fmt,
		out_sample_rate,
		in_ch_layout,
		pThis->m_paCodecCtx->sample_fmt,
		pThis->m_paCodecCtx->sample_rate,
		0,
		NULL);

	swr_init(au_convert_ctx);

	//播放音频
	SDL_PauseAudio(0);

	int ret = 0;
	AVPacket packet;
	uint8_t* out_buffer = (uint8_t*)av_malloc(2 * out_sample_rate);
	AVFrame* pFrame = av_frame_alloc();

	while (true)
	{
		SetEvent(pThis->m_hAudioEvent);
		WaitForSingleObject(pThis->m_hSupendEvent, INFINITE);
		ResetEvent(pThis->m_hAudioEvent);
		if (pThis->GetPlayType() == PLAY_TYPE_STOP)
		{
			av_seek_frame(pThis->m_pFormatCtx, pThis->m_AudioStream_Index, 0, AVSEEK_FLAG_BACKWARD);
			break;
		}

		AVPacket* packet = nullptr;
		{
			std::lock_guard<mutex> mtx_locker(pThis->m_AMutex);
			if (!pThis->m_AudioList.empty())
			{
				packet = &pThis->m_AudioList.front();
				if (packet->buf == NULL)
				{
					av_free_packet(packet);
					pThis->m_AudioList.pop_front();
					continue;
				}
			}
			else
				continue;
		}

		ret = avcodec_send_packet(pThis->m_paCodecCtx, packet);
		if (ret < 0)
		{
			break;
		}

		while (ret >= 0)
		{
			ret = avcodec_receive_frame(pThis->m_paCodecCtx, pFrame);
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			{
				break;
			}
			else if (ret < 0)
			{
				continue;
			}
			else
			{
				int nRet = swr_convert(au_convert_ctx, &out_buffer, 2 * out_sample_rate, (const uint8_t**)pFrame->data, pFrame->nb_samples);
				int buf_len = nRet * out_channels * GetSampleCount(out_sample_fmt);

				ST_ABUF st_buf;
				st_buf.buf = new Uint8[2 * out_sample_rate];
				memset(st_buf.buf, 0, 2 * out_sample_rate);
				memcpy(st_buf.buf, out_buffer, buf_len);
				st_buf.buf_len = buf_len;
				st_buf.pts = packet->pts > 0 ? packet->pts : packet->dts;

				//将解码的音频数据放入队列
				std::lock_guard<mutex> mtx_locker(pThis->m_CMutex);
				pThis->m_qAbuf.push(st_buf);
			}
		}

		av_free_packet(packet);
		std::lock_guard<mutex> mtx_locker(pThis->m_AMutex);
		pThis->m_AudioList.pop_front();
	}

	av_free(out_buffer);
	SDL_CloseAudio();
	swr_free(&au_convert_ctx);
	} while (0);

	return 0;
}