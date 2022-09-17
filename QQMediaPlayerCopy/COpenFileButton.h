#pragma once

#include <QWidget>
#include <QPushButton>

class COpenFileButton : public QWidget
{
	Q_OBJECT

public:
	COpenFileButton(QWidget* p = nullptr);
	~COpenFileButton();

private slots:
	void on_openfile();
	void on_openFloder();
	void on_openNetStream();

protected:
	void paintEvent(QPaintEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;

signals:
	void sig_openfile(const QStringList& fileList);
	void sig_openUrl(const QString& url);

private:
	QPushButton* m_pOpenFileButton = nullptr;
	QPushButton* m_pArrowButton = nullptr;
	int m_totalWidth = 0;
};

