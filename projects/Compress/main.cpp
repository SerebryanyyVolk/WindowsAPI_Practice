#include "resource.h"
#include "BForm.h"
#include "zip.h"

CBForm form1(ID_form1);

void Form1_Load(void);
void form1_QueryUnload(int pCancel);
void ListView_FileDrop(int ptrArrFiles, int fileCnt, int mouseX, int mouseY);

void ButtonZip_CreateZip(void);
void ButtonExit_Form1Exit(void);

static bool CreateZip(LPTSTR path);

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
	
	form1.Control(ID_listViewFile).ListViewAddColumn(TEXT("文件路径"),
		form1.Control(ID_listViewFile).Width(),
		0);//宽度与ListView相同，左对齐
	form1.Control(ID_listViewFile).AddItem(TEXT("向此处拖拽文件"));
}

void form1_QueryUnload(int pCancel){
	if(MsgBox(TEXT("确定要退出吗？"), TEXT("退出"), mb_YesNo, mb_IconQuestion)==idNo){
		*((int*)pCancel)=1;
	}
}

void ListView_FileDrop(int ptrArrFiles, int fileCnt, int mouseX, int mouseY){
	static bool initCheckBox = false;
	TCHAR ** ptr = (TCHAR **)ptrArrFiles;

	if(false == initCheckBox){
		form1.Control(ID_listViewFile).RemoveItem(1);
		form1.Control(ID_listViewFile).ListViewCheckBoxesSet(true);
		initCheckBox = true;
	}

	for(int i=1;i <= fileCnt;i++){
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
				if(false == CreateZip(form1.Control(ID_listViewFile).ItemText(i, 1)))//压缩序号为 i 的项
					return;//若失败且不重试则直接返回
			}
	}
	else
	{
		// 无勾选风格，以“蓝色反白显示”为选中项
		for(i = iCount; i >= 1; i--)    //逆序压缩
			if (form1.Control(ID_listViewFile).ItemSelected(i)){
				if(false == CreateZip(form1.Control(ID_listViewFile).ItemText(i, 1)))//压缩序号为 i 的项
					return; //若失败且不重试则直接返回
			}
	}
	MsgBox(TEXT("压缩成功！"), TEXT("成功"), mb_OK, mb_IconInformation);
}

void ButtonExit_Form1Exit(void){
	form1.UnLoad();
}

bool CreateZip(LPTSTR path){
	eZipErrCode ret = eZip_ErrUnknown;

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
	else{
		ret = eZip_ErrSrcFile;
	}

	delete [] zipPath;
	
	if(eZip_OK != ret){
		TCHAR * ErrBoxText = new TCHAR[100];

		switch(ret){
		case eZip_ErrUnknown:
			_stprintf_s(ErrBoxText, 100, TEXT("压缩发生错误！错误码:%d!\n%s"), 
				ret, TEXT("未知错误！"));
			break;
		case eZip_ErrSrcFile:
			_stprintf_s(ErrBoxText, 100, TEXT("压缩发生错误！错误码:%d!\n%s"),
				ret, TEXT("无法访问源文件！"));
			break;
		case eZip_ErrTgtFile:
			_stprintf_s(ErrBoxText, 100, TEXT("压缩发生错误！错误码:%d!\n%s"), 
				ret, TEXT("无法对目标路径进行写入操作！"));
			break;
		case eZip_ErrTgtClose:
			_stprintf_s(ErrBoxText, 100, TEXT("压缩发生错误！错误码:%d!\n%s"), 
				ret, TEXT("无法关闭写入文件流！"));
			break;
		default:
			_stprintf_s(ErrBoxText, 100, TEXT("压缩发生错误！错误码:%d!\n"), 
				ret);
			break;
		}
		
		if(idRetry == MsgBox(ErrBoxText, TEXT("错误"), mb_RetryCancel, mb_IconError)){
			delete [] ErrBoxText;
			CreateZip(path);
		}
		else{
			delete [] ErrBoxText;
		}
		return false;
	}
	//else{
	//	//MsgBox(TEXT("压缩成功！"), TEXT("成功"), mb_OK, mb_IconInformation);
	//}
	return true;
}







