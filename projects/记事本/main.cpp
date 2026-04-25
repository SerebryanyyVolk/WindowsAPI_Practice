#include "main.h"

CBForm formMain(ID_formMain);
TMain m_form;			//主窗口结构体变量

int main(void){
	formMain.EventAdd(0, eForm_Load, FormMain_Load);
	formMain.EventAdd(0, eForm_Resize, FormMain_Resize);
	formMain.EventAdd(0, eMenu_Click, FormMain_MenuClick);
	
	formMain.Show();

	return 0;
}

void FormMain_Load(void){
	formMain.IconSet(IDI_ICON6);	//设置图标

	m_form.status.showStatus = true;	//开始运行时默认显示状态栏
	m_form.status.topMost = false;		//开始运行时默认窗口不置顶
	FormMain_Resize();				//摆放控件

	formMain.Control(ID_txtMain).FontSizeSet(14);	//设置文本框字体大小为较大
	formMain.Control(ID_lblStatus).TextSet(TEXT("欢迎使用！"));	//设置状态栏初始文字
}

void FormMain_Resize(void){
	int cyStatus = 0;
	if(m_form.status.showStatus){
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
	switch(menuID){
	case ID_mnuFileNew:
		//直接在控制台运行 "记事本.exe"，相当于windows帮忙实现新建
		if((INT_PTR)ShellExecute(NULL, TEXT("open"), 
								StrAppend(pApp->Path(), TEXT("记事本.exe")),
								NULL, NULL, SW_SHOWNORMAL)
			<= 32)
		{
			MsgBox(TEXT("新建时发生未知错误！"),TEXT("错误"), mb_OK, mb_IconError);	//若ShellExecute返回值小于等于32
		}																			//则说明exe运行失败，弹窗报错
		break;

	case ID_mnuFileOpen:
		//OsdSetFilter(TEXT("文本文档(*.txt)|*.txt"), true);
		//szFile = OsdOpenDlg();
		//if(*szFile == TEXT('\0')){
		//}
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
		m_form.status.showStatus = !m_form.status.showStatus;	//切换状态栏标志位状态
		formMain.Control(ID_lblStatus).VisibleSet(m_form.status.showStatus);	//改变状态栏透明状态
		formMain.Menu(ID_mnuViewState).CheckedSet(m_form.status.showStatus);	//改变菜单中状态栏项前面有无对勾的状态
		FormMain_Resize();		//因为状态栏控件出现或隐藏会改变控件布局，故需要重新排布控件位置
		break;

	case ID_mnuViewTopMost:		//点击窗口置顶选项
		m_form.status.topMost = !m_form.status.topMost;		//切换窗口置顶标志位状态
		//切换窗口置顶状态
		if(m_form.status.topMost){		
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
		formMain.Menu(ID_mnuViewTopMost).CheckedSet(m_form.status.topMost);
		break;

	case ID_mnuHelpAbout:	//点击关于键，显示版本等信息
		MsgBox(TEXT("记事本    版本:1.0\r\n作者:不玩神碑\r\n\r\n版权所有(C)    2026"),
			TEXT("关于 记事本"), mb_OK, mb_IconInformation);
		break;

	default:
		break;

	}
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
	_tcscpy(m_form.szFileName, szFileName);
	// 在窗体标题栏中显示：文件名 - 我的记事本
	formMain.TextSet(m_form.szFileName);
	formMain.TextAdd(TEXT(" - 记事本"));
	// 在状态栏给出提示
	formMain.Control(ID_lblStatus).TextSet(StrAppend(TEXT("文件保存成功，已保存到:\""), m_form.szFileName, TEXT("\"!")));

	// 返回成功
	return true;
}