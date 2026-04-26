#include "main.h"

CBForm formMain(ID_formMain);

//主窗口全局结构体变量，包含所有主窗口所需全局变量
tMainContext appContext;

int main(void){
	formMain.EventAdd(0, eForm_Load, FormMain_Load);
	formMain.EventAdd(0, eForm_Resize, FormMain_Resize);
	formMain.EventAdd(0, eMenu_Click, FormMain_MenuClick);
	
	formMain.Show();

	return 0;
}

void FormMain_StatusInit(void){
	appContext.status.showStatusBar = true;	//开始运行时默认显示状态栏
	appContext.status.topMost = false;		//开始运行时默认窗口不置顶
	
	//获取.exe路径
	appContext.execPath = pApp->Command();	//先获得左右两边带 \" 字符的路径
	appContext.execPath.erase(0, 1);		//去掉字符串最前面的 \"
	int iPos = 0, last = 0;
	while(iPos != tstring::npos){		//找到最后面的 \" 的位置(因为用户的路径中可能有其他\"字符)
		last = iPos;
		iPos = appContext.execPath.find(TEXT("\""), iPos + 1);
	}
	if(last != 0){						//去掉最后面的 \" 字符，获得 .exe 路径
		appContext.execPath.erase(last);
	}
}

void FormMain_Load(void){
	FormMain_StatusInit();

	formMain.IconSet(IDI_ICON6);	//设置图标
	FormMain_Resize();				//摆放控件

	formMain.Control(ID_txtMain).FontSizeSet(14);	//设置文本框字体大小为较大
	formMain.Control(ID_lblStatus).TextSet(TEXT("欢迎使用！"));	//设置状态栏初始文字
}

void FormMain_Resize(void){
	int cyStatus = 0;
	if(appContext.status.showStatusBar){
		cyStatus = formMain.Control(ID_lblStatus).Height() + 6; //包含空白间隔的总高度
		formMain.Control(ID_lblStatus).Move(
			4, formMain.ClientHeight() - cyStatus + 4,	//x y 坐标，减去空白间隔
			formMain.ClientWidth() - 8,				//状态栏宽度
			cyStatus - 6							//状态栏高度
			);
	}
	formMain.Control(ID_txtMain).Move(
		0, 0,								//Edit 控件 x y 坐标为0
		formMain.ClientWidth(),				//Edit 控件宽度
		formMain.ClientHeight() - cyStatus		//Edit 控件高度
		);
}

void FormMain_MenuClick(int menuID, int bIsFromAcce, int bIsFromSysMenu)
{
	INT_PTR	ret = 0;
	bool temp = false;

	switch(menuID){
	case ID_mnuFileNew:
		//直接在控制台运行 "记事本.exe"
		//程序未运行时 .exe 名称被用户修改不会影响该功能
		//程序运行时用户若修改 .exe 名称则会弹窗报错，错误代码为 2
		//但是若用户关闭窗口重新运行 .exe 则不会再次报错
		ret = (INT_PTR)ShellExecute(NULL, TEXT("open"), 
									appContext.execPath.c_str(),
									NULL, NULL, SW_SHOWNORMAL);
		if(ret <= 32){
			MsgBox(StrAppend(TEXT("新建时发生错误！\r\n\r\n错误代码："), Str(ret)),		//若ShellExecute返回值小于等于32
				TEXT("严重错误"), mb_OK, mb_IconError);									//则说明 exe 运行失败，弹窗报错	
		}
		break;

	case ID_mnuFileOpen:
		OsdSetFilter(TEXT("文本文档(*.txt)|*.txt"), true);			//设置 "打开文档" 窗口可以筛选的文件类型

		//弹出"打开文档"窗口，模式为单选模式，获取文件路径，并通过 temp 获取打开模式（是否为以只读模式打开）
		appContext.szFilePath = OsdOpenDlg((HWND)0, (LPCTSTR)NULL, (LPCTSTR)NULL, false, &temp);
		if(!appContext.szFilePath.empty()){	//若 hform.szFilePath.empty() 返回 false ，说明用户选择了文件，继续处理
			if(false == LoadFile(appContext.szFilePath.c_str())){

			}
			else{
				appContext.status.isOpen = true;	//设置 isOpen 标志位为 true
				appContext.status.isOpenReadOnly = temp;	//设置 isOpenReadOnly 标志位等于 OsdOpenDlg 函数返回值 temp
			}
		}
		break;

	case ID_mnuFileExit:	//点击退出键
		formMain.UnLoad();	//退出
		break;

	case ID_mnuEditSelAll:	//全选
		formMain.Control(ID_txtMain).SelSet();//不填参数默认全选
		break;

	case ID_mnuEditDate:	//日期和时间
		formMain.Control(ID_txtMain).SelTextSet(Now());//在光标处插入日期和时间，或者把选中的文本替换为日期和时间
		break;

	case ID_mnuViewState:	//点击状态栏键
		appContext.status.showStatusBar = !appContext.status.showStatusBar;	//切换状态栏标志位状态
		formMain.Control(ID_lblStatus).VisibleSet(appContext.status.showStatusBar);	//改变状态栏透明状态
		formMain.Menu(ID_mnuViewState).CheckedSet(appContext.status.showStatusBar);	//改变菜单中状态栏项前面有无对勾的状态
		FormMain_Resize();		//因为状态栏控件出现或隐藏会改变控件布局，故需要重新排布控件位置
		break;

	case ID_mnuViewTopMost:		//点击窗口置顶选项
		appContext.status.topMost = !appContext.status.topMost;		//切换窗口置顶标志位状态
		//切换窗口置顶状态
		if(appContext.status.topMost){		
			SetWindowPos(formMain.hWnd(), HWND_TOPMOST, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE
				);
		}
		else{
			SetWindowPos(formMain.hWnd(), HWND_NOTOPMOST, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE
				);
		}
		//改变菜单中窗口置顶项前面有无对勾的状态
		formMain.Menu(ID_mnuViewTopMost).CheckedSet(appContext.status.topMost);
		break;

	case ID_mnuHelpAbout:	//点击关于键，显示版本等信息
		MsgBox(TEXT("记事本    版本:1.0\r\n作者:不玩神碑\r\n\r\n版权所有(C)    2026"),
			TEXT("关于 记事本"), mb_OK, mb_IconInformation);
		break;

	default:
		break;

	}
}

//打开位置在 szFileName 的文件，并在主窗口中展示文本
// mode 决定文件打开方式 默认为 EF_OpStyle_Input (仅读取)
//打开成功返回 true ，否则返回 false 
//注：本函数与全局文件路径 appContext.szFilePath 无关
bool LoadFile(LPCTSTR szFilePath, eEFOpenStyle mode/* EF_OpStyle_Input */){
	LONGLONG fileLen = 0L;//文件长度
	LONG readLen = 0L;//实际读取长度
	LPCTSTR szFailInfo = NULL;

	// 清除状态栏提示
	formMain.Control(ID_lblStatus).TextSet(TEXT(""));

	HANDLE hFile = EFOpen(szFilePath, mode);   // 打开文件
	//若打开文件失败，返回 false
	if((int)hFile == -1){
		formMain.Control(ID_lblStatus).TextSet(TEXT("文件打开失败！"));
		return false;
	}

	//需要使用流式读取，目前我写不出来，先注释掉

	////获取文件长度（字节数）
	//fileLen = EFLOF(hFile);

	////开辟读取数据缓冲区
	//char * pBuff = new char[4];

	////按字节读取文件
	//readLen = EFGetBytes(hFile, 0L, pBuff, 4, 2, szFailInfo);
	//if(readLen < 0){// readLen<0 说明读取出错 
	//	if(-2 == readLen){//用户点击"忽略"
	//		formMain.Control(ID_lblStatus).TextSet(TEXT("文件打开失败！"));
	//		return false;
	//	}
	//	else{//用户直接关闭报错弹窗或点击"终止"或"取消"
	//		formMain.Control(ID_lblStatus).TextSet(TEXT("文件打开失败！"));
	//		MsgBox(StrAppend(TEXT("文件打开失败！\r\n\r\n"), szFailInfo), TEXT("错误"), mb_OK, mb_IconError);
	//		return false;
	//	}
	//}



	// 在窗体标题栏中显示：文件名 - 记事本
	formMain.TextSet(szFilePath);
	formMain.TextAdd(TEXT(" - 记事本"));

	return true;
}

// 保存文件到参数 szFileName 所指定的路径+文件名中
// 成功返回 true 并同时设置 m_szFileName[]；失败返回 false
bool SaveFile(LPCTSTR szFileName)
{
	// 清除状态栏提示
	formMain.Control(ID_lblStatus).TextSet(TEXT(""));

	HANDLE hFile = EFOpen(szFileName, EF_OpStyle_Output);   // 打开文件
	// 直接向文件输出字符串内容为控件 ID_txtMain 的内容
	// EF_LineSeed_None 表示输出后不要再多输出个换行符
	EFPrint(hFile, formMain.Control(ID_txtMain).Text(), EF_LineSeed_None );
	EFClose(hFile);                                          // 关闭文件

	// 将文件名保存到全局 m_szFileName
	appContext.szFilePath = szFileName;

	// 在窗体标题栏中显示：文件名 - 记事本
	formMain.TextSet(appContext.szFilePath);
	formMain.TextAdd(TEXT(" - 记事本"));
	// 在状态栏给出提示
	formMain.Control(ID_lblStatus).TextSet(StrAppend(TEXT("文件保存成功，已保存到:\""),
													appContext.szFilePath.c_str(), TEXT("\"!")));

	// 返回成功
	return true;
}