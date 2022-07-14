# 更新说明
2022-07-14
项目开发环境更新到vs2022 qt6.3, 不影响vs2019 qt5.15.2以下版本的编译

# QQMediaPlayerCopy项目介绍

C++ Qt 高仿Windows端QQ影音视频播放器, 界面如下

![image](https://user-images.githubusercontent.com/42860254/155153215-aafac438-ad2f-4fcd-95c4-5e303aa9ac15.png)

播放界面
![3](https://user-images.githubusercontent.com/42860254/155159626-0ccfb947-336b-4ea3-9716-60ce1b7b9111.png)

# 开发环境 
win11/win10  vs2019 Qt5.15.2 
build  x64
<br/>
# 分支说明
<br/>
player_SDL分支使用SDL FFmpeg做解码转换播放

# 编译方法
1 vs2019打开QQMediaPlayerCopy.sln 右键项目属性---> Qt ---> 如下图
![qqq](https://user-images.githubusercontent.com/42860254/159120393-2b091a49-4058-493d-8dfb-1b086aec4295.png)
<br/>
2 下载依赖的sdk
<br/>
链接：https://pan.baidu.com/s/10uXf3976ihfZyyLrrTtGOA 
提取码：hmhn
下载后，解压，将sdk_package放到QQMediaPlayerCopy.vcxproj同级目录


# 开发环境安装
VS2019 Qt开发环境的配置可以参考这篇博客：https://mingshiqiang.blog.csdn.net/article/details/108015209

