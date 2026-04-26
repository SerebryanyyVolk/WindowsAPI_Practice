#pragma once

#include "resource.h"
#include "BForm.h"
#include "mdlOpenSaveDlg.h"

//窗口状态结构体
struct tMainStatus{
	//是否显示状态栏
	bool showStatusBar;

	//是否窗口置顶
	bool topMost;		

	//是否打开了文档（实际上是 是否保存了一个实际上存在的文档的路径）
	bool isOpen;

	//是否打开的是只读文档（isOpen == false 时无效）
	//（与 isOpenReadOnly 区别是， isReadOnly 保存文档自身属性， isOpenReadOnly 保存用户选择的打开模式）
	bool isReadOnly;

	//是否以只读模式打开（isOpen == false 时无效）
	//（与 isReadOnly 区别是， isReadOnly 保存文档自身属性， isOpenReadOnly 保存用户选择的打开模式）
	bool isOpenReadOnly;
};

//主窗体全局结构体，包含所有主窗口所需全局变量
struct tMainContext{
	//主窗体状态结构体，保存所有标志位
	tMainStatus status;

	//程序 .exe 路径（包含文件名称）
	tstring execPath;

	//程序指向的文档的路径（包含文件名称）
	tstring szFilePath;

	////程序指向的文档所属文件夹内的路径
	//tstring szFolderPath;	

};

//主窗口加载处理函数
void FormMain_Load(void);

//主窗口控件摆放函数，在窗口被拉伸时调用
void FormMain_Resize(void);

//主窗口菜单点击处理函数
void FormMain_MenuClick(int menuID, int bIsFromAcce, int bIsFromSysMenu);

//打开位置在 szFileName 的文件，并在主窗口中展示文本
// mode 决定文件打开方式 默认为 EF_OpStyle_Input (仅读取)
//打开成功返回 true ，否则返回 false 
//注：本函数与全局文件路径 appContext.szFilePath 无关
//目前还没实现
bool LoadFile(LPCTSTR szFilePath, eEFOpenStyle mode = EF_OpStyle_Input);

// 保存文件到参数 szFileName 所指定的路径+文件名中
// 成功返回 true 并同时设置 m_szFileName[]；失败返回 false
//目前还没实现
bool SaveFile(LPCTSTR szFileName);

