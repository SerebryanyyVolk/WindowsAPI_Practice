#include "resource.h"
#include "BForm.h"

CBForm form1(ID_form1);

static bool checkTxtFormat(TCHAR * pText){
	TCHAR lastChar;
	if('.'==*pText){
		return false;
	}
	else if('-'==*pText){
		lastChar = *pText;
		pText ++;
	}
	while(*pText != 0){
		lastChar = *pText;
		if((*pText<='9'&& *pText>='0')|| '.'==*pText){
			pText ++;
		}
		else{
			return false;
		}
	}
	if(('.'==lastChar) || ('-'==lastChar)){
		return false;
	}
	return true;
}

void form1_Button1_Click(void){
	double a,b;
	TCHAR * pText1 = form1.Control(ID_inputTxtData1).Text();
	TCHAR * pText2 = form1.Control(ID_inputTxtData2).Text();
	if((checkTxtFormat(pText1)==false) || (checkTxtFormat(pText2)==false)){
		form1.Control(ID_outputTxtResult).TextSet(TEXT("输入格式错误"));
	}
	else{
		a = form1.Control(ID_inputTxtData1).TextVal();
		b = form1.Control(ID_inputTxtData2).TextVal();
		form1.Control(ID_outputTxtResult).TextSet(a+b);
	}
}

void form1_Button2_Click(void){
	double a,b;
	TCHAR * pText1 = form1.Control(ID_inputTxtData1).Text();
	TCHAR * pText2 = form1.Control(ID_inputTxtData2).Text();
	if((checkTxtFormat(pText1)==false) || (checkTxtFormat(pText2)==false)){
		form1.Control(ID_outputTxtResult).TextSet(TEXT("输入格式错误"));
	}
	else{
		a = form1.Control(ID_inputTxtData1).TextVal();
		b = form1.Control(ID_inputTxtData2).TextVal();
		form1.Control(ID_outputTxtResult).TextSet(a-b);
	}
}

void form1_Button3_Click(void){
	double a,b;
	TCHAR * pText1 = form1.Control(ID_inputTxtData1).Text();
	TCHAR * pText2 = form1.Control(ID_inputTxtData2).Text();
	if((checkTxtFormat(pText1)==false) || (checkTxtFormat(pText2)==false)){
		form1.Control(ID_outputTxtResult).TextSet(TEXT("输入格式错误"));
	}
	else{
		a = form1.Control(ID_inputTxtData1).TextVal();
		b = form1.Control(ID_inputTxtData2).TextVal();
		form1.Control(ID_outputTxtResult).TextSet(a*b);
	}
}

void form1_Button4_Click(void){
	double a,b;
	TCHAR * pText1 = form1.Control(ID_inputTxtData1).Text();
	TCHAR * pText2 = form1.Control(ID_inputTxtData2).Text();
	if((checkTxtFormat(pText1)==false) || (checkTxtFormat(pText2)==false)){
		form1.Control(ID_outputTxtResult).TextSet(TEXT("输入格式错误"));
	}
	else{
		a = form1.Control(ID_inputTxtData1).TextVal();
		b = form1.Control(ID_inputTxtData2).TextVal();
		if(0==b){
			form1.Control(ID_outputTxtResult).TextSet(TEXT("除数不能为0"));
		}
		else{
			form1.Control(ID_outputTxtResult).TextSet(a/b);
		}
	} 
}

void form1_ButtonExit_Click(void){
	if(MsgBox(TEXT("确定要退出吗？"), TEXT("退出"), mb_YesNo, mb_IconQuestion)==idYes){
		form1.UnLoad();
	}
}

int main(void){
	form1.EventAdd(ID_button1, eCommandButton_Click, form1_Button1_Click);
	form1.EventAdd(ID_button2, eCommandButton_Click, form1_Button2_Click);
	form1.EventAdd(ID_button3, eCommandButton_Click, form1_Button3_Click);
	form1.EventAdd(ID_button4, eCommandButton_Click, form1_Button4_Click);
	form1.EventAdd(ID_buttonExit, eCommandButton_Click, form1_ButtonExit_Click);
    form1.IconSet(IDI_ICON1);
	form1.Show();
	return 0;
}
