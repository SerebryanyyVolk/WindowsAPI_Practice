#include "resource.h"
#include "BForm.h"
#include "zip.h"

CBForm form1(ID_form1);

void Form1_Load(void);
void form1_QueryUnload(int pCancel);
void ListView_FileDrop(int ptrArrFiles, int fileCnt, int mouseX, int mouseY);

void ButtonZip_CreateZip(void);
void ButtonExit_Form1Exit(void);

static void CreateZip(LPTSTR path);

int main(void){
	form1.EventAdd(0, eForm_Load, Form1_Load);
	form1.EventAdd(0, eForm_QueryUnload, form1_QueryUnload);

	form1.EventAdd(ID_listViewFile, eFilesDrop, ListView_FileDrop);

	form1.EventAdd(ID_buttonZip, eCommandButton_Click, ButtonZip_CreateZip);
	form1.EventAdd(ID_buttonExit, eCommandButton_Click, ButtonExit_Form1Exit);

    form1.IconSet(IDI_ICON1);
	form1.Show();
	return 0;
}

void Form1_Load(void){
	/***
	初始化主窗口各项设置，包括图标、ComboBox
	***/

	//初始化 ListView 组件
	form1.Control(ID_listViewFile).ListViewAddColumn(TEXT("文件路径"), 400, 0);//宽度500，左对齐
	form1.Control(ID_listViewFile).AddItem(TEXT("向此处拖拽文件"));
}

void form1_QueryUnload(int pCancel){
	if(MsgBox(TEXT("确定要退出吗？"), TEXT("退出"), mb_YesNo, mb_IconQuestion)==idNo){
		*((int*)pCancel)=1;
	}
}

void ListView_FileDrop(int ptrArrFiles, int fileCnt, int mouseX, int mouseY){
	static int lastFileCnt = 0;
	TCHAR ** ptr = (TCHAR **)ptrArrFiles;

	if(0 == lastFileCnt){
		form1.Control(ID_listViewFile).RemoveItem(1);
		form1.Control(ID_listViewFile).ListViewCheckBoxesSet(true);
	}

	for(int i=lastFileCnt+1;i <= fileCnt;i++){
		form1.Control(ID_listViewFile).AddItem(ptr[i]);
	}
}

void ButtonZip_CreateZip(void){
	//初始化中间变量
	bool ret = false;
	int iCount, i;
	iCount = form1.Control(ID_listViewFile).ListCount();

	if (form1.Control(ID_listViewFile).ListViewCheckBoxes())
	{
		// 有勾选风格，以“勾选”为选中项
		for(i = iCount; i >= 1; i--)    //逆序压缩
			if (form1.Control(ID_listViewFile).ItemChecked(i)){
				CreateZip(form1.Control(ID_listViewFile).ItemText(i)); //压缩序号为 i 的项
			}	
	}
	else
	{
		// 无勾选风格，以“蓝色反白显示”为选中项
		for(i = iCount; i >= 1; i--)    //逆序压缩
			if (form1.Control(ID_listViewFile).ItemSelected(i)){
				CreateZip(form1.Control(ID_listViewFile).ItemText(i)); //压缩序号为 i 的项
			}
	}
}

void ButtonExit_Form1Exit(void){
	form1.UnLoad();
}

void CreateZip(LPTSTR path){
	bool ret = false;

	TCHAR * tempPath = new TCHAR[MAX_PATH];
	TCHAR * fileName = new TCHAR[MAX_PATH];
	TCHAR * zipPath = new TCHAR[MAX_PATH];
	memset(tempPath, 0, MAX_PATH);
	memset(zipPath, 0, MAX_PATH);
	memset(fileName, 0, MAX_PATH);

	GetFileNameWithoutExt(path, fileName);
	GetDirectoryFromFilePath(path, tempPath);

	_stprintf_s(zipPath, MAX_PATH, TEXT("%s\\%s%s"), tempPath, fileName, TEXT(".zip"));

	delete [] tempPath;
	delete [] fileName;

	if (Path_File == GetPathType(path)){
		ret = CreateZipFromFile(path, zipPath);
	}
	else if(Path_Directory == GetPathType(path)){
		ret = CreateZipFromFolder(path, zipPath);
	}

	delete [] zipPath;

	if(false == ret){
		if(idRetry == MsgBox(TEXT("压缩发生错误！"), TEXT("错误"), mb_RetryCancel, mb_IconError)){
			CreateZip(path);
		}
		else{
			return;
		}
	}
	else{
		MsgBox(TEXT("压缩成功！"), TEXT("成功"), mb_OK, mb_IconInformation);
	}
	return;
}







