#include "resource.h"
#include "BForm.h"

CBForm form_main(ID_form_main);
CBForm form1(ID_form1);
CBForm form2(ID_form2);
CBForm form3(ID_form3);

void form_main_Load(void);
void form1_Load(void);
void form2_Load(void);
void form3_Load(void);

void form_main_Button1_Click(void);
void form_main_Button2_Click(void);
void form_main_Button3_Click(void);

int main(void){
    form_main.IconSet(IDI_ICON1);
	form1.IconSet(IDI_ICON1);

	form_main.EventAdd(0, eForm_Load, form_main_Load);
	form_main.EventAdd(ID_form_main_button1, eCommandButton_Click, form_main_Button1_Click);
	form_main.EventAdd(ID_form_main_button2, eCommandButton_Click, form_main_Button2_Click);
	form_main.EventAdd(ID_form_main_button3, eCommandButton_Click, form_main_Button3_Click);

	form1.EventAdd(0, eForm_Load, form1_Load);

	form_main.Show();

	return 0;
}

void form_main_Load(void){
	form_main.Control(ID_form_main_txtTitle).FontSizeSet(15);
}

void form1_Load(void){
	form1.Control(ID_form1_txt1).FontSizeSet(15);
	form1.Control(ID_form1_txt1).FontSizeSet(10);
	form1.Control(ID_form1_txt2).FontSizeSet(10);
	form1.Control(ID_form1_txt3).FontSizeSet(10);
	form1.Control(ID_form1_txt4).FontSizeSet(10);
	form1.Control(ID_form1_txt5).FontSizeSet(10);
	form1.Control(ID_form1_txt6).FontSizeSet(10);
}

void form3_Load(void){

}

void form_main_Button1_Click(void){
	form1.Show();
}

void form_main_Button2_Click(void){
	form2.Show();
}

void form_main_Button3_Click(void){
	form3.Show();
}
