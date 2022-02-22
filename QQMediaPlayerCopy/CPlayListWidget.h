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

private slots:
	void on_Item_doubleClicked(const QModelIndex& index);

signals:
	void sig_fileName(const QString& fileName);

private:
	QListView* m_pListView = nullptr;
	QStringListModel* m_pStringListModel = nullptr;
};


