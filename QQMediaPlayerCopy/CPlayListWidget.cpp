#include "CPlayListWidget.h"
#include <QVBoxLayout>

CPlayListWidget::CPlayListWidget(QWidget* parent) : QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	this->setFixedWidth(400);

	QVBoxLayout* pVLay = new QVBoxLayout(this);
	
	m_pListView = new QListView(this);
	m_pListView->setFixedWidth(390);
	
	//设置不可编辑
	m_pListView->setEditTriggers(QListView::EditTrigger::NoEditTriggers);

	QString qss = "QListView{background-color:rgb(64,74,84);show-decoration-selected:1;border:none;color:#FFFFFF;outline:none} \
		QListView::item{padding-top:12px;padding-bottom:12px;} \
		QListView::item:alternate{background:#EEEEEE;} \
		QListView::item:selected{border:1px solid #6a6ea9;} \
		QListView::item::selected:!active{background:qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #6a6ea9, stop:1 #888dd9);} \
		QListView::item::hover{background:qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #000000, stop:1 #FFFFFF);}";

	m_pListView->setStyleSheet(qss);

	pVLay->addWidget(m_pListView);
	pVLay->setContentsMargins(0, 0, 0, 0);

	setLayout(pVLay);

	connect(m_pListView, &QListView::doubleClicked, this, &CPlayListWidget::on_Item_doubleClicked);
}

CPlayListWidget::~CPlayListWidget()
{
}

void CPlayListWidget::setPlayList(QStringListModel* listModel)
{
	m_pStringListModel = listModel;
	m_pListView->setModel(listModel);
}

QStringList CPlayListWidget::getPlaylist() const
{
	return m_pStringListModel->stringList();  //获取数据模型的StringList
}

void CPlayListWidget::on_Item_doubleClicked(const QModelIndex& index)
{
	QString itemValue = m_pStringListModel->data(index).toString();
	emit sig_doubleClickFileName(itemValue);
}
