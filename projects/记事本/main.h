#pragma once

#include "resource.h"
#include "BForm.h"
#include "mdlOpenSaveDlg.h"

struct TStatus{	//窗口状态结构体
	bool showStatus;	//是否显示状态栏
	bool topMost;		//是否窗口置顶
};

struct TMain{
	TStatus status;
	TCHAR szFileName[1024];
};

void FormMain_Load(void);
void FormMain_Resize(void);
void FormMain_MenuClick(int menuID, int bIsFromAcce, int bIsFromSysMenu);
