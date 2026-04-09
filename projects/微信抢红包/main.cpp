#include "resource.h"
#include "BForm.h"
#include "CRedEnvelope.h"

CBForm form1(ID_form1);

//pack1,2,3分别对应红包1,2,3
CRedEnvelope pack1(100, 3, 0);
CRedEnvelope pack2(200, 5, 0);
CRedEnvelope pack3;

void Form1_Load(void);
//窗口退出弹出确认窗口
void Form1_QueryUnload(int pbCancel);

void ButtonGrab1_Click(void);
void ButtonLook1_Click(void);

void ButtonGrab2_Click(void);
void ButtonLook2_Click(void);

void Form1_TxtKeyPressHandler(int keyAscii, int pbCancel);
void ButtonSet3_Click(void);
void ButtonGrab3_Click(void);
void ButtonLook3_Click(void);

int main(void){
	//窗体事件注册
	form1.EventAdd(0, eForm_Load, Form1_Load);
	form1.EventAdd(0, eForm_QueryUnload, Form1_QueryUnload);

	//红包1事件注册
	form1.EventAdd(ID_buttonGrab1, eCommandButton_Click, ButtonGrab1_Click);
	form1.EventAdd(ID_buttonLook1, eCommandButton_Click, ButtonLook1_Click);

	//红包2事件注册
	form1.EventAdd(ID_buttonGrab2, eCommandButton_Click, ButtonGrab2_Click);
	form1.EventAdd(ID_buttonLook2, eCommandButton_Click, ButtonLook2_Click);

	//红包3事件注册
	form1.EventAdd(ID_inputTxtMoney, eKeyPress, Form1_TxtKeyPressHandler);
	form1.EventAdd(ID_inputTxtSize, eKeyPress, Form1_TxtKeyPressHandler);
	form1.EventAdd(ID_buttonSet3, eCommandButton_Click, ButtonSet3_Click);
	form1.EventAdd(ID_buttonGrab3, eCommandButton_Click, ButtonGrab3_Click);
	form1.EventAdd(ID_buttonLook3, eCommandButton_Click, ButtonLook3_Click);

    form1.IconSet(IDI_ICON1);
	form1.Show();
	return 0;
}

void Form1_Load(void){
	//将红包封面贴到左上角
	form1.Control(ID_pic).PictureSet(IDB_BITMAP1);
	form1.Control(ID_pic).StretchSet(true);

	//将红包1和红包2的输出文本清空
	form1.Control(ID_txt1).TextSet(TEXT(""));
	form1.Control(ID_txt2).TextSet(TEXT(""));

	//设置红包3抢红包和查看按钮为不可按，因为此时还没塞钱进红包
	form1.Control(ID_buttonGrab3).EnabledSet(false);
	form1.Control(ID_buttonLook3).EnabledSet(false);

	//设置红包1查看按键不可用，因为刚开始没有历史记录
	form1.Control(ID_buttonLook1).EnabledSet(false);

	//在左侧列表中显示提示
	form1.Control(ID_list).AddItem(TEXT("点击“查看”键在此查看历史记录"));

	//先让机器人抢一次红包2
	pack2.OpenOne(TEXT("机器人2"));
}

void Form1_QueryUnload(int pbCancel){
	MsgBeep(mb_SoundQuestion);
	if(idYes != MsgBox(TEXT("确定要退出吗？"), TEXT("退出"), mb_YesNo, mb_IconQuestion)){
		*((int *)pbCancel) = 1;
	}
}

void ButtonGrab1_Click(void){
	//如果用户名为弹窗提示输入用户名
	if(0 == *form1.Control(ID_inputTxt1).Text()){
		MsgBox(TEXT("请先输入用户名称！"), TEXT("提示"), mb_OK, mb_IconExclamation);
		return;
	}
	//使用用户名开一份红包（同名不能重复开模式）
	int iGet = pack1.OpenOne(form1.Control(ID_inputTxt1).Text());
	//如果返回-1说明红包开完了，弹窗提醒
	if(-1 == iGet){
		MsgBox(TEXT("来晚了，红包抢完了"), TEXT("没抢到"), mb_OK, mb_IconExclamation);
	}
	else if(-2 == iGet){//返回-2说明这个用户名已经开过红包了，弹窗提醒
		MsgBox(TEXT("同一用户不能重复抢红包"), TEXT("提示"), mb_OK, mb_IconExclamation);
	}
	else{
		//成功开红包，弹窗提示钱数
		LPTSTR msg =StrAppend(
						TEXT("恭喜！"),
						form1.Control(ID_inputTxt1).Text(),
						TEXT("抢到"),
						Str(iGet/100.0),
						TEXT("元红包！")
						);
		MsgBox(msg, TEXT("抢到红包！"), mb_OK, mb_IconInformation);
		//在输入用户名下方的文本框中显示钱数
		form1.Control(ID_txt1).TextSet(msg);
		form1.Control(ID_buttonLook1).EnabledSet(true);
	}
}

void ButtonLook1_Click(void){
	//初始化手气最佳索引
	int bestIdx = pack1.GetBestIdx();
	//获得开过红包个数
	int opened = pack1.GetPartsOpened();
	//获得剩余红包个数
	int remained = pack1.GetPartsRemained();
	//初始化消息字符串
	LPTSTR msg;
	//初始化空PART变量，此变量用于读取PartsPop返回值
	PART temp;
	memset(&temp, 0, sizeof(temp));

	//清空列表
	form1.Control(ID_list).ListClear();

	//清空pop索引值，从首个抢红包用户开始获得用户信息
	pack1.ClearPartsPopIdx();

	//pop opened 次，获取所有开过红包的用户名及其钱数
	for(int i = 0;i<opened;i++){
		temp = pack1.PartsPop();

		//拼接msg变量，展示谁开出来多少元
		//如果pop索引正好为bestIdx，则msg尾部加上手气最佳提示
		if(bestIdx == i){
			msg = StrAppend(
				temp.account,
				TEXT("  抢到  "),
				Str(temp.cents/100.0),
				TEXT("元  手气最佳!")
				);
		}
		else{
			msg = StrAppend(
				temp.account,
				TEXT("  抢到  "),
				Str(temp.cents/100.0),
				TEXT("元")
				);
		}

		//在列表控件中显示
		form1.Control(ID_list).AddItem(msg);
	}

	//展示剩下红包数
	//若开完则说明
	if(remained > 0){
		msg = StrAppend(TEXT("还有  "), Str(remained), TEXT("  份红包没开！"));
		form1.Control(ID_list).AddItem(msg);
	}
	else{
		form1.Control(ID_list).AddItem(TEXT("所有红包均开完！"));
	}
}

void ButtonGrab2_Click(void){
	//如果用户名为弹窗提示输入用户名
	if(0 == *form1.Control(ID_inputTxt2).Text()){
		MsgBox(TEXT("请先输入用户名称！"), TEXT("提示"), mb_OK, mb_IconExclamation);
		return;
	}
	//使用用户名开一份红包（同名不能重复开模式）
	int iGet = pack2.OpenOne(form1.Control(ID_inputTxt2).Text());
	//如果返回-1说明红包开完了，弹窗提醒
	if(-1 == iGet){
		MsgBox(TEXT("来晚了，红包抢完了"), TEXT("没抢到"), mb_OK, mb_IconExclamation);
	}
	else if(-2 == iGet){//返回-2说明这个用户名已经开过红包了，弹窗提醒
		MsgBox(TEXT("同一用户不能重复抢红包"), TEXT("提示"), mb_OK, mb_IconExclamation);
	}
	else{
		//成功开红包，弹窗提示钱数
		LPTSTR msg =StrAppend(
			TEXT("恭喜！"),
			form1.Control(ID_inputTxt2).Text(),
			TEXT("抢到"),
			Str(iGet/100.0),
			TEXT("元红包！")
			);
		MsgBox(msg, TEXT("抢到红包！"), mb_OK, mb_IconInformation);
		//在输入用户名下方的文本框中显示钱数
		form1.Control(ID_txt2).TextSet(msg);
	}
}

void ButtonLook2_Click(void){
	//初始化手气最佳索引
	int bestIdx = pack2.GetBestIdx();
	//获得开过红包个数
	int opened = pack2.GetPartsOpened();
	//获得剩余红包个数
	int remained = pack2.GetPartsRemained();
	//初始化消息字符串
	LPTSTR msg;
	//初始化空PART变量，此变量用于读取PartsPop返回值
	PART temp;
	memset(&temp, 0, sizeof(temp));
	
	//清空列表
	form1.Control(ID_list).ListClear();

	//清空pop索引值
	pack2.ClearPartsPopIdx();

	//pop opened 次，获取所有开过红包的用户名及其钱数
	for(int i = 0;i<opened;i++){
		temp = pack2.PartsPop();

		//拼接msg变量，展示谁开出来多少元
		//如果pop索引正好为bestIdx，则msg尾部加上手气最佳提示
		if(bestIdx == i){
			msg = StrAppend(
				temp.account,
				TEXT("  抢到  "),
				Str(temp.cents/100.0),
				TEXT("元  手气最佳!")
				);
		}
		else{
			msg = StrAppend(
				temp.account,
				TEXT("  抢到  "),
				Str(temp.cents/100.0),
				TEXT("元")
				);
		}

		//在列表控件中显示
		form1.Control(ID_list).AddItem(msg);
	}

	//展示剩下红包数
	//若开完则说明
	if(remained > 0){
		msg = StrAppend(TEXT("还有  "), Str(remained), TEXT("  份红包没开！"));
		form1.Control(ID_list).AddItem(msg);
	}
	else{
		form1.Control(ID_list).AddItem(TEXT("所有红包均开完！"));
	}
}

//文本框按键前处理函数
void Form1_TxtKeyPressHandler(int keyAscii, int pbCancel){
	//如果输入的不是数字或'.'，则拦截
	if((keyAscii>'9'||keyAscii<'0') && '.'!=keyAscii && 8!=keyAscii){
		*(int *)pbCancel = 1;
	}
}

void ButtonSet3_Click(void){
	//检查输入是否合法（具体看MsgBox中的文本）
	if(0 == *form1.Control(ID_inputTxtMoney).Text()){
		MsgBox(TEXT("请输入放入的总钱数！"), TEXT("提示"), mb_OK, mb_IconExclamation);
		return;
	}
	if(0 == *form1.Control(ID_inputTxtMoney).Text()){
		MsgBox(TEXT("请输入放入的总钱数！"), TEXT("提示"), mb_OK, mb_IconExclamation);
		return;
	}
	
	double money = form1.Control(ID_inputTxtMoney).TextVal();
	int size = form1.Control(ID_inputTxtSize).TextInt();

	if(money <= 0){
		MsgBox(TEXT("红包不能装入0元！"), TEXT("提示"), mb_OK, mb_IconExclamation);
		return;
	}
	if(size <= 0){
		MsgBox(TEXT("红包不能分成0份！"), TEXT("提示"), mb_OK, mb_IconExclamation);
		return;
	}
	if((int)(money*100) <= size){
		MsgBox(TEXT("钱数太少或分数太多！\n红包每份至少要有0.01元！"), TEXT("提示"), mb_OK, mb_IconExclamation);
		return;
	}

	//塞钱进红包
	if(pack3.EnvelopeSet((int)(money*100), size) == false){
		//若还有问题可能是程序出bug了，弹窗提示
		MsgBox(TEXT("塞钱进红包失败！\n未知错误"), TEXT("提示"), mb_OK, mb_IconError);
		return;
	}

	//使能抢红包按钮控件和查看按钮控件
	form1.Control(ID_buttonGrab3).EnabledSet(true);
	form1.Control(ID_buttonLook3).EnabledSet(true);

	//弹窗提示塞钱进红包成功
	MsgBox(TEXT("塞钱进红包成功！\n可以开始抢红包了！"), TEXT("提示"), mb_OK, mb_IconInformation);
}

void ButtonGrab3_Click(void){
	//如果用户名为弹窗提示输入用户名
	if(0 == *form1.Control(ID_inputTxt3).Text()){
		MsgBox(TEXT("请先输入用户名称！"), TEXT("提示"), mb_OK, mb_IconExclamation);
		return;
	}
	//使用用户名开一份红包（同名不能重复开模式）
	int iGet = pack3.OpenOne(form1.Control(ID_inputTxt3).Text());
	//如果返回-1说明红包开完了，弹窗提醒
	if(-1 == iGet){
		MsgBox(TEXT("来晚了，红包抢完了"), TEXT("没抢到"), mb_OK, mb_IconExclamation);
	}
	else if(-2 == iGet){//返回-2说明这个用户名已经开过红包了，弹窗提醒
		MsgBox(TEXT("同一用户不能重复抢红包"), TEXT("提示"), mb_OK, mb_IconExclamation);
	}
	else{
		//成功开红包，弹窗提示钱数
		LPTSTR msg =StrAppend(
			TEXT("恭喜！"),
			form1.Control(ID_inputTxt3).Text(),
			TEXT("抢到"),
			Str(iGet/100.0),
			TEXT("元红包！")
			);
		MsgBox(msg, TEXT("抢到红包！"), mb_OK, mb_IconInformation);
	}
}

void ButtonLook3_Click(void){
	//初始化手气最佳索引
	int bestIdx = pack3.GetBestIdx();
	//获得开过红包个数
	int opened = pack3.GetPartsOpened();
	//获得剩余红包个数
	int remained = pack3.GetPartsRemained();
	//初始化消息字符串
	LPTSTR msg;
	//初始化空PART变量，此变量用于读取PartsPop返回值
	PART temp;
	memset(&temp, 0, sizeof(temp));

	//清空列表
	form1.Control(ID_list).ListClear();

	//清空pop索引值
	pack3.ClearPartsPopIdx();

	//pop opened 次，获取所有开过红包的用户名及其钱数
	for(int i = 0;i<opened;i++){
		temp = pack3.PartsPop();

		//拼接msg变量，展示谁开出来多少元
		//如果pop索引正好为bestIdx，则msg尾部加上手气最佳提示
		if(bestIdx == i){
			msg = StrAppend(
				temp.account,
				TEXT("  抢到  "),
				Str(temp.cents/100.0),
				TEXT("元  手气最佳!")
				);
		}
		else{
			msg = StrAppend(
				temp.account,
				TEXT("  抢到  "),
				Str(temp.cents/100.0),
				TEXT("元")
				);
		}

		//在列表控件中显示
		form1.Control(ID_list).AddItem(msg);
	}

	//展示剩余红包数
	//若开完则说明
	if(remained > 0){
		msg = StrAppend(TEXT("还有  "), Str(remained), TEXT("  份红包没开！"));
		form1.Control(ID_list).AddItem(msg);
	}
	else{
		form1.Control(ID_list).AddItem(TEXT("所有红包均开完！"));
	}
}
