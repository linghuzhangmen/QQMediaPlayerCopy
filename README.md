# 更新说明
2023-11-1
<br/>
(1) 修改底部控制栏倍速按钮样式
<br/>
(2) 增加拖拽播放功能
<br/>
<br/>

2023-03-19
<br/>
(1) 修改底部控制栏的实现
<br/>
<br/>
2022-09-17
<br/>
(1) 增加网络流播放功能
<br/>
<br/>
2022-07-23
<br/>
(1) 修改界面ui，增加翻译文件，解决Qt中文乱码问题
<br/>
(2) 增加倍速播放功能
<br/>
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

使用C++ Qt开发的Windows端视频播放器，界面高仿QQ影音, 如下图

![image](https://github.com/linghuzhangmen/QQMediaPlayerCopy/assets/42860254/18cc071c-0948-44ec-a9a3-ee7852bd8479)



<br/>
主要功能
<ol>
  <li>支持mp4、avi、flv等视频文件的播放</li>
  <li>支持网络流播放，例如http, rtsp, rtmp等网络流</li>
  <li>支持选择多个文件循环播放</li>
  <li>支持打开目录进行播放</li>
  <li>双击全屏播放</li>
  <li>支持倍速播放，可进行0.5~~2倍速播放</li>
</ol>
<br/>

# 分支说明
main分支：使用libvlc为播放组件
<br/>
player_SDL分支：使用SDL FFmpeg做解码转换播放

# 开发环境 
main分支：win11 x64、vs2022  qt6.3
<br/>
player_SDL分支：win11/win10  vs2019 Qt5.15.2 x64
<br/>
VS2019 Qt开发环境的配置可以参考这篇博客：https://mingshiqiang.blog.csdn.net/article/details/108015209

# 编译方法
本项目只能进行x64编译，不支持x86编译
<br/>
1 vs2019/vs2022打开QQMediaPlayerCopy.sln 右键项目属性---> Qt ---> 如下图
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


