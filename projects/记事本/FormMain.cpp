#include "main.h"

//主窗口加载处理函数
static void FormMain_Load(void);

//主窗口控件摆放函数，在窗口被拉伸时调用
static void FormMain_Resize(void);

//主窗口主文本框字符变化处理函数
static void FormMain_TxtChange(void);

//主窗口菜单点击处理函数
static void FormMain_MenuClick(int menuID, int bIsFromAcce, int bIsFromSysMenu);

//主窗口关闭前处理函数
static void FormMain_QueryUnload(int pbCancel);



//设置主文本框字体大小
static void TxtMainFontSizeSet(float fSize);

//设置主文本框字体颜色
static void TxtMainFontColorSet(COLORREF color);

//设置主文本框字体颜色
static void TxtMainFontColorSet(EColorType color);

//设置上面主菜单栏全部选项的可用状态
static void MenuAllEnableSet(bool bVal);

//初始化主窗体全局变量结构体
static void FormMain_StatusInit(void);

//打开位置在 szFileName 的文件，并在主窗口中展示文本
//打开成功返回 true ，否则返回 false 
//注：本函数与全局文件路径 appContext.szFilePath 无关，需自行传入路径
static bool LoadFile(LPCTSTR szFilePatht);

// 保存文件到参数 szFileName 所指定的路径+文件名中
// 成功返回 true 并同时设置 m_szFileName[]；失败返回 false
//目前还没实现
static bool SaveFile(LPCTSTR szFileName);



//主窗体对象
CBForm formMain(ID_formMain);

void FormMain_EventMapInit(void){
	//注册各种事件
	formMain.EventAdd(0, eForm_Load, FormMain_Load);
	formMain.EventAdd(0, eForm_Resize, FormMain_Resize);
	formMain.EventAdd(0, eMenu_Click, FormMain_MenuClick);
	formMain.EventAdd(0, eForm_QueryUnload, FormMain_QueryUnload);

	formMain.EventAdd(ID_txtMainWarp, eEdit_Change, FormMain_TxtChange);
	formMain.EventAdd(ID_txtMainNWarp, eEdit_Change, FormMain_TxtChange);
}

//加载主窗体
void FormMain_Load(void){
	FormMain_StatusInit();

	formMain.IconSet(IDI_ICON6);	//设置图标
	FormMain_Resize();				//摆放控件

	TxtMainFontSizeSet(14);		//设置文本框字体大小为较大
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
	formMain.Control(appContext.txtIdShow).Move(
		0, 0,								//Edit 控件 x y 坐标为0
		formMain.ClientWidth(),				//Edit 控件宽度
		formMain.ClientHeight() - cyStatus		//Edit 控件高度
		);
}

void FormMain_TxtChange(void){
	//若修改了文本则将保存状态设为 false
	appContext.status.isSaved = false;
}

void FormMain_MenuClick(int menuID, int bIsFromAcce, int bIsFromSysMenu)
{
	tstring sTemp;
	INT_PTR	ret = 0;
	bool bTemp = false;
	int iTemp = 0;

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
		appContext.szFilePath = OsdOpenDlg((HWND)0, (LPCTSTR)NULL, (LPCTSTR)NULL, false, &bTemp);
		if(!appContext.szFilePath.empty()){	//若 hform.szFilePath.empty() 返回 false ，说明用户选择了文件，继续处理
			appContext.status.cancel = false;
			if(true == LoadFile(appContext.szFilePath.c_str())){
				appContext.status.isOpenReadOnly = bTemp;	//设置 isOpenReadOnly 标志位等于 OsdOpenDlg 函数返回值 temp
			}
			else{
				//文件打开失败，清空全局变量
				appContext.szFilePath = TEXT("");
			}
		}
		else{
			//用户取消打开
			appContext.status.cancel = true;
		}
		break;

	case ID_mnuFileSave:
		//若当前还未指向任意一文件，则直接调用 另存为
		if(appContext.szFilePath.empty()){
			FormMain_MenuClick(ID_mnuFileSaveAs, bIsFromAcce, bIsFromSysMenu);
			return;
		}
		if(!SaveFile(appContext.szFilePath.c_str())){
			//若保存失败则修改底部状态栏
			formMain.Control(ID_lblStatus).TextSet(TEXT("文件保存失败！"));
		}
		break;

	case ID_mnuFileSaveAs:
		OsdSetFilter(TEXT("文本文档(*.txt)|*.txt"), true);			//设置 "另存为" 窗口可以选择的文件类型
		sTemp = OsdSaveDlg((HWND)0, (LPCTSTR)NULL, (LPCTSTR)NULL);	//获取路径
		if(!sTemp.empty()){						//若路径不为空
			appContext.status.cancel = false;	//"用户取消" 全局变量设为 false
			if(SaveFile(sTemp.c_str())){
				//若保存成功则将路径存到全局变量中
				appContext.szFilePath = sTemp;
			}
			else{
				//若保存失败则修改底部状态栏
				formMain.Control(ID_lblStatus).TextSet(TEXT("文件保存失败！"));
			}
		}
		else{
			//用户取消另存为 "用户取消" 全局变量设为 true
			appContext.status.cancel = true;
		}
		break;

	case ID_mnuFileExit:	//点击退出键
		formMain.UnLoad();	//退出
		break;

	case ID_mnuEditSelAll:	//全选
		formMain.Control(appContext.txtIdShow).SelSet();//不填参数默认全选
		break;

	case ID_mnuEditEncodingUTF8:
		//若之前使用的本来就是UTF-8,则直接跳过

		//若之前用的是ANSI
		if(appContext.status.asUTF8 == false){
			//菜单勾选项改为UTF-8
			formMain.Menu(ID_mnuEditEncodingUTF8).CheckedSet(true);
			formMain.Menu(ID_mnuEditEncodingANSI).CheckedSet(false);
			//全局变量改为使用UTF-8
			appContext.status.asUTF8 = true;

			//若指向文档，则询问用户是否以UTF-8格式重新打开文档
			if(!appContext.szFilePath.empty()){
				if(idYes == MsgBox(TEXT("是否重新打开文档？"), TEXT("重新打开文档"), mb_YesNo, mb_IconQuestion)){
					if(true == LoadFile(appContext.szFilePath.c_str())){
						appContext.status.isOpenReadOnly = bTemp;	//设置 isOpenReadOnly 
																	//标志位等于 OsdOpenDlg 函数返回值 temp
					}
					else{
						//文件打开失败，清空全局变量
						appContext.szFilePath = TEXT("");
					}
				}
			}
		}
		break;
	
	case ID_mnuEditEncodingANSI:
		//若之前用的本来就是ANSI,则直接跳过
		
		//若之前使用的是UTF-8
		if(appContext.status.asUTF8 == true){
			//修改菜单勾选项
			formMain.Menu(ID_mnuEditEncodingUTF8).CheckedSet(false);
			formMain.Menu(ID_mnuEditEncodingANSI).CheckedSet(true);
			//修改全局变量
			appContext.status.asUTF8 = false;

			//询问用户是否将当前打开的文件以 ANSI 格式重新打开
			if(!appContext.szFilePath.empty()){
				if(idYes == MsgBox(TEXT("是否重新打开文档？"), TEXT("重新打开文档"), mb_YesNo, mb_IconQuestion)){
					if(true == LoadFile(appContext.szFilePath.c_str())){
						appContext.status.isOpenReadOnly = bTemp;	//设置 isOpenReadOnly 
																	//标志位等于 OsdOpenDlg 函数返回值 temp
					}
					else{
						//文件打开失败，清空全局变量
						appContext.szFilePath = TEXT("");
					}
				}
			}
		}
		break;

	case ID_mnuEditDate:	//日期和时间
		formMain.Control(appContext.txtIdShow).SelTextSet(Now());//在光标处插入日期和时间，或者把选中的文本替换为日期和时间
		break;

	case ID_mnuViewWarp:
		//交换显示及隐藏文本框的状态变量（实际上就是 id）
		iTemp = appContext.txtIdShow;
		appContext.txtIdShow = appContext.txtIdHide;
		appContext.txtIdHide = (unsigned short)iTemp;
		iTemp = 0;

		//将原本显示的文本框的内容复制到将要显示的文本框中
		if(appContext.status.isSaved == true){
			//因为 TextSet 会修改文本框内容，从而调用 FormMain_TxtChange 函数，导致 isSaved 变量一定会变为 false
			//故通过此处判断保留 isSaved 原本状态
			formMain.Control(appContext.txtIdShow).TextSet(formMain.Control(appContext.txtIdHide).Text());
			appContext.status.isSaved = true;
		}
		else{
			formMain.Control(appContext.txtIdShow).TextSet(formMain.Control(appContext.txtIdHide).Text());
		}
		//切换显示状态
		formMain.Control(appContext.txtIdHide).VisibleSet(false);
		formMain.Control(appContext.txtIdShow).VisibleSet(true);
		//重新排一下控件位置
		FormMain_Resize();
		//切换菜单栏对应项对勾状态
		formMain.Menu(ID_mnuViewWarp).CheckedSet(appContext.txtIdShow == ID_txtMainWarp);
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


void FormMain_QueryUnload(int pbCancel){
	//若文件没保存则先询问是否保存

	//若文件没保存且指向了文件
	//或者没指向文件但是文本框中有内容
	if(((!appContext.status.isSaved) && (!appContext.szFilePath.empty())) ||
		(appContext.szFilePath.empty() && (formMain.Control(appContext.txtIdShow).Text()[0] != 0)))
	{
		if(idYes == MsgBox(TEXT("文件尚未保存，是否保存？"), TEXT("退出"), mb_YesNo, mb_IconQuestion)){
			FormMain_MenuClick(ID_mnuFileSave, 0, 0);
			//若用户在另存为窗口取消，阻止退出
			if((!appContext.status.isSaved) && appContext.status.cancel){
				*(int*) pbCancel = 1;
			}
		}
	}
}

//设置时把展示和隐藏的文本框都更改
void TxtMainFontSizeSet(float fSize){
	formMain.Control(appContext.txtIdShow).FontSizeSet(fSize);
	formMain.Control(appContext.txtIdHide).FontSizeSet(fSize);

}

//设置时把展示和隐藏的文本框都更改
void TxtMainFontColorSet(COLORREF color){
	formMain.Control(appContext.txtIdShow).ForeColorSet(color);
	formMain.Control(appContext.txtIdHide).ForeColorSet(color);
}

//设置时把展示和隐藏的文本框都更改
void TxtMainFontColorSet(EColorType color){
	formMain.Control(appContext.txtIdShow).ForeColorSet(color);
	formMain.Control(appContext.txtIdHide).ForeColorSet(color);
}

void MenuAllEnableSet(bool bVal){
	formMain.Menu(1, 0).EnabledSet(bVal);
	formMain.Menu(2, 0).EnabledSet(bVal);
	formMain.Menu(3, 0).EnabledSet(bVal);
	formMain.Menu(4, 0).EnabledSet(bVal);
	formMain.Menu(5, 0).EnabledSet(bVal);
}

void FormMain_StatusInit(void){
	appContext.status.showStatusBar = true;	//开始运行时默认显示状态栏
	appContext.status.topMost = false;		//开始运行时默认窗口不置顶

	appContext.txtIdShow = ID_txtMainWarp;	//初始默认自动换行
	appContext.txtIdHide = ID_txtMainNWarp;

	appContext.status.isSaved = false;
	appContext.status.cancel = false;

	appContext.status.asUTF8 = true;		//初始默认以utf-8格式打开

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


//打开位置在 szFilePath 的文件，并在主窗口中展示文本
//打开成功返回 true ，否则返回 false 
//注：本函数与全局文件路径 appContext.szFilePath 无关
bool LoadFile(LPCTSTR szFilePath){
	//按行读取文件
	//每读取一行，就显示到文本框空间中，节省内存
	tstring szLine(TEXT(""));	//初始为 ""
	LPTSTR szTemp = NULL;		//初始为 NULL
	CBReadLinesEx file;			//文件读取对象

	file.AsUTF8 = appContext.status.asUTF8;

	//打开文件
	while(!file.OpenFile(szFilePath)){
		//若打不开，则在提示栏显示打开失败
		formMain.Control(ID_lblStatus).TextSet(TEXT("文件打开失败！"));
		//并弹窗询问是否重试
		if(MsgBox(TEXT("文件打开失败！"), TEXT("失败"), mb_RetryCancel, mb_IconError) == idRetry){
			//重试
			continue;
		}
		else{
			//若不重试则返回 false
			return false;
		}
	}

	//清空提示框
	formMain.Control(ID_lblStatus).TextSet(TEXT(""));

	//设置鼠标光标为“等待”样式
	pApp->MousePointerGlobalSet(IDC_Wait);

	//清空文本框
	formMain.Control(appContext.txtIdShow).TextSet(TEXT(""));

	//设置菜单栏不可点击
	MenuAllEnableSet(false);

	//读取文件
	while (!file.IsEndRead()){
		//读取下一行内容
		file.GetNextLine(szTemp);
		
		//若读取文件时发生错误
		if(file.IsErrOccured()){
			//设置鼠标光标为默认样式
			pApp->MousePointerGlobalSet(0);
			//弹窗报错并返回 false
			MsgBox(TEXT("读取文件时发生严重错误！\r\n\r\n若重试，请重新打开文件！"), 
				TEXT("严重错误"), mb_OK, mb_IconError);

			//恢复顶部菜单栏为可用
			MenuAllEnableSet(true);

			return false;
		}

		szLine = szTemp;
		//读取文本本身不含换行符
		//若 file.iEndLineSign > 0 需为读取的文本添加换行符
		if(file.iEndLineSign > 0){			
			szLine += TEXT("\r\n");
		}

		//将内容添加到文本框中
		formMain.Control(appContext.txtIdShow).TextAdd(szLine);
	}

	//恢复鼠标样式为默认
	pApp->MousePointerGlobalSet(0);

	//在提示框中打印提示信息
	if(appContext.status.asUTF8){
		formMain.Control(ID_lblStatus).TextSet(StrAppend(TEXT("已成功打开文件:\""), 
											szFilePath, TEXT("\"    使用编码格式: UTF-8")));
	}
	else{
		formMain.Control(ID_lblStatus).TextSet(StrAppend(TEXT("已成功打开文件:\""), 
			szFilePath, TEXT("\"    使用编码格式: ANSI")));
	}


	// 在窗体标题栏中显示：文件名 - 记事本
	formMain.TextSet(szFilePath);
	formMain.TextAdd(TEXT(" - 记事本"));

	//恢复顶部菜单栏为可用
	MenuAllEnableSet(true);

	return true;
}

// 保存文件到参数 szFileName 所指定的路径+文件名中
// 成功返回 true 并同时设置 appContext.szFilePath；失败返回 false
bool SaveFile(LPCTSTR szFilePath)
{
	//设置鼠标样式为“加载”
	pApp->MousePointerGlobalSet(IDC_Wait);

	// 清除状态栏提示
	formMain.Control(ID_lblStatus).TextSet(TEXT(""));

	HANDLE hFile = EFOpen(szFilePath, EF_OpStyle_Output);   // 打开文件

	//若打开出错则直接返回 false
	if(INVALID_HANDLE_VALUE == hFile){
		return false;
	}

	// 直接向文件输出字符串内容为控件 ID_txtMain 的内容
	// EF_LineSeed_None 表示输出后不要再多输出个换行符

	//以菜单栏中所选编码格式写入文档
	if(EFPrint(hFile, formMain.Control(appContext.txtIdShow).Text(),
			EF_LineSeed_None, -1i64, 1, TEXT("无法向文件中写入内容"), 
			appContext.status.asUTF8)
		< 0){
		//若返回值小于0 则写入出错
		//先关闭 hFile 再返回 false
		EFClose(hFile);		// 关闭文件
		return false;
	}
	EFClose(hFile);              // 关闭文件

	// 将文件名保存到全局变量
	appContext.szFilePath = szFilePath;

	//设置保存状态为 true
	appContext.status.isSaved = true;

	// 在窗体标题栏中显示：文件名 - 记事本
	formMain.TextSet(appContext.szFilePath);
	formMain.TextAdd(TEXT(" - 记事本"));
	// 在状态栏给出提示
	formMain.Control(ID_lblStatus).TextSet(StrAppend(TEXT("文件保存成功，已保存到:\""),
		appContext.szFilePath.c_str(), TEXT("\"!")));

	//设置鼠标样式为默认
	pApp->MousePointerGlobalSet(0);

	// 返回成功
	return true;
}



