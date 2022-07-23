# 更新说明
2022-07-23
<br/>
(1) 修改界面ui，增加翻译文件，解决Qt中文乱码问题
<br/>
(2) 增加倍速播放功能
<br/>
2022-07-17
<br/>
(1) main分支使用libvlc作为播放组件，player_SDL任然使用ffmpeg、SDL解码播放
<br/>
(2) 修改增加窗口置顶、修改界面样式
<br/>
<br/>
2022-07-14
<br/>
项目开发环境更新到vs2022 qt6.3, 不影响vs2019 qt5.15.2以下版本的编译

# QQMediaPlayerCopy项目介绍

使用C++ Qt开发的Windows端视频播放器，高仿QQ影音界面, 如下

![image](https://user-images.githubusercontent.com/42860254/180600270-91697fc3-36f9-4830-bbd7-9cd33f7640e1.png)


播放界面
![3](https://user-images.githubusercontent.com/42860254/155159626-0ccfb947-336b-4ea3-9716-60ce1b7b9111.png)
<br/>
主要功能
+ 支持mp4、avi、flv等视频文件的播放
+ 支持选择多个文件循环播放
+ 支持打开目录进行播放
+ 双击全屏播放
+ 支持倍速播放，0.5~~2倍速播放
<br/>
其它待开发功能
+ rtmp流播放
+ 视频进度调节、音量调节
+ 界面部分功能优化
<br/>

# 分支说明
main分支：使用libvlc为播放组件
<br/>
player_SDL分支：使用SDL FFmpeg做解码转换播放

# 开发环境 
main分支：win11 x64、vs2022  qt6.3
<br/>
<br/>
player_SDL分支：win11/win10  vs2019 Qt5.15.2 x64


# 编译方法
本项目只能进行x64编译，不支持x86编译
<br/>
1 vs2019打开QQMediaPlayerCopy.sln 右键项目属性---> Qt ---> 如下图
![qqq](https://user-images.githubusercontent.com/42860254/159120393-2b091a49-4058-493d-8dfb-1b086aec4295.png)
<br/>
<br/>
2 下载依赖的sdk
<br/>
链接：https://pan.baidu.com/s/10uXf3976ihfZyyLrrTtGOA 
<br/>
提取码：hmhn
<br/>
下载后，解压，将sdk_package放到QQMediaPlayerCopy.vcxproj同级目录


# 开发环境安装
VS2019 Qt开发环境的配置可以参考这篇博客：https://mingshiqiang.blog.csdn.net/article/details/108015209

