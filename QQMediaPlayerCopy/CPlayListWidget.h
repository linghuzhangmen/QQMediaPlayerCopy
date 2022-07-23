#pragma once

#include <QWidget>
#include <QListView>
#include <QStringListModel>

class CPlayListWidget : public QWidget
{
	Q_OBJECT

public:
	CPlayListWidget(QWidget* parent = Q_NULLPTR);
	~CPlayListWidget();

	void setPlayList(QStringListModel* listModel);
	QStringList getPlaylist() const;
	void setPlay(bool play) { m_isPlay = play; }
	bool getPlay() const { return m_isPlay; }

private slots:
	void on_Item_doubleClicked(const QModelIndex& index);

signals:
	void sig_doubleClickFileName(const QString& fileName);

private:
	QListView* m_pListView = nullptr;
	QStringListModel* m_pStringListModel = nullptr;
	bool m_isPlay = false;
};


