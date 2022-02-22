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

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/resources/logo.png"));
    //qRegisterMetaType<std::string>("std::string");

    QQMediaPlayerCopy w;
    w.show();
    return a.exec();
}
