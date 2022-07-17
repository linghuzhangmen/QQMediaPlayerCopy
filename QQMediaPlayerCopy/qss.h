#pragma once

#include <string>

std::string logo_button_qss = R"(
	QPushButton{
		background-color:rgb(54,54,54);
		border:none;
		font-family:Microsoft YaHei;
		font-size:18px; 
		color:rgb(255,255,255);
	}

	QPushButton::menu-indicator:open{
		image:url(:/titleBar/resources/titleBar/down_arrow.svg);
		subcontrol-position:right center;
		subcontrol-origin:padding;border:none;
	}

	QPushButton::menu-indicator:closed{
		image:url(:/titleBar/resources/titleBar/up_arrow.svg);
		subcontrol-position:right center;
		subcontrol-origin:padding;border:none;
	})";

std::string menu_qss = R"(
	QMenu{background-color:rgb(253,253,253);}
	QMenu::item{
		font:16px;
		background-color:rgb(253,253,253);
		padding:8px 32px;
		margin:0px 8px;
		border-bottom:1px solid #DBDBDB;}
		/*—°‘ÒœÓ…Ë÷√*/
		QMenu::item:selected{background-color: #FFF8DC;})";

std::string minimode_qss = R"(
	QPushButton#m_pMinimodeBtn{
		background-image:url(:/titleBar/resources/titleBar/minimode.svg);
		border:none
	}

	QPushButton#m_pMinimodeBtn:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titleBar/resources/titleBar/minimode_hover.svg);
		border:none;
	}
)";

std::string settop_qss = R"(
	QPushButton#m_pSettopBtn{
		background-image:url(:/titleBar/resources/titleBar/settop.svg);
		border:none
	}

	QPushButton#m_pSettopBtn:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titleBar/resources/titleBar/settop_hover.svg);
		border:none
	}
)";

std::string settoped_qss = R"(
	QPushButton#m_pSettopBtn{
		background-image:url(:/titleBar/resources/titleBar/settoped);
		border:none
	}

	QPushButton#m_pSettopBtn:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titleBar/resources/titleBar/settoped);
		border:none
	}
)";

std::string min_qss = R"(
	QPushButton{
		background-image:url(:/titleBar/resources/titleBar/min.svg);
		border:none
	}

	QPushButton:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titleBar/resources/titleBar/min_hover.svg);
		border:none;
	}

	QPushButton:pressed
	{
		background-image:url(:/titleBar/resources/titleBar/min.svg);
		border:none;
	}
	)";

std::string max_qss = R"(
	QPushButton#m_pMaxBtn{
		background-image:url(:/titleBar/resources/titleBar/normal.svg);
		border:none}

	QPushButton#m_pMaxBtn:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titleBar/resources/titleBar/normal_hover.svg);
		border:none;
	}

	QPushButton#m_pMaxBtn:pressed
	{
		background-image:url(:/titleBar/resources/titleBar/min.svg);
		border:none;
	}
	)";

std::string close_qss = R"(
	QPushButton{
		background-image:url(:/titleBar/resources/titleBar/close.svg);
		border:none
	}

	QPushButton:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titleBar/resources/titleBar/close_hover.svg);
		border:none;
	}

	QPushButton:pressed
	{
		background-image:url(:/titleBar/resources/titleBar/close.svg);
		border:none;
	}
	)";
