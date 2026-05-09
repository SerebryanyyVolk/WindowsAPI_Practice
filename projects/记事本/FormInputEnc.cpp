#include "main.h"
#include "resource.h"

//加密窗体对象
CBForm formKey(ID_formInputKey);

//是否输入密码标志位
static bool m_fEncInputOK = false;

void FormKey_Load(void){
	//初始化密码字符串指针
	cFMain.encKey = NULL;
	//初始化是否输入密码标志位
	m_fEncInputOK = 0;

	//初始化窗体图标
	formKey.IconSet(IDI_ICON7);
}

void FormKey_CmdOK_Click(void){
	//读取密码
	LPTSTR szText = formKey.Control(ID_txtEnc).Text();
	//若输入密码为空则指针指向 NULL
	if(TEXT('\0') == *szText){
		cFMain.encKey = NULL;
		m_fEncInputOK = 0;
	}
	else{
		//若输入密码不为空则设置标志位
		cFMain.encKey = StrConvFromUnicode(szText);
		m_fEncInputOK = 1;
	}
	//卸载窗体退出模态
	formKey.UnLoad();
}

void FormKey_CmdCancel_Click(void){
	cFMain.encKey = NULL;
	m_fEncInputOK = 0;
	formKey.UnLoad();
}

bool InputEncrypt(void){
	//加载输入窗体
	formKey.Load();

	//将即将弹出的窗口位置放到主窗口中央
	formKey.Move(
		formMain.Left() + (formMain.Width() - formKey.Width())/2,
		formMain.Top() + (formMain.Height() - formKey.Height())/2
		);

	//弹出输入窗口
	formKey.Show(1, formMain.hWnd());

	//窗体卸载后将窗口置顶
	formMain.ZOrder();

	return m_fEncInputOK;
}

void FormKey_EventsMapInit(void){
	//注册各类事件
	formKey.EventAdd(0, eForm_Load, FormKey_Load);
	formKey.EventAdd(ID_cmdOK, eCommandButton_Click, FormKey_CmdOK_Click);
	formKey.EventAdd(ID_cmdCancel, eCommandButton_Click, FormKey_CmdCancel_Click);
}
