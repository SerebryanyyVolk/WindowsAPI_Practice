#include "resource.h"
#include "BForm.h"

CBForm formLogin(ID_formLogin);
CBForm formMain(ID_formMainWnd);

void FormLogin_Load(void);
void FormLogin_QueryUnload(int pbCancel);
void FormLogin_CmdLogin(void);
void FormLogin_CmdExit(void);

void FormMain_Load(void);
void FormMain_QueryUnload(int pbCancel);
void FormMain_CmdExitMain(void);
void FormMain_CmdLogout(void);
void FormMain_TxtHandler(void);

int main(void){
	//注册登录窗体事件
	formLogin.EventAdd(0, eForm_Load, FormLogin_Load);
	formLogin.EventAdd(0, eForm_QueryUnload, FormLogin_QueryUnload);
	formLogin.EventAdd(ID_buttonLogin, eCommandButton_Click, FormLogin_CmdLogin);
	formLogin.EventAdd(ID_buttonExit, eCommandButton_Click, FormLogin_CmdExit);

	//注册主窗体事件
	formMain.EventAdd(0, eForm_Load, FormMain_Load);
	formMain.EventAdd(0, eForm_QueryUnload, FormMain_QueryUnload);
	formMain.EventAdd(ID_buttonExitMain, eCommandButton_Click, FormMain_CmdExitMain);
	formMain.EventAdd(ID_buttonLogout, eCommandButton_Click, FormMain_CmdLogout);
	formMain.EventAdd(ID_inputTxt, eEdit_Change, FormMain_TxtHandler);

	formLogin.Show();
	return 0;
}

void FormLogin_Load(void){
	//设置登录窗体图标
	formLogin.IconSet(IDI_ICON1);

	//窗体居中
	formLogin.Move(
		(pApp->ScreenWidth() - formLogin.Width()) / 2,
		(pApp->ScreenHeight() - formLogin.Height()) / 2
		);

	//将封面贴到窗体上
	formLogin.Control(ID_picCover).PictureSet(IDB_BITMAP1);
	formLogin.Control(ID_picCover).StretchSet(true);
}

void FormLogin_QueryUnload(int pbCancel){
	//先检查登录界面是否被隐藏
	//若被隐藏说明用户已经登录成功，直接退出
	if(formLogin.Visible()){
		//未被隐藏说明未登录，则弹窗询问是否退出
		if(idYes != MsgBox(TEXT("要退出吗？"), TEXT("退出"), mb_YesNo, mb_IconQuestion))
			*(int *)pbCancel = 1;
	}
}

void FormLogin_CmdLogin(void){
	//检查密码是否正确，如果正确隐藏登录窗体并显示主窗体
	//否则弹窗提示密码错误，并清空密码文本框
	tstring inputPwd = formLogin.Control(ID_inputTxtPwd).Text();
	if(inputPwd == TEXT("Pass")){
		//隐藏登录窗体
		formLogin.Hide();
		formMain.Show();
	}
	else{
		MsgBox(TEXT("密码错误！请重新输入密码！"), 
			TEXT("密码错误"), mb_OK, mb_IconExclamation);
		formLogin.Control(ID_inputTxtPwd).TextSet(TEXT(""));
	}
}

void FormLogin_CmdExit(void){
	//用户点击“退出”按钮则退出登录界面
	formLogin.UnLoad();
}

void FormMain_Load(void){
	//设置主窗体图标
	formMain.IconSet(IDI_ICON1);

	//窗体居中
	formMain.Move(
		(pApp->ScreenWidth() - formMain.Width()) / 2,
		(pApp->ScreenHeight() - formMain.Height()) / 2
		);

	//设置初始文本
	tstring sInit(TEXT("命令行内容：\r\n"));
	sInit += pApp->Command();
	formMain.Control(ID_outputTxt).TextSet(sInit);
}

void FormMain_QueryUnload(int pbCancel){
	//如果窗体未被隐藏
	if(formMain.Visible()){
		//弹窗询问是否退出
		if(idYes != MsgBox(TEXT("要退出吗？"), 
			TEXT("退出"), mb_YesNo, mb_IconQuestion)){
			*(int *)pbCancel = 1;
		}
		else{//若确认退出还得先将隐藏的登录窗体退出
			formLogin.UnLoad();
		}
	}
	//若窗体被隐藏则直接退出
}

void FormMain_CmdExitMain(void){
	//用户点击“退出”按钮
	formMain.UnLoad();
}

void FormMain_CmdLogout(void){
	//用户点击“退出登录”按钮
	//隐藏主窗体，通知QueryUnload事件函数不用弹窗提示
	//再退出主窗体
	formMain.Hide();
	formMain.UnLoad();
	//显示登录窗体并置顶
	SetWindowPos(formLogin.hWnd(), HWND_TOP, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
}

void FormMain_TxtHandler(void){
	tstring sText;					//用户在文本框控件中输入的内容
	tstring sResult(TEXT(""));		//在listbox中展示分点内容
	sText = formMain.Control(ID_inputTxt).Text();

	//显示内容
	sResult += TEXT("您输入的内容是：");
	sResult += sText;
	sResult += TEXT("\r\n");

	//求字符串长度
	int iLen = sText.length();
	sResult = sResult + TEXT("字符串的长度是：") + Str(iLen) + TEXT("\r\n");

	//显示字符串的字符
	if(iLen > 0){
		sResult += TEXT("字符串的第一个字符是：[");
		sResult = sResult + sText[0] + TEXT("]");
		if(iLen > 4){
			sResult = sResult + TEXT("，第五个字符是：[") + sText[4] + TEXT("]。\r\n");
		}
		else{
			sResult += TEXT("。\r\n");
		}
	}

	//在字符串中查找 "abc"
	int iPos = 0;
	iPos = sText.find(TEXT("abc"));
	if(iPos != tstring::npos){
		sResult = sResult + TEXT("字符串包含[abc]，在字符位置：") +
			Str(iPos) + TEXT("\r\n");
	}
	else{
		sResult += TEXT("在字符串中未找到[abc]。\r\n");
	}

	//在字符串中寻找字符'o'的总数
	int cnt = 0;		//'o'的总数
	iPos = -1;		//下一处'o'的位置

	while(true){
		//从 ipos 的下一个位置找'o'，结果存入iPos
		//若iPos != tstring::npos则表示没找到，否则就是找到
		iPos = sText.find(TEXT("o"), iPos+1);
		if(iPos != tstring::npos)
			cnt++;
		else
			break;
	}
	sResult = sResult + TEXT("字符串含有 ") + Str(cnt) + TEXT(" 个[o]\r\n");

	//使用','分割字符串
	TCHAR ** p;
	cnt = Split(sText.c_str(), p, TEXT(","));
	sResult = sResult + TEXT("\r\n按逗号分割的子字符串是：\r\n");
	for(int i=1;i<=cnt;i++){
		sResult = sResult + TEXT("[") + Str(i) + TEXT("]");
		sResult = sResult + p[i] + TEXT("\r\n");
	}

	//在输出文本框中显示结果
	formMain.Control(ID_outputTxt).TextSet(sResult);
}
