#include "CVlcKits.h"
#include <iostream>
#include <QDir>

using namespace std;

void vlc_callback(const struct libvlc_event_t* p_event, void* p_data)
{
    CVlcKits* pThis = static_cast<CVlcKits*>(p_data);

    if (pThis)
    {
        switch (p_event->type)
        {
        case libvlc_MediaPlayerPositionChanged:
        {
            float pos = libvlc_media_player_get_position(pThis->media_player());
            pThis->setTimeSliderPos(pos * 100);

            qint64 curSecs = libvlc_media_player_get_time(pThis->media_player()) / 1000;

            int curH = curSecs / 3600;
            int curMinute = (curSecs - curH * 3600) / 60;
            int curSec = curSecs - curH * 3600 - curMinute * 60;

            char buf1[256] = { 0 };
            sprintf(buf1, "%02d:%02d:%02d", curH, curMinute, curSec);

            QString str1(buf1);

            qint64 totalSecs = pThis->getDuration();

            int _hour = totalSecs / 3600;
            int _minute = (totalSecs - _hour * 3600) / 60;
            int _sec = totalSecs - _hour * 3600 - _minute * 60;

            char buf2[256] = { 0 };
            sprintf(buf2, "%02d:%02d:%02d", _hour, _minute, _sec);

            QString str2(buf2);

            QString text = str1 + "/" + str2;

            pThis->setTimeText(text);
        }
        break;

        case libvlc_MediaPlayerAudioVolume:
        {
            int volume = libvlc_audio_get_volume(pThis->media_player());
            pThis->setVolumeSliderPos(volume);
        }
        break;

        case libvlc_MediaPlayerMediaChanged:
        {
            std::cout << "libvlc_MediaPlayerMediaChanged" << endl;

            pThis->addCurrentIndex();
            int _index = pThis->getCurrentIndex();

            int vSize = pThis->getVecDuration().size();
            if (_index > vSize)
            {
                // 开启了循环播放
                // 4 5 6 7 8 9 
                if (_index % vSize == 0)
                {
                    _index = vSize;
                }
                else
                {
                    _index = _index % vSize;
                }
            }

            libvlc_time_t _duration = pThis->getVecDuration()[_index - 1];
            pThis->setCurrentDuration(_duration);
        }
        break;


        // 加载完成
        //case libvlc_MediaPlayerVout:
        //{
        //    emit obj->playAllTime(obj->GetTime() / 1000);
        //    break;
        //}
        //// media player 位置改变
        //case libvlc_MediaPlayerPositionChanged:
        //{
        //    int time = obj->GetPlayTime();
        //    emit obj->playCurrentTime(time / 1000);
        //    break;
        //}
        //// 播放完成
        //case libvlc_MediaPlayerEndReached:
        //{
        //    obj->GetPlayState();
        //    obj->PlayEnd();
        //    break;
        //}
        }
    }
    
}

CVlcKits::CVlcKits()
{
}

CVlcKits::~CVlcKits()
{
    if (m_pMediaPlayer)
    {
        libvlc_media_player_release(m_pMediaPlayer);
        m_pMediaPlayer = nullptr;
    }

    // 最后释放
    if (m_pInstance)
    {
        libvlc_release(m_pInstance);
        m_pInstance = nullptr;
    }
}

/// <summary>
/// 初始化libvlc
/// </summary>
/// <returns>
/// 0 success
/// -1, libvlc_new failed
/// -2, libvlc_media_player_new failed
/// </returns>
int CVlcKits::initVLC()
{
    m_pInstance = libvlc_new(0, nullptr);

    if (m_pInstance)
    {
        m_pMediaPlayer = libvlc_media_player_new(m_pInstance);

        if (m_pMediaPlayer)
        {
            m_pEvent_manager = libvlc_media_player_event_manager(m_pMediaPlayer);

            libvlc_event_attach(m_pEvent_manager, libvlc_MediaPlayerPositionChanged, vlc_callback, this);
            libvlc_event_attach(m_pEvent_manager, libvlc_MediaPlayerAudioVolume, vlc_callback, this);
            libvlc_event_attach(m_pEvent_manager, libvlc_MediaPlayerMediaChanged, vlc_callback, this);
        }
        else
        {
            libvlc_release(m_pInstance);
            return -2;  // libvlc_media_player_new failed
        }
    }
    else
    {
        return -1;  // libvlc_new failed
    }

    return 0;
}

libvlc_media_player_t* CVlcKits::media_player()
{
	return m_pMediaPlayer;
}

void CVlcKits::setTimeSliderPos(int value)
{
    emit sig_TimeSliderPos(value);
}

void CVlcKits::setVolumeSliderPos(int value)
{
    emit sig_VolumeSliderPos(value);
}

void CVlcKits::setTimeText(const QString& str)
{
    emit sig_UpdateTimeText(str);
}

void CVlcKits::setCurrentDuration(libvlc_time_t value)
{
    m_CurrentDuration = value;
}

libvlc_time_t CVlcKits::getDuration()
{
    return m_CurrentDuration;
}

int CVlcKits::play(QStringList fileList, void* hwnd)
{
    m_pMediaPlayerList = libvlc_media_list_player_new(m_pInstance);
    m_medialist = libvlc_media_list_new(m_pInstance);

    int size = fileList.size();
    for (int i = 0; i < size; i++)
    {
        QString fileName = fileList[i];
        fileName = QDir::toNativeSeparators(fileName);

        libvlc_media_t* _pMedia = libvlc_media_new_path(m_pInstance, fileName.toStdString().c_str());
        if (!_pMedia)
        {
            return -1;
        }

        libvlc_media_list_add_media(m_medialist, _pMedia);

        //cout << "before parse" << endl;
        libvlc_media_parse(_pMedia);
        //cout << "after parse" << endl;
        libvlc_time_t _duration = libvlc_media_get_duration(_pMedia);
        if (_duration < -1)
        {
            return -2;
        }

        //cout << "before push" << endl;
        m_vecDurations.push_back(_duration / 1000);
        //cout << "after push" << endl;

        libvlc_media_release(_pMedia);
    }

    // 设置播放循环模式
    libvlc_media_list_player_set_playback_mode(m_pMediaPlayerList, libvlc_playback_mode_loop);

    libvlc_media_list_player_set_media_list(m_pMediaPlayerList, m_medialist);
    libvlc_media_list_player_set_media_player(m_pMediaPlayerList, m_pMediaPlayer);
    libvlc_media_player_set_hwnd(m_pMediaPlayer, hwnd);
    libvlc_media_list_player_play(m_pMediaPlayerList);

    return 0;
}

// 播放网络流
int CVlcKits::play(const QString& url, void* hwnd)
{
    libvlc_media_t* m;
    m = libvlc_media_new_location(m_pInstance, url.toStdString().data());
    if (m)
    {
        m_pMediaPlayer = libvlc_media_player_new_from_media(m);
        if (m_pMediaPlayer)
        {
            libvlc_media_parse(m);
            m_pEvent_manager = libvlc_media_player_event_manager(m_pMediaPlayer);
            libvlc_event_attach(m_pEvent_manager, libvlc_MediaPlayerVout, vlc_callback, this);
            libvlc_event_attach(m_pEvent_manager, libvlc_MediaPlayerEndReached, vlc_callback, this);
            libvlc_event_attach(m_pEvent_manager, libvlc_MediaPlayerPositionChanged, vlc_callback, this);
            if (hwnd != nullptr)
                libvlc_media_player_set_hwnd(m_pMediaPlayer, hwnd);
            
            //Play();
            if (m_pMediaPlayer)
            {
                return  libvlc_media_player_play(m_pMediaPlayer);
            }
        }
    }

    return 0;
}

void CVlcKits::play()
{
    if (libvlc_media_player_get_state(m_pMediaPlayer)
        == libvlc_state_t::libvlc_Paused)
    {
        libvlc_media_list_player_play(m_pMediaPlayerList);
    }
    else if(libvlc_media_player_get_state(m_pMediaPlayer)
        == libvlc_state_t::libvlc_Playing)
    {
        libvlc_media_list_player_pause(m_pMediaPlayerList);
    }   
}

void CVlcKits::pause()
{
    if (libvlc_media_player_get_state(m_pMediaPlayer)
        == libvlc_state_t::libvlc_Playing)
    {
        libvlc_media_player_pause(m_pMediaPlayer);
    }
}

void CVlcKits::stop()
{
    if (libvlc_media_player_get_state(m_pMediaPlayer)
        == libvlc_state_t::libvlc_Playing
        || libvlc_media_player_get_state(m_pMediaPlayer)
        == libvlc_state_t::libvlc_Paused)
    {
        libvlc_media_player_stop(m_pMediaPlayer);
    }
}

vector<libvlc_time_t> CVlcKits::getVecDuration()
{
    return m_vecDurations;
}

int CVlcKits::getCurrentIndex()
{
    return m_CurrentInex;;
}

void CVlcKits::addCurrentIndex()
{
    m_CurrentInex++;
}

void CVlcKits::setVideoPostion(int value)
{
    libvlc_media_player_set_position(m_pMediaPlayer, value / 100.0);
}

// 设置播放速度
void CVlcKits::setPlayRate(double rate)
{
    if(m_pMediaPlayer)
        libvlc_media_player_set_rate(m_pMediaPlayer, rate);
}
