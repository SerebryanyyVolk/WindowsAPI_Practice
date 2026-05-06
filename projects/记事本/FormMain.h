#pragma once

/*	目前只有 FormMain.cpp 引用该头文件
	主要是因为如果不把结构体声明放在头文件中，
	VisualAssist 不能正确显示注释信息，开发很不方便
*/

#include "BForm.h"

//窗口状态结构体
struct tMainStatus{
	//是否显示状态栏
	bool showStatusBar;

	//是否窗口置顶
	bool topMost;

	//以何种编码格式显示打开
	// true 为 UTF-8
	// false 为 ANSI
	bool asUTF8;

	//是否保存
	bool isSaved;

	//上一次操作中用户是否取消
	//目前只有打开和另存为操作使用该全局变量
	bool cancel;

	//是否打开的是只读文档（isOpen == false 时无效）
	//（与 isOpenReadOnly 区别是， isReadOnly 保存文档自身属性， isOpenReadOnly 保存用户选择的打开模式）
	bool isReadOnly;

	//是否以只读模式打开（isOpen == false 时无效）
	//（与 isReadOnly 区别是， isReadOnly 保存文档自身属性， isOpenReadOnly 保存用户选择的打开模式）
	bool isOpenReadOnly;
};

//主窗体全局结构体，包含所有主窗口所需全局变量
struct TMainContext{
	//主窗体状态结构体，保存所有标志位
	tMainStatus status;

	//程序 .exe 路径（包含文件名称）
	tstring execPath;

	//程序指向的文档的路径（包含文件名称）
	tstring szFilePath;

	//程序当前前显示的 主文本框控件ID
	unsigned short txtIdShow;

	//程序当前前隐藏的 主文本框控件ID
	unsigned short txtIdHide;

	////程序指向的文档所属文件夹内的路径
	//tstring szFolderPath;	
};
