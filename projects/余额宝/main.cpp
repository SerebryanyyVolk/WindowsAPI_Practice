#include "resource.h"
#include "BForm.h"

//全局变量
//窗口类变量
CBForm form_main(ID_form_main);
CBForm form1(ID_form1);
CBForm form2(ID_form2);
CBForm form3(ID_form3);

//函数声明
void form_main_Load(void);
void form_main_QueryUnload(int pbCancel);
void form_main_Button1_Click(void);
void form_main_Button2_Click(void);
void form_main_Button3_Click(void);

void form1_Load(void);
void form1_KeyPressHandler(int keyAscii, int pbCancel);
void form1_TxtKeyPressHandler(int keyAscii, int pbCancel);
void form1_Button1_Click(void);
void form1_exitButton_Click(void);
void form1_Button3_Click(void);
void form1_Button4_Click(void);
void form1_lsvClickRight(int indexItem, int indexSubItem, int x, int y);
void form1_lsvDblClick(int indexItem, int indexSubItem, int x, int y);
void form1_InputTxtLsvEnterInput(bool blOKCancel, bool blListViewSetFocus);
void form1_InputTxtLsvLostFocus();
void form1_InputTxtLsvKeyPress(int keyAscii, int pbCancel);

void form2_Load(void);
void form2_KeyPressHandler(int keyAscii, int pbCancel);
void form2_TxtKeyPressHandler(int keyAscii, int pbCancel);
void form2_Button1_Click(void);
void form2_exitButton_Click(void);
void form2_Button3_Click(void);
void form2_Button4_Click(void);
void form2_lsvClickRight(int indexItem, int indexSubItem, int x, int y);
void form2_lsvDblClick(int indexItem, int indexSubItem, int x, int y);
void form2_InputTxtLsvEnterInput(bool blOKCancel, bool blListViewSetFocus);
void form2_InputTxtLsvLostFocus();
void form2_InputTxtLsvKeyPress(int keyAscii, int pbCancel);

void form3_Load(void);
void form3_KeyPressHandler(int keyAscii, int pbCancel);
void form3_TxtKeyPressHandler(int keyAscii, int pbCancel);
void form3_Button1_Click(void);
void form3_exitButton_Click(void);
void form3_Button3_Click(void);
void form3_Button4_Click(void);
void form3_lsvClickRight(int indexItem, int indexSubItem, int x, int y);
void form3_lsvDblClick(int indexItem, int indexSubItem, int x, int y);
void form3_InputTxtLsvEnterInput(bool blOKCancel, bool blListViewSetFocus);
void form3_InputTxtLsvLostFocus();
void form3_InputTxtLsvKeyPress(int keyAscii, int pbCancel);



//静态函数声明
static void list_view_Set(int lsv_id);
static void AddHis(CBForm * _form, UINT16 _id_lsv, double _rate, double _in, double _year, double _out);

static double CalcOutputMoney(double rate, double years, double inputMoney);
static double CalcInputMoney(double rate, double years, double outputMoney);
static double CalcYears(double rate, double inputMoney, double outputMoney);

int main(void){

	//设置主窗体事件
	form_main.EventAdd(0, eForm_Load, form_main_Load);
	form_main.EventAdd(0, eForm_QueryUnload, form_main_QueryUnload);
	form_main.EventAdd(ID_form_main_button1, eCommandButton_Click, form_main_Button1_Click);
	form_main.EventAdd(ID_form_main_button2, eCommandButton_Click, form_main_Button2_Click);
	form_main.EventAdd(ID_form_main_button3, eCommandButton_Click, form_main_Button3_Click);

	//设置窗体1（求时间）事件
	form1.EventAdd(0, eForm_Load, form1_Load);
	form1.EventAdd(0, eKeyPress, form1_KeyPressHandler);
	form1.EventAdd(ID_form1_inputTxt1, eKeyPress, form1_TxtKeyPressHandler);
	form1.EventAdd(ID_form1_inputTxt2, eKeyPress, form1_TxtKeyPressHandler);
	form1.EventAdd(ID_form1_inputTxt3, eKeyPress, form1_TxtKeyPressHandler);
	form1.EventAdd(ID_form1_button1, eCommandButton_Click, form1_Button1_Click);
	form1.EventAdd(ID_form1_button2, eCommandButton_Click, form1_exitButton_Click);
	form1.EventAdd(ID_form1_button3, eCommandButton_Click, form1_Button3_Click);
	form1.EventAdd(ID_form1_button4, eCommandButton_Click, form1_Button4_Click);
	form1.EventAdd(ID_form1_lsv, eListView_ClickRight, form1_lsvClickRight);

	form1.EventAdd(ID_form1_lsv, eListView_DblClick, form1_lsvDblClick);
	form1.EventAdd(ID_form1_inputTxtLsv, eLostFocus, form1_InputTxtLsvLostFocus);
	form1.EventAdd(ID_form1_inputTxtLsv, eKeyPress, form1_InputTxtLsvKeyPress);

	//设置窗体2（计算起始资金）事件
	form2.EventAdd(0, eForm_Load, form2_Load);
	form2.EventAdd(0, eKeyPress, form2_KeyPressHandler);
	form2.EventAdd(ID_form2_inputTxt1, eKeyPress, form2_TxtKeyPressHandler);
	form2.EventAdd(ID_form2_inputTxt2, eKeyPress, form2_TxtKeyPressHandler);
	form2.EventAdd(ID_form2_inputTxt3, eKeyPress, form2_TxtKeyPressHandler);
	form2.EventAdd(ID_form2_button1, eCommandButton_Click, form2_Button1_Click);
	form2.EventAdd(ID_form2_button2, eCommandButton_Click, form2_exitButton_Click);
	form2.EventAdd(ID_form2_button3, eCommandButton_Click, form2_Button3_Click);
	form2.EventAdd(ID_form2_button4, eCommandButton_Click, form2_Button4_Click);
	form2.EventAdd(ID_form2_lsv, eListView_ClickRight, form2_lsvClickRight);
	form2.EventAdd(ID_form2_lsv, eListView_DblClick, form2_lsvDblClick);
	form2.EventAdd(ID_form2_inputTxtLsv, eLostFocus, form2_InputTxtLsvLostFocus);
	form2.EventAdd(ID_form2_inputTxtLsv, eKeyPress, form2_InputTxtLsvKeyPress);

	//设置窗体3（计算所得存款）事件
	form3.EventAdd(0, eForm_Load, form3_Load);
	form3.EventAdd(0, eKeyPress, form3_KeyPressHandler);
	form3.EventAdd(ID_form3_inputTxt1, eKeyPress, form3_TxtKeyPressHandler);
	form3.EventAdd(ID_form3_inputTxt2, eKeyPress, form3_TxtKeyPressHandler);
	form3.EventAdd(ID_form3_inputTxt3, eKeyPress, form3_TxtKeyPressHandler);
	form3.EventAdd(ID_form3_button1, eCommandButton_Click, form3_Button1_Click);
	form3.EventAdd(ID_form3_button2, eCommandButton_Click, form3_exitButton_Click);
	form3.EventAdd(ID_form3_button3, eCommandButton_Click, form3_Button3_Click);
	form3.EventAdd(ID_form3_button4, eCommandButton_Click, form3_Button4_Click);
	form3.EventAdd(ID_form3_lsv, eListView_ClickRight, form3_lsvClickRight);
	form3.EventAdd(ID_form3_lsv, eListView_DblClick, form3_lsvDblClick);
	form3.EventAdd(ID_form3_inputTxtLsv, eLostFocus, form3_InputTxtLsvLostFocus);
	form3.EventAdd(ID_form3_inputTxtLsv, eKeyPress, form3_InputTxtLsvKeyPress);
	
	//设置图标（加载窗体）
	form_main.IconSet(IDI_ICON1);

	//展示主窗体
	form_main.Show();

	return 0;
}


//form_main事件函数定义	开始
void form_main_Load(void){
	form_main.Control(ID_form_main_txtTitle).FontSizeSet(15);
}

void form_main_QueryUnload(int pbCancel){
	//弹出确认退出窗口
	if(MsgBox(TEXT("确定要退出吗？"), TEXT("确认退出"), mb_YesNo, mb_IconQuestion) != idYes){
		//取消退出
		*(int *)pbCancel = 1;
		return;
	}
	else{
		//关闭所有窗口（自身自动Unload无需手动）
		form1.UnLoad();
		form2.UnLoad();
		form3.UnLoad();
	}
}

//点击按键弹出相应对话框
void form_main_Button1_Click(void){
	form1.IconSet(IDI_ICON1);
	form1.Show();
}

void form_main_Button2_Click(void){
	form2.IconSet(IDI_ICON1);
	form2.Show();
}

void form_main_Button3_Click(void){
	form3.IconSet(IDI_ICON1);
	form3.Show();
}
//form_main事件函数定义	结束


//form1事件函数定义	开始
void form1_Load(void){
	//设置标题字体大小
	form1.Control(ID_form1_txtTitle).FontSizeSet(15);

	//设置其他文字字体大小
	form1.Control(ID_form1_txt1).FontSizeSet(10);
	form1.Control(ID_form1_txt2).FontSizeSet(10);
	form1.Control(ID_form1_txt3).FontSizeSet(10);
	form1.Control(ID_form1_txt4).FontSizeSet(10);
	form1.Control(ID_form1_txt5).FontSizeSet(10);
	form1.Control(ID_form1_txt6).FontSizeSet(10);

	//初始化输入文本框
	form1.Control(ID_form1_inputTxt1).TextSet(2.0);
	form1.Control(ID_form1_inputTxt2).TextSet(1000);
	form1.Control(ID_form1_inputTxt3).TextSet(5000);

	//初始化ListView
	list_view_Set(ID_form1_lsv);
}

//窗体按键前处理函数
void form1_KeyPressHandler(int keyAscii, int pbCancel){
	switch(keyAscii){
	case 13:
		//如果按下Enter键则立刻计算
		form1_Button1_Click();
		break;
	case 27:
		//如果按下Esc键则退出窗口
		form1_exitButton_Click();
		break;
	default:
		break;
	}
}

//文本框按键前处理函数
void form1_TxtKeyPressHandler(int keyAscii, int pbCancel){
	//调用窗体按键前处理函数
	form1_KeyPressHandler(keyAscii, pbCancel);

	//如果输入的不是数字或'.'或'-'，则拦截
	if((keyAscii>'9'||keyAscii<'0') && '.'!=keyAscii && '-'!=keyAscii && 8!=keyAscii){
		*(int *)pbCancel = 1;
	}
}


void form1_Button1_Click(void){
	double inputMoney, outputMoney, years, rate;

	//从文本框获取年化收益率、存入金额和预期金额
	rate = form1.Control(ID_form1_inputTxt1).TextVal()/100.0;
	inputMoney = form1.Control(ID_form1_inputTxt2).TextVal();
	outputMoney = form1.Control(ID_form1_inputTxt3).TextVal();

	//若年化收益率、存入金额或最终金额任意一个小于0，则弹窗报错
	if(rate < 0){
		MsgBox(TEXT("年化收益率不能小于0！"), TEXT("错误"), mb_OK, mb_IconInformation);
		return;
	}
	if(inputMoney < 0){
		MsgBox(TEXT("起始金额不能小于0！"), TEXT("错误"), mb_OK, mb_IconInformation);
		return;
	}
	if (outputMoney < 0){
		MsgBox(TEXT("预期金额不能小于0！"), TEXT("错误"), mb_OK, mb_IconInformation);
		return;
	}
	//若最终金额小于存入金额，则弹窗报错
	if(inputMoney > outputMoney){
		MsgBox(TEXT("预期金额必须大于起始金额！"), TEXT("错误"), mb_OK, mb_IconInformation);
		return;
	}

	//计算年
	if(outputMoney == inputMoney){
		//若输入==输出显然年为0
		years = 0;
	}
	else if(0 == rate){
		//如果仅满足年化收益率为0，而输入!=输出，则不存在实数解，弹窗报错
		MsgBox(TEXT("无解！"), TEXT("错误"), mb_OK, mb_IconInformation);
		return;
	}
	else{
		//计算年
		years = CalcYears(rate, inputMoney, outputMoney);
	}

	//在结果框中打印结果
	form1.Control(ID_form1_txt5).TextSet(years);

	//在ListView里打印结果
	AddHis(&form1, ID_form1_lsv, rate, inputMoney, years, outputMoney);
}

void form1_exitButton_Click(void){
	//退出窗体1
	form1.UnLoad();
}

//将ListView中所有蓝白反色的选中项复制到剪贴板
void form1_Button3_Click(void){
	form1.Control(ID_form1_lsv).ItemsCopyToClipboard();
}

//删除ListView中选中项
void form1_Button4_Click(void){
	int iCount, i;
	iCount = form1.Control(ID_form1_lsv).ListCount();//共有项数
	if (form1.Control(ID_form1_lsv).ListViewCheckBoxes())
	{
		// 有勾选风格，以“勾选”为选中项
		for(i = iCount; i >= 1; i--)    //逆序删除；删除后其他项序号可不变
			if (form1.Control(ID_form1_lsv).ItemChecked(i))
				form1.Control(ID_form1_lsv).RemoveItem(i); //删除序号为 i 的项
	}
	else
	{
		// 无勾选风格，以“蓝色反白显示”为选中项
		for(i = iCount; i >= 1; i--)    //逆序删除；删除后其他项序号可不变
			if (form1.Control(ID_form1_lsv).ItemSelected(i))
				form1.Control(ID_form1_lsv).RemoveItem(i); //删除序号为 i 的项
	}
}

//右击ListView事件函数
void form1_lsvClickRight(int indexItem, int indexSubItem, int x, int y){
	// 所右击的单元格为：行号为indexItem、列号为indexSubItem
	// 右击时鼠标指针坐标为 (x, y) （控件坐标系）

	// 将数据恢复到文本框
	form1.Control(ID_form1_inputTxt1).TextSet(
		form1.Control(ID_form1_lsv).ItemText(indexItem, 2));
	form1.Control(ID_form1_inputTxt2).TextSet(
		form1.Control(ID_form1_lsv).ItemText(indexItem, 3));
	form1.Control(ID_form1_inputTxt3).TextSet(
		form1.Control(ID_form1_lsv).ItemText(indexItem, 5));
	//清空结果文本框
	form1.Control(ID_form1_txt5).TextSet(TEXT(""));
}

// 双击 ListView 的内容的事件函数：注意有4个参数
void form1_lsvDblClick(int indexItem, int indexSubItem, int x, int y)
{
	// 所双击的单元格为：行号为indexItem、列号为indexSubItem
	// 双击时鼠标指针坐标为 (x,y) （控件坐标系）

	// 启动编辑
	// 将所双击的单元格内容拷贝到文本框控件
	form1.Control(ID_form1_inputTxtLsv).TextSet(
		form1.Control(ID_form1_lsv).ItemText(indexItem, indexSubItem));
	// 获得所双击的单元格的矩形框范围，并自动设置文本框控件的大小和位置为该范围
	form1.Control(ID_form1_lsv).ListViewGetItemRect(
		indexItem, indexSubItem, form1.Control(ID_form1_inputTxtLsv).hWnd());
	// 使用文本框控件的附加数据功能（TagInt 和 TagInt2）分别保存
	// 本单元格的行号、列号（免去自行定义全局变量的麻烦）
	form1.Control(ID_form1_inputTxtLsv).TagIntSet(indexItem); // TagInt 保存行号
	form1.Control(ID_form1_inputTxtLsv).TagInt2Set(indexSubItem); // TagInt2 保存列号
	// 显示文本框控件
	form1.Control(ID_form1_inputTxtLsv).VisibleSet(true);
	// 调整文本框控件的 z-顺序，使位于顶层（覆盖在 ListView 控件之上）
	form1.Control(ID_form1_inputTxtLsv).ZOrder(0);
	// 让文本框控件获得输入焦点
	form1.Control(ID_form1_inputTxtLsv).SetFocus();
}

// 在 inputTxtLsv 文本框控件中结束输入
// blOKCancel==true 表确认编辑；==false表取消此次编辑
// blListViewSetFocus 是否随后让 ListView 控件获得输入焦点
void form1_InputTxtLsvEnterInput(bool blOKCancel, bool blListViewSetFocus)
{
	if (blOKCancel)
	{
		// 将 inputTxtLsv 中内容确认输入到对应的 ListView 控件的单元格
		// 若取消编辑，不执行此块，而直接执行if后的语句清除信息即可
		// 此次编辑所对应的单元格行号、列号，之前已被保存到文本框控件
		// 的附加数据 TagInt 和 TagInt2 中
		int idxItem = form1.Control(ID_form1_inputTxtLsv).TagInt();
		int idxSubItem = form1.Control(ID_form1_inputTxtLsv).TagInt2();
		if (idxItem>0)
			form1.Control(ID_form1_lsv).ItemTextSet(idxItem,
			form1.Control(ID_form1_inputTxtLsv).Text(), idxSubItem);
	}
	//清除信息并隐藏文本框
	form1.Control(ID_form1_inputTxtLsv).TagIntSet(0);
	form1.Control(ID_form1_inputTxtLsv).TagInt2Set(0);
	form1.Control(ID_form1_inputTxtLsv).TextSet(TEXT(""));
	if (blListViewSetFocus) form1.Control(ID_form1_lsv).SetFocus();
	form1.Control(ID_form1_inputTxtLsv).VisibleSet(false);
}

// 文本框控件失去输入焦点
void form1_InputTxtLsvLostFocus()
{
	// EnterInput 的参数2为 false，不强制让ListView获得输入焦点
	// 而保持用户所切换焦点的控件获得输入焦点
	form1_InputTxtLsvEnterInput(true, false);    // 确认编辑
}

void form1_InputTxtLsvKeyPress(int keyAscii, int pbCancel)
{
	if (13 == keyAscii)         // 按下回车键：确认编辑
	{
		form1_InputTxtLsvEnterInput(true, true);
		*(int *)pbCancel = 1;
	}
	else if (27 == keyAscii)    // 按下ESC键：取消编辑
	{
		form1_InputTxtLsvEnterInput(false, true);
		*(int *)pbCancel = 1;
	}
	// 设置 *(int *)pbCancel=1; 以取消按键，防止滴滴响声
	// 因在单行文本框中按回车、ESC都属非法按键，系统会发出滴声
	// EnterInput 的参数2为 true，随后让 ListView 控件获得焦点较好
	// 否则系统会按Tab顺序让其他控件获得焦点，易让其他控件误接收输入
}

//form1事件函数定义	结束


//form2事件函数定义	开始
void form2_Load(void){
	//设置标题字体大小
	form1.Control(ID_form2_txtTitle).FontSizeSet(15);

	//设置其他文字字体大小
	form1.Control(ID_form2_txt1).FontSizeSet(10);
	form1.Control(ID_form2_txt2).FontSizeSet(10);
	form1.Control(ID_form2_txt3).FontSizeSet(10);
	form1.Control(ID_form2_txt4).FontSizeSet(10);
	form1.Control(ID_form2_txt5).FontSizeSet(10);
	form1.Control(ID_form2_txt6).FontSizeSet(10);

	//初始化输入文本框
	form2.Control(ID_form2_inputTxt1).TextSet(2.0);
	form2.Control(ID_form2_inputTxt2).TextSet(5);
	form2.Control(ID_form2_inputTxt3).TextSet(5000);

	//初始化ListView
	list_view_Set(ID_form2_lsv);

}

//窗体按键前处理函数
void form2_KeyPressHandler(int keyAscii, int pbCancel){
	switch(keyAscii){
	case 13:
		//如果按下Enter键则立刻计算
		form2_Button1_Click();
		break;
	case 27:
		//如果按下Esc键则退出窗口
		form2_exitButton_Click();
		break;
	default:
		break;
	}
}
//文本框按键前处理函数
void form2_TxtKeyPressHandler(int keyAscii, int pbCancel){
	//调用窗体按键前处理函数
	form2_KeyPressHandler(keyAscii, pbCancel);
	
	//如果输入的不是数字或'.'或'-'，则拦截
	if((keyAscii>'9'||keyAscii<'0') && '.'!=keyAscii && '-'!=keyAscii && 8!=keyAscii){
		*(int *)pbCancel = 1;
	}
}

void form2_Button1_Click(void){
	double inputMoney, outputMoney, years, rate;

	//从文本框获取年化收益率、年和预期金额
	rate = form2.Control(ID_form2_inputTxt1).TextVal()/100.0;
	years = form2.Control(ID_form2_inputTxt2).TextVal();
	outputMoney = form2.Control(ID_form2_inputTxt3).TextVal();

	//如果年化收益率小于0，弹出错误提示
	if(rate < 0){
		MsgBox(TEXT("年化收益率不能小于0！"), TEXT("错误"), mb_OK, mb_IconInformation);
		return;
	}
	//如果预期金额小于等于0则弹出错误提示
	if (outputMoney < 0){
		MsgBox(TEXT("预期金额不能小于0！"), TEXT("错误"), mb_OK, mb_IconInformation);
		return;
	}
	//若年小于0，弹窗报错
	if(years < 0 ){
		MsgBox(TEXT("年份不能小于等于0！"), TEXT("错误"), mb_OK, mb_IconInformation);
		return;
	}

	//计算预先存入金额（向上取整，精度为1分钱）
	inputMoney = CalcInputMoney(rate, years, outputMoney);

	//在结果框中打印结果
	form2.Control(ID_form2_txt5).TextSet(inputMoney);

	//在ListView里打印结果
	AddHis(&form2, ID_form2_lsv, rate, inputMoney, years, outputMoney);
}

void form2_exitButton_Click(void){
	//退出窗体2
	form2.UnLoad();
}
//将ListView中所有蓝白反色的选中项复制到剪贴板
void form2_Button3_Click(void){
	form2.Control(ID_form2_lsv).ItemsCopyToClipboard();
}

//删除ListView中选中项
void form2_Button4_Click(void){
	int iCount, i;
	iCount = form2.Control(ID_form2_lsv).ListCount();//共有项数
	if (form2.Control(ID_form2_lsv).ListViewCheckBoxes())
	{
		// 有勾选风格，以“勾选”为选中项
		for(i = iCount; i >= 1; i--)    //逆序删除；删除后其他项序号可不变
			if (form2.Control(ID_form2_lsv).ItemChecked(i))
				form2.Control(ID_form2_lsv).RemoveItem(i); //删除序号为 i 的项
	}
	else
	{
		// 无勾选风格，以“蓝色反白显示”为选中项
		for(i = iCount; i >= 1; i--)    //逆序删除；删除后其他项序号可不变
			if (form2.Control(ID_form2_lsv).ItemSelected(i))
				form2.Control(ID_form2_lsv).RemoveItem(i); //删除序号为 i 的项
	}
}

//右击ListView事件函数
void form2_lsvClickRight(int indexItem, int indexSubItem, int x, int y){
	// 所右击的单元格为：行号为indexItem、列号为indexSubItem
	// 右击时鼠标指针坐标为 (x, y) （控件坐标系）

	// 将数据恢复到文本框
	form2.Control(ID_form2_inputTxt1).TextSet(
		form2.Control(ID_form2_lsv).ItemText(indexItem, 2));
	form2.Control(ID_form2_inputTxt2).TextSet(
		form2.Control(ID_form2_lsv).ItemText(indexItem, 4));
	form2.Control(ID_form2_inputTxt3).TextSet(
		form2.Control(ID_form2_lsv).ItemText(indexItem, 5));
	//清空结果文本框
	form2.Control(ID_form2_txt5).TextSet(TEXT(""));
}

// 双击 ListView 的内容的事件函数：注意有4个参数
void form2_lsvDblClick(int indexItem, int indexSubItem, int x, int y)
{
	// 所双击的单元格为：行号为indexItem、列号为indexSubItem
	// 双击时鼠标指针坐标为 (x,y) （控件坐标系）

	// 启动编辑
	// 将所双击的单元格内容拷贝到文本框控件
	form2.Control(ID_form2_inputTxtLsv).TextSet(
		form2.Control(ID_form2_lsv).ItemText(indexItem, indexSubItem));
	// 获得所双击的单元格的矩形框范围，并自动设置文本框控件的大小和位置为该范围
	form2.Control(ID_form2_lsv).ListViewGetItemRect(
		indexItem, indexSubItem, form2.Control(ID_form2_inputTxtLsv).hWnd());
	// 使用文本框控件的附加数据功能（TagInt 和 TagInt2）分别保存
	// 本单元格的行号、列号（免去自行定义全局变量的麻烦）
	form2.Control(ID_form2_inputTxtLsv).TagIntSet(indexItem); // TagInt 保存行号
	form2.Control(ID_form2_inputTxtLsv).TagInt2Set(indexSubItem); // TagInt2 保存列号
	// 显示文本框控件
	form2.Control(ID_form2_inputTxtLsv).VisibleSet(true);
	// 调整文本框控件的 z-顺序，使位于顶层（覆盖在 ListView 控件之上）
	form2.Control(ID_form2_inputTxtLsv).ZOrder(0);
	// 让文本框控件获得输入焦点
	form2.Control(ID_form2_inputTxtLsv).SetFocus();
}

// 在 inputTxtLsv 文本框控件中结束输入
// blOKCancel==true 表确认编辑；==false表取消此次编辑
// blListViewSetFocus 是否随后让 ListView 控件获得输入焦点
void form2_InputTxtLsvEnterInput(bool blOKCancel, bool blListViewSetFocus)
{
	if (blOKCancel)
	{
		// 将 inputTxtLsv 中内容确认输入到对应的 ListView 控件的单元格
		// 若取消编辑，不执行此块，而直接执行if后的语句清除信息即可
		// 此次编辑所对应的单元格行号、列号，之前已被保存到文本框控件
		// 的附加数据 TagInt 和 TagInt2 中
		int idxItem = form2.Control(ID_form2_inputTxtLsv).TagInt();
		int idxSubItem = form2.Control(ID_form2_inputTxtLsv).TagInt2();
		if (idxItem>0)
			form2.Control(ID_form2_lsv).ItemTextSet(idxItem,
			form2.Control(ID_form2_inputTxtLsv).Text(), idxSubItem);
	}
	//清除信息并隐藏文本框
	form2.Control(ID_form2_inputTxtLsv).TagIntSet(0);
	form2.Control(ID_form2_inputTxtLsv).TagInt2Set(0);
	form2.Control(ID_form2_inputTxtLsv).TextSet(TEXT(""));
	if (blListViewSetFocus) form2.Control(ID_form2_lsv).SetFocus();
	form2.Control(ID_form2_inputTxtLsv).VisibleSet(false);
}

// 文本框控件失去输入焦点
void form2_InputTxtLsvLostFocus()
{
	// EnterInput 的参数2为 false，不强制让ListView获得输入焦点
	// 而保持用户所切换焦点的控件获得输入焦点
	form2_InputTxtLsvEnterInput(true, false);    // 确认编辑
}

void form2_InputTxtLsvKeyPress(int keyAscii, int pbCancel)
{
	if (13 == keyAscii)         // 按下回车键：确认编辑
	{
		form2_InputTxtLsvEnterInput(true, true);
		*(int *)pbCancel = 1;
	}
	else if (27 == keyAscii)    // 按下ESC键：取消编辑
	{
		form2_InputTxtLsvEnterInput(false, true);
		*(int *)pbCancel = 1;
	}
	// 设置 *(int *)pbCancel=1; 以取消按键，防止滴滴响声
	// 因在单行文本框中按回车、ESC都属非法按键，系统会发出滴声
	// EnterInput 的参数2为 true，随后让 ListView 控件获得焦点较好
	// 否则系统会按Tab顺序让其他控件获得焦点，易让其他控件误接收输入
}
//form2事件函数定义	结束


//form3事件函数定义	开始
void form3_Load(void){
	//设置标题字体大小
	form1.Control(ID_form3_txtTitle).FontSizeSet(15);

	//设置其他文字字体大小
	form1.Control(ID_form3_txt1).FontSizeSet(10);
	form1.Control(ID_form3_txt2).FontSizeSet(10);
	form1.Control(ID_form3_txt3).FontSizeSet(10);
	form1.Control(ID_form3_txt4).FontSizeSet(10);
	form1.Control(ID_form3_txt5).FontSizeSet(10);
	form1.Control(ID_form3_txt6).FontSizeSet(10);

	//初始化输入文本框
	form3.Control(ID_form3_inputTxt1).TextSet(2.0);
	form3.Control(ID_form3_inputTxt2).TextSet(5);
	form3.Control(ID_form3_inputTxt3).TextSet(1000);

	//初始化ListView
	list_view_Set(ID_form3_lsv);

}

//窗体按键前处理函数
void form3_KeyPressHandler(int keyAscii, int pbCancel){
	switch(keyAscii){
	case 13:
		//如果按下Enter键则立刻计算
		form3_Button1_Click();
		break;
	case 27:
		//如果按下Esc键则退出窗口
		form3_exitButton_Click();
		break;
	default:
		break;
	}
}
//文本框按键前处理函数
void form3_TxtKeyPressHandler(int keyAscii, int pbCancel){
	//调用窗体按键前处理函数
	form3_KeyPressHandler(keyAscii, pbCancel);

	//如果输入的不是数字或'.'或'-'，则拦截
	if((keyAscii>'9'||keyAscii<'0') && '.'!=keyAscii && '-'!=keyAscii && 8!=keyAscii){
		*(int *)pbCancel = 1;
	}
}

void form3_Button1_Click(void){
	double inputMoney, outputMoney, years, rate;

	//从文本框获取年化收益率、年、起始金额
	rate = form3.Control(ID_form3_inputTxt1).TextVal()/100.0;
	years = form3.Control(ID_form3_inputTxt2).TextVal();
	inputMoney = form3.Control(ID_form3_inputTxt3).TextVal();

	//若收益率小于0弹窗报错
	if(rate < 0){
		MsgBox(TEXT("年化收益率不能小于0！"), TEXT("错误"), mb_OK, mb_IconInformation);
		return;
	}
	//若起始金额小于0弹窗报错
	if(inputMoney < 0){
		MsgBox(TEXT("起始存入金额不能小于0！"), TEXT("错误"), mb_OK, mb_IconInformation);
		return;
	}
	//若年份小于0弹窗报错
	if(years < 0){
		MsgBox(TEXT("年份不能小于0！"), TEXT("错误"), mb_OK, mb_IconInformation);
		return;
	}

	outputMoney = CalcOutputMoney(rate, years, inputMoney);

	form3.Control(ID_form3_txt5).TextSet(outputMoney);
	AddHis(&form3, ID_form3_lsv, rate, inputMoney, years, outputMoney);
}

void form3_exitButton_Click(void){
	//退出窗体3
	form3.UnLoad();
}

//将ListView中所有蓝白反色的选中项复制到剪贴板
void form3_Button3_Click(void){
	form3.Control(ID_form3_lsv).ItemsCopyToClipboard();
}

//删除ListView中选中项
void form3_Button4_Click(void){
	int iCount, i;
	iCount = form3.Control(ID_form3_lsv).ListCount();//共有项数
	if (form3.Control(ID_form3_lsv).ListViewCheckBoxes())
	{
		// 有勾选风格，以“勾选”为选中项
		for(i = iCount; i >= 1; i--)    //逆序删除；删除后其他项序号可不变
			if (form3.Control(ID_form3_lsv).ItemChecked(i))
				form3.Control(ID_form3_lsv).RemoveItem(i); //删除序号为 i 的项
	}
	else
	{
		// 无勾选风格，以“蓝色反白显示”为选中项
		for(i = iCount; i >= 1; i--)    //逆序删除；删除后其他项序号可不变
			if (form3.Control(ID_form3_lsv).ItemSelected(i))
				form3.Control(ID_form3_lsv).RemoveItem(i); //删除序号为 i 的项
	}
}

//右击ListView事件函数
void form3_lsvClickRight(int indexItem, int indexSubItem, int x, int y){
	// 所右击的单元格为：行号为indexItem、列号为indexSubItem
	// 右击时鼠标指针坐标为 (x, y) （控件坐标系）

	// 将数据恢复到文本框
	form3.Control(ID_form3_inputTxt1).TextSet(
		form3.Control(ID_form3_lsv).ItemText(indexItem, 2));
	form3.Control(ID_form3_inputTxt2).TextSet(
		form3.Control(ID_form3_lsv).ItemText(indexItem, 4));
	form3.Control(ID_form3_inputTxt3).TextSet(
		form3.Control(ID_form3_lsv).ItemText(indexItem, 3));
	//清空结果文本框
	form3.Control(ID_form3_txt5).TextSet(TEXT(""));
}

// 双击 ListView 的内容的事件函数：注意有4个参数
void form3_lsvDblClick(int indexItem, int indexSubItem, int x, int y)
{
	// 所双击的单元格为：行号为indexItem、列号为indexSubItem
	// 双击时鼠标指针坐标为 (x,y) （控件坐标系）

	// 启动编辑
	// 将所双击的单元格内容拷贝到文本框控件
	form3.Control(ID_form3_inputTxtLsv).TextSet(
		form3.Control(ID_form3_lsv).ItemText(indexItem, indexSubItem));
	// 获得所双击的单元格的矩形框范围，并自动设置文本框控件的大小和位置为该范围
	form3.Control(ID_form3_lsv).ListViewGetItemRect(
		indexItem, indexSubItem, form3.Control(ID_form3_inputTxtLsv).hWnd());
	// 使用文本框控件的附加数据功能（TagInt 和 TagInt2）分别保存
	// 本单元格的行号、列号（免去自行定义全局变量的麻烦）
	form3.Control(ID_form3_inputTxtLsv).TagIntSet(indexItem); // TagInt 保存行号
	form3.Control(ID_form3_inputTxtLsv).TagInt2Set(indexSubItem); // TagInt2 保存列号
	// 显示文本框控件
	form3.Control(ID_form3_inputTxtLsv).VisibleSet(true);
	// 调整文本框控件的 z-顺序，使位于顶层（覆盖在 ListView 控件之上）
	form3.Control(ID_form3_inputTxtLsv).ZOrder(0);
	// 让文本框控件获得输入焦点
	form3.Control(ID_form3_inputTxtLsv).SetFocus();
}

// 在 inputTxtLsv 文本框控件中结束输入
// blOKCancel==true 表确认编辑；==false表取消此次编辑
// blListViewSetFocus 是否随后让 ListView 控件获得输入焦点
void form3_InputTxtLsvEnterInput(bool blOKCancel, bool blListViewSetFocus)
{
	if (blOKCancel)
	{
		// 将 inputTxtLsv 中内容确认输入到对应的 ListView 控件的单元格
		// 若取消编辑，不执行此块，而直接执行if后的语句清除信息即可
		// 此次编辑所对应的单元格行号、列号，之前已被保存到文本框控件
		// 的附加数据 TagInt 和 TagInt2 中
		int idxItem = form3.Control(ID_form3_inputTxtLsv).TagInt();
		int idxSubItem = form3.Control(ID_form3_inputTxtLsv).TagInt2();
		if (idxItem>0)
			form3.Control(ID_form3_lsv).ItemTextSet(idxItem,
			form3.Control(ID_form3_inputTxtLsv).Text(), idxSubItem);
	}
	//清除信息并隐藏文本框
	form3.Control(ID_form3_inputTxtLsv).TagIntSet(0);
	form3.Control(ID_form3_inputTxtLsv).TagInt2Set(0);
	form3.Control(ID_form3_inputTxtLsv).TextSet(TEXT(""));
	if (blListViewSetFocus) form3.Control(ID_form3_lsv).SetFocus();
	form3.Control(ID_form3_inputTxtLsv).VisibleSet(false);
}

// 文本框控件失去输入焦点
void form3_InputTxtLsvLostFocus()
{
	// EnterInput 的参数2为 false，不强制让ListView获得输入焦点
	// 而保持用户所切换焦点的控件获得输入焦点
	form3_InputTxtLsvEnterInput(true, false);    // 确认编辑
}

void form3_InputTxtLsvKeyPress(int keyAscii, int pbCancel)
{
	if (13 == keyAscii)         // 按下回车键：确认编辑
	{
		form3_InputTxtLsvEnterInput(true, true);
		*(int *)pbCancel = 1;
	}
	else if (27 == keyAscii)    // 按下ESC键：取消编辑
	{
		form3_InputTxtLsvEnterInput(false, true);
		*(int *)pbCancel = 1;
	}
	// 设置 *(int *)pbCancel=1; 以取消按键，防止滴滴响声
	// 因在单行文本框中按回车、ESC都属非法按键，系统会发出滴声
	// EnterInput 的参数2为 true，随后让 ListView 控件获得焦点较好
	// 否则系统会按Tab顺序让其他控件获得焦点，易让其他控件误接收输入
}
//form3事件函数定义	结束



//静态函数定义	开始

//新建ListView
static void list_view_Set(int lsv_id){
	//窗口1、2、3共用一套ListView设置

	//设置 ListView 字体大小
	form1.Control(lsv_id).FontSizeSet(10);

	//设置 ListView 控件属性
	form1.Control(lsv_id).ListViewGridLinesSet(true);
	form1.Control(lsv_id).ListViewFullRowSelectSet(true);

	//新建各列（参数1、2、3依次为列标题、列宽、对齐方式）
	form1.Control(lsv_id).ListViewAddColumn(TEXT("序号"), 50, 0);
	form1.Control(lsv_id).ListViewAddColumn(TEXT("年化收益率（%）"), 110, 0);
	form1.Control(lsv_id).ListViewAddColumn(TEXT("开始存入（元）"), 100, 0);
	form1.Control(lsv_id).ListViewAddColumn(TEXT("存入时间（年）"), 100, 0);
	form1.Control(lsv_id).ListViewAddColumn(TEXT("最后总金额（元）"), 110, 0);
}

//在ListView里添加一行数据
static void AddHis(CBForm * _form, UINT16 _id_lsv, double _rate, double _in, double _year, double _out){
	static int iNumIncre = 0;
	int idxNew;

	//收益率带单位（%）需乘100
	_rate *= 100;
	idxNew = _form->Control(_id_lsv).AddItem(Str(++iNumIncre));
	//年化收益率
	_form->Control(_id_lsv).ItemTextSet(idxNew, Str(_rate), 2);
	//开始存入
	_form->Control(_id_lsv).ItemTextSet(idxNew, Str(_in), 3);
	//存入时间
	_form->Control(_id_lsv).ItemTextSet(idxNew, Str(_year), 4);
	//最后总金额
	_form->Control(_id_lsv).ItemTextSet(idxNew, Str(_out), 5);
}

//已知收益率，开始存入金额、预期金额，求年
//使用对数换底公式，要求rate>0，结果向上取整
static double CalcYears(double rate, double inputMoney, double outputMoney){
	return ceil(log10(outputMoney/inputMoney)/log10(1+rate));
}

//已知收益率、年、预期金额，求开始存入金额，结果向上取整
static double CalcInputMoney(double rate, double years, double outputMoney){
	return ceil(outputMoney / pow(1+rate, years) / 100.0) * 100.0;
}

//已知收益率、年、开始存入金额，求最终金额，结果向下取整
static double CalcOutputMoney(double rate, double years, double inputMoney){
	return floor(inputMoney * pow(1+rate, years) * 100)/100;
}
//静态函数定义	结束
