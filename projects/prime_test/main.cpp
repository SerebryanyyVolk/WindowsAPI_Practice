#include "resource.h"
#include "BForm.h"

CBForm form1(ID_form1);

static bool is_prime(int val);

void form1_Button1_Click(void){
	form1.Control(ID_outputTxt1).TextSet(TEXT(""));
	BOOL retSuccess[1];
	TCHAR outputTxt[80];
	TCHAR * inputTxt = form1.Control(ID_inputTxt1).Text();
	int inputInt = form1.Control(ID_inputTxt1).TextInt(retSuccess);
	memset(outputTxt, 0, sizeof(outputTxt));

	form1.Control(ID_inputTxt1).SetFocus();
	if(inputTxt[0] == 0){
		return;
	}
	if(*retSuccess==false){
		form1.Control(ID_outputTxt1).TextSet(TEXT("不支持超大质数验证！"));
		return;
	}
	form1.Control(ID_outputTxt1).TextSet(TEXT("计算中..."));
	if(is_prime(inputInt)){
		wsprintf(outputTxt, _T("%s%s"), inputTxt,TEXT("是质数"));
		form1.Control(ID_outputTxt1).TextSet(TEXT("是质数！"));
		form1.Control(ID_txtList1).AddItem(outputTxt);
	}
	else{
		wsprintf(outputTxt, _T("%s%s"), inputTxt,TEXT("不是质数"));
		form1.Control(ID_outputTxt1).TextSet(TEXT("不是质数！"));
		form1.Control(ID_txtList1).AddItem(outputTxt);
	}
	
}

void form1_Button2_Click(void){
	form1.Control(ID_outputTxt1).TextSet(TEXT(""));
	BOOL retSuccess[1];
	TCHAR outputTxt[80];
	int outputInt;
	TCHAR * inputTxt = form1.Control(ID_inputTxt1).Text();
	int inputInt = form1.Control(ID_inputTxt1).TextInt(retSuccess);
	memset(outputTxt, 0, sizeof(outputTxt));

	form1.Control(ID_inputTxt1).SetFocus();
	if(inputTxt[0] == 0){
		return;
	}
	if(*retSuccess==false || inputInt == MAXINT){
		form1.Control(ID_outputTxt1).TextSet(TEXT("不支持超大质数验证！"));
		return;
	}
	form1.Control(ID_outputTxt1).TextSet(TEXT("计算中..."));
	for(outputInt=(inputInt+1);is_prime(outputInt)==false;outputInt++){
		if(outputInt==MAXINT){
			form1.Control(ID_outputTxt1).TextSet(TEXT("超出验证范围！"));
			return;
		}
	}
	wsprintf(outputTxt,_T("%s%s%s%d"),TEXT("大于"),inputTxt,TEXT("的最小质数是："),outputInt);
	form1.Control(ID_outputTxt1).TextSet(outputTxt);
	form1.Control(ID_txtList1).AddItem(outputTxt);
}

void form1_Button3_Click(void){
	form1.Control(ID_outputTxt1).TextSet(TEXT(""));
	BOOL retSuccess[1];
	TCHAR outputTxt[100];
	int outputInt[5];
	TCHAR * inputTxt = form1.Control(ID_inputTxt1).Text();
	int inputInt = form1.Control(ID_inputTxt1).TextInt(retSuccess);
	memset(outputTxt, 0, sizeof(outputTxt));
	memset(outputInt, 0, sizeof(outputInt));

	form1.Control(ID_inputTxt1).SetFocus();
	if(inputTxt[0] == 0){
		return;
	}
	if(*retSuccess==false || inputInt == MAXINT){
		form1.Control(ID_outputTxt1).TextSet(TEXT("不支持超大质数验证！"));
		return;
	}
	form1.Control(ID_outputTxt1).TextSet(TEXT("计算中..."));
	for(int i=0;i<5;i++){
		if(0==i){
			outputInt[i]=inputInt++;
		}
		else{
			outputInt[i]=outputInt[i-1]+1;
		}
		for(;is_prime(outputInt[i])==false;outputInt[i]++){
			if(outputInt[i]==MAXINT){
				form1.Control(ID_outputTxt1).TextSet(TEXT("超出验证范围！"));
				return;
			}
		}
	}
	wsprintf(outputTxt,
		_T("%s%s%s%d,%d,%d,%d,%d"),
		TEXT("大于"),inputTxt,TEXT("的最小5个质数是："),
		outputInt[0],outputInt[1],outputInt[2],outputInt[3],outputInt[4]);

	form1.Control(ID_outputTxt1).TextSet(outputTxt);
	form1.Control(ID_txtList1).AddItem(outputTxt);
}

void form1_ButtonExit_Click(void){
	if(MsgBox(	TEXT("确定退出？"),
			TEXT("退出"),
			mb_YesNo,
			mb_IconQuestion
			)==idYes){
				form1.UnLoad();
	}
}

int main(void){
	//initialize the static function "is_prime"
	is_prime(3);

	form1.EventAdd(ID_button1, eCommandButton_Click, form1_Button1_Click);
	form1.EventAdd(ID_button2, eCommandButton_Click, form1_Button2_Click);
	form1.EventAdd(ID_button3, eCommandButton_Click, form1_Button3_Click);
	form1.EventAdd(ID_buttonExit, eCommandButton_Click, form1_ButtonExit_Click);
    form1.IconSet(IDI_ICON1);
	form1.Show();

	form1.Control(ID_txt1).FontSizeSet(13);
	form1.Control(ID_txt4).FontSizeSet(11);
	form1.Control(ID_txtList1).FontSizeSet(10);
	form1.Control(ID_inputTxt1).SetFocus();

	return 0;
}


static bool is_prime(int val){
	static int primeCache[512] = {0};
	static bool initBool = false;
	float sqrtVal = sqrt((float)val);

	//initialize primeCache
	if(initBool == false){
		int temp = -1;
		primeCache[0] = 2;
		for(int j=1;j<512;j++){
			for(temp = primeCache[j-1];temp != primeCache[j];){
				temp++;
				float sqrtTemp = sqrt((float)temp);
				for(int i = 0; i<j; i++){
					if(temp%primeCache[i]==0){
						break;
					}
					else if(primeCache[i]>sqrtTemp){
						primeCache[j] = temp;
						break;
					}
					else{
						continue;
					}
				}
			}
		}
		initBool = true;
	}

	//traverse primeCache and check if val is divisible by any of them
	if(val < 2){
		return false;
	}
	for(int i = 0; i < 512; i++){
		if(val==primeCache[i]){
			return true;
		}
		else if(val%primeCache[i]==0){
			return false;
		}
		else if(primeCache[i]>sqrtVal){
			return true;
		}
	}

	//if val exceeds the range that primeCache can support
	//traverse all possible divisors
	for(int i = primeCache[511];;i++){
		if(val % i == 0){
			return false;
		}
		else if(i>sqrtVal){
			return true;
		}
	}
}