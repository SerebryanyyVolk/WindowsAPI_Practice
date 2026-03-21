#include "resource.h"
#include "BForm.h"

CBForm form1(ID_form1);
CBForm form2(ID_form2);


void form1_Resize(void){
	form1.Control(ID_staticText1).Move(
		(form1.ClientWidth() - form1.Control(ID_staticText1).Width()) / 2,
		(form1.ClientHeight() - form1.Control(ID_staticText1).Height()) / 2
		);
}

void form1_Button1_Click(void){
	form2.Show();
}

void form2_Button2_Click(void){
	form1.Control(ID_staticText1).ForeColorSet(RGB(255, 0, 0));
}

int main(void){
	form1.EventAdd(0, eForm_Resize, form1_Resize);
	form1.EventAdd(ID_button1, eCommandButton_Click, form1_Button1_Click);
	form1.EventAdd(ID_button2, eCommandButton_Click, form2_Button2_Click);
    form1.IconSet(IDI_ICON1);
	form1.Show();
	form1_Resize();
	form1.Control(ID_staticText1).FontSizeSet(15);
	return 0;
}
