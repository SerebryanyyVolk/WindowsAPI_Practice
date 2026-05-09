#pragma once

#include "BForm.h"

//主窗体状态结构体
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

	//程序指向的文档所属文件夹内的路径
	//tstring szfolderpath;

	//加密字符串，为 NULL 时不加密
	const char * encKey;
};



//主窗体注册事件表函数
extern void FormMain_EventsMapInit(void);

//主窗体启动函数
extern void FormMain_Start(void);

//秘钥输入窗体注册事件表函数
extern void FormKey_EventsMapInit(void);

/*	密码窗体启动函数
	返回是否获得了密码
	true 为获得了密码
	若成功获得密码，指针会传到 cFMain.encKey 中
*/
extern bool InputEncrypt(void);

//主窗体控制结构体
extern TMainContext cFMain;

//主窗体对象
extern CBForm formMain;

//密码窗体对象
extern CBForm formKey;


