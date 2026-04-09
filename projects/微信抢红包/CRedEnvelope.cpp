#include "CRedEnvelope.h"
#include <cstdlib>
#include <ctime>
#include <cstring>


CRedEnvelope::CRedEnvelope(void){
	parts = NULL;
	EnvelopeSet(100, 1);
}

CRedEnvelope::CRedEnvelope(int _mode){
	parts = NULL;
	EnvelopeSet(100, 1, _mode);
}

CRedEnvelope::CRedEnvelope(int _money, int _mode){
	parts = NULL;
	if(false == EnvelopeSet(_money, 1, _mode)){
		EnvelopeSet(100, 1, _mode);//如果塞钱失败则相当于调用无参数时的构造函数
	}
}

CRedEnvelope::CRedEnvelope(int _money, int _size, int _mode){
	parts = NULL;
	if(false == EnvelopeSet(_money, _size, _mode)){
		EnvelopeSet(100, 1, _mode);//如果塞钱失败则相当于调用无参数时的构造函数
	}
}

CRedEnvelope::~CRedEnvelope(void){
	if(parts){//若parts不指向NULL，则parts空间未释放
		delete [] parts;//释放parts
		parts = NULL;	//将parts指向NULL，防止野指针
	}
}

bool CRedEnvelope::EnvelopeSet(int _money, int _size, int _mode){
	if (_money<= 0 ||
		_size <= 0)
		return false;
	if(_size > _money)
		return false;

	//如果有之前的红包数据，则先清理
	if(parts){
		delete [] parts;
		parts = NULL;
	}
	money = 0;
	psize = 0;
	opened = 0;
	popIdx = 0;

	//设置新红包的信息
	money = _money;
	psize = _size;
	mode = _mode;
	parts = new struct PART[psize];
	memset(parts, 0, psize * sizeof(PART));

	//设置随机数种子
	srand((unsigned int)time(0));

	return true;
}

int CRedEnvelope::OpenOne(TCHAR * user){
	if(0 == money)	//尚未塞钱进红包
		return 0;
	if(0 == psize)	//尚未划分红包
		return 0;
	int remainMoney = GetMoneyRemained();	//剩余钱数
	int remainParts = psize - opened;	//剩余份数
	int ret = 0;						//用户抢到的钱数


	if(remainParts <= 0)	//红包抢完了
		return -1;
	if(0 == mode && -1 != GetAccountIdx(user))	//若mode==0 则同一用户只能抢一份红包
		return -2;
	else if(1 == remainParts){
		ret = remainMoney;
	}
	else{
		/**********************************************************
		(注：此处对应的视频代码有问题，视频中随机数不遵循均匀分布)

		随机打开一份，所产生的钱数范围在
		1分～（剩余钱数/剩余份数 * 2 - 1）
		产生随机数：1～（剩余钱数/剩余份数 * 2 - 1）

		rand() % (remainMoney/remainParts*2 - 1)能产生
		范围在0～（剩余钱数/剩余份数 * 2 - 2）的随机数，
		加上1可得所需范围随机数，且不改变rand()的概率分布
		***********************************************************/
		ret = rand() % (remainMoney/remainParts*2 - 1) + 1;

	}

	//将此次开红包信息记录到parts[]
	parts[opened].cents = ret;
	_tcscpy_s(parts[opened].account, 64 * sizeof(TCHAR), user);

	//已打开的红包数+1
	opened ++;

	return ret;
}

int CRedEnvelope::GetMoneyRemained(void){
	int sum = 0;
	for(int i = 0; i < opened; i++){//将所有开过的红包钱数加在一起
		sum += parts[i].cents;
	}
	return money - sum;//返回剩余钱数
}

int CRedEnvelope::GetSize(void){
	return psize;
}

int CRedEnvelope::GetPartsOpened(void){
	return opened;
}
int CRedEnvelope::GetPartsRemained(void){
	return psize - opened;
}

int CRedEnvelope::GetAccountIdx(TCHAR * user){
	//在所有开过红包的用户名中寻找
	for(int i = 0; i < opened; i++){
		if(_tcscmp(user, parts[i].account) == 0)//若有相同的返回索引值i
			return i;
	}
	return -1;//若没有返回-1
}

int CRedEnvelope::GetBestIdx(void){
	int temp = 0;
	int idx = -1;
	for(int i = 0; i < opened;){
		//如果当前i索引的变量大于temp存储的变量
		//则令temp等于该值，并让idx等于当前索引值i
		//遍历一遍数组即可得到最大值对应索引idx
		if(temp < parts[i].cents){
			temp = parts[i].cents;
			idx = i;
		}
		i++;
	}
	return idx;
}

PART CRedEnvelope::PartsPop(void){
	//初始化空PART变量
	PART ret;
	memset(&ret, 0, sizeof(PART));
	
	//如果popIdx大于开启索引值(opened - 1)
	//清零popIdx并返回空PART变量
	if(popIdx >= opened){
		popIdx = 0;
		return ret;
	}
	
	//将parts[popIdx]复制到ret中
	ret = parts[popIdx];
	//索引自增
	popIdx++;

	return ret;
}
void CRedEnvelope::ClearPartsPopIdx(void){
	popIdx = 0;
}

PART CRedEnvelope::GerBestPart(void){
	//获取手气最佳索引值
	int idx = GetBestIdx();

	//若返回索引值为-1，说明还没人开过红包
	//返回空PART变量
	if(-1 == idx){
		PART ret;
		memset(&ret, 0, sizeof(ret));
		return ret;
	}
	
	//根据索引值找到手气最佳并返回
	return parts[idx];
}

