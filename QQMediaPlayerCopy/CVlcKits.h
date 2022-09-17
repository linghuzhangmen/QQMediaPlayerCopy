/*

libvlc qt¹¦ÄÜ·â×°

*/

#pragma once

#ifdef _WIN32
#include <basetsd.h>
typedef SSIZE_T ssize_t;
#endif

#include "vlc/vlc.h"
#include <QObject>
#include <vector>

using namespace std;

class CVlcKits : public QObject
{
    Q_OBJECT

public:
	CVlcKits();
	~CVlcKits();

    int initVLC();

    libvlc_media_player_t* media_player();

    void setTimeSliderPos(int value);
    void setVolumeSliderPos(int value);
    void setTimeText(const QString& str);
    libvlc_time_t getDuration();
    void setCurrentDuration(libvlc_time_t value);

    int play(QStringList fileList, void* hwnd);
    int play(const QString& url, void* hwnd);
    void play();
    void pause();
    void stop();

    vector<libvlc_time_t> getVecDuration();
    int getCurrentIndex();
    void addCurrentIndex();

    void setVideoPostion(int value);

    void setPlayRate(double rate);

signals:
    void sig_TimeSliderPos(const int& value);
    void sig_VolumeSliderPos(const int& value);
    void sig_UpdateTimeText(const QString& str);

private:
    libvlc_instance_t* m_pInstance = nullptr;
    libvlc_media_player_t* m_pMediaPlayer = nullptr;
    libvlc_media_t* m_pMedia = nullptr;
    libvlc_event_manager_t* m_pEvent_manager = nullptr;

    libvlc_media_list_player_t* m_pMediaPlayerList;
    libvlc_media_list_t* m_medialist;

    libvlc_time_t m_CurrentDuration = 0;
    vector<libvlc_time_t> m_vecDurations;

    int m_CurrentInex = 0;
};

