/*

图片资源说明

标题栏 只用一张图，大小16*16，#fff

*/

#pragma once

#include <QString>



QString logo_button_qss = R"(
	QPushButton{
		background-color:rgb(54,54,54);
		text-align:left;
		font-family:Microsoft YaHei;
		font-size:14px; 
		color:rgb(255,255,255);
		border:none;
	}

	QPushButton::menu-indicator:open{
		image:url(:/titleBar/resources/titleBar/down_arrow.svg);
		subcontrol-position:right center;
		subcontrol-origin:padding;
		border:none
	}

	QPushButton::menu-indicator:closed{
		image:url(:/titleBar/resources/titleBar/up_arrow.svg);
		subcontrol-position:right center;
		subcontrol-origin:padding;
		border:none
	})";

QString menu_qss = R"(
		QMenu
		{
			background-color:rgb(73, 73, 73);
		}
 
		QMenu::item
		{
			 font:16px;
			 color:white;
			 background-color:rgb(73, 73, 73);
			 margin:8px 8px;
		}
 
		/*选择项设置*/
		QMenu::item:selected
		{
			 background-color:rgb(54, 54, 54);
		}
	)";

QString minimode_qss = R"(
	QPushButton#m_pMinimodeBtn{
		background-image:url(:/titleBar/resources/titleBar/minimode.png);
		background-position:center; 
		background-repeat: no-repeat;
		border:none
	}

	QPushButton#m_pMinimodeBtn:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titleBar/resources/titleBar/minimode.png);
		border:none;
	}
)";

QString settop_qss = R"(
	QPushButton#m_pSettopBtn{
		background-image:url(:/titleBar/resources/titleBar/settop.png);
		background-position:center; 
		background-repeat: no-repeat;
		border:none
	}

	QPushButton#m_pSettopBtn:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titleBar/resources/titleBar/settop.png);
		border:none
	}
)";

QString settoped_qss = R"(
	QPushButton#m_pSettopBtn{
		background-image:url(:/titleBar/resources/titleBar/settop_cancel.png);
		background-position:center; 
		background-repeat: no-repeat;
		border:none
	}

	QPushButton#m_pSettopBtn:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titleBar/resources/titleBar/settop_cancel.png);
		border:none
	}
)";

QString min_qss = R"(
	QPushButton{
		background-image:url(:/titleBar/resources/titleBar/min.png);
		background-position:center; 
		background-repeat: no-repeat;
		border:none
	}

	QPushButton:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titleBar/resources/titleBar/min.png);
		border:none;
	}

	QPushButton:pressed
	{
		background-image:url(:/titleBar/resources/titleBar/min.png);
		border:none;
	}
	)";

QString max_normal_qss = R"(
	QPushButton#m_pMaxBtn{
		background-image:url(:/titleBar/resources/titleBar/max_normal.png);
		background-position:center; 
		background-repeat: no-repeat;
		border:none}

	QPushButton#m_pMaxBtn:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titleBar/resources/titleBar/max_normal.png);
		border:none;
	}

	QPushButton#m_pMaxBtn:pressed
	{
		background-image:url(:/titleBar/resources/titleBar/max_normal.png);
		border:none;
	}
	)";

QString maxed_qss = R"(
	QPushButton#m_pMaxBtn{
		background-image:url(:/titleBar/resources/titleBar/max.png);
		background-position:center; 
		background-repeat: no-repeat;
		border:none}

	QPushButton#m_pMaxBtn:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titleBar/resources/titleBar/max.png);
		border:none;
	}

	QPushButton#m_pMaxBtn:pressed
	{
		background-image:url(:/titleBar/resources/titleBar/max.png);
		border:none;
	}
	)";


QString close_qss = R"(
	QPushButton#m_pCloseBtn{
		background-image:url(:/titleBar/resources/titleBar/close.png);
		background-position:center; 
		background-repeat: no-repeat;
		border:none
	}

	QPushButton#m_pCloseBtn:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titleBar/resources/titleBar/close.png);
		border:none;
	}

	QPushButton#m_pCloseBtn:pressed
	{
		background-image:url(:/titleBar/resources/titleBar/close.png);
		border:none;
	}
	)";
