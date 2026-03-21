#include "resource.h"
#include "BForm.h"

CBForm form1(ID_form1);

void button1_Click(void){
	form1.Control(ID_edit1).TextSet(TEXT("abcdefg"));
}

void form1_QueryUnload(int pCancel){
	if(MsgBox(TEXT("确定要退出吗？"), TEXT("退出"), mb_YesNo, mb_IconQuestion)==idNo){
		*((int*)pCancel)=1;
	}
}

int main(void){
	form1.EventAdd(ID_button1, eCommandButton_Click, button1_Click);
	form1.EventAdd(0, eForm_QueryUnload, form1_QueryUnload);
    form1.IconSet(IDI_ICON1);
	form1.Show();
	return 0;
}
