/*

开发环境  ： win10/win11 vs2019-Qt5.15.2
VS Build :  x64


作者：令狐掌门
博客链接：https://blog.csdn.net/yao_hou/category_9275800.html
QQ群：894097803

*/


#include "QQMediaPlayerCopy.h"
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

    QQMediaPlayerCopy w;
    w.show();
    return a.exec();
}
