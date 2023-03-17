/*

Qt libvlc 视频播放器器项目

开发环境  ： win10/win11 vs2022 Qt6.4 x64
VS Build :  x64


作者：令狐掌门
博客链接：https://blog.csdn.net/yao_hou/category_9275800.html

*/


#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <string>
#include <QTranslator>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/resources/logo.png"));

    //Translation_zh_Hans.qm
    QTranslator* trans = new QTranslator();

    //注意路径的反斜线，翻译文件加载错误将不会翻译
    QString qm_path = a.applicationDirPath() + "/translations/Translation_zh_Hans.qm";
    trans->load(qm_path);
    a.installTranslator(trans);

    CMainWindow w;
    w.show();
    return a.exec();
}
