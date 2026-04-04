#pragma once

#include <tchar.h>

struct PART{
	int cents;			//抢到的钱数
	TCHAR account[20];	//抢红包的用户名
};

class CRedEnvelope{
public:
	//塞钱进红包，无参数默认塞1元红包分一份
	CRedEnvelope(void);

	//塞钱进红包，_money为塞钱数，默认分一份，
	//若_money<=0则默认变为100
	CRedEnvelope(int _money);

	//塞钱进红包，_money为塞钱数，_size为份数，
	//若参数不合法默认塞1元分一份
	CRedEnvelope(int _money, int _size);

	~CRedEnvelope(void);

	/****************************************
	塞钱进红包
	参数_money：总钱数，注意单位为分
	参数_size：红包总份数
	返回值true：成功
	返回值false：失败
	*****************************************/
	bool EnvelopeSet(int _money, int _size);

	/*********************************************************************
	抢一次红包
	参数user：用户名字符串首地址，类型为TCHAR
	参数mode：0为每个相同的用户名只能抢一次，非0为不限制用户抢红包次数
	返回本次抢红包获得的钱数,单位为分
	若返回0，则尚未塞钱进红包或尚未划分红包，红包存在内部错误
	若返回-1，则红包已被抢完
	若mode==0（mode默认为0），则可返回-2，说明该用户已经抢过了
	**********************************************************************/
	int OpenOne(TCHAR * user, int mode = 0);

	//返回红包中未抢到的钱数(单位：分)
	int GetMoneyRemained(void);

	//返回红包总份数
	int GetSize(void);

	//返回红包开过的份数
	int GetPartsOpened(void);
	
	//返回红包剩下的份数
	int GetPartsRemained(void);

	//检查已抢过红包列表里是否有与user相同用户名的项，
	//若有则返回私有数组parts中首个相同项的索引值,若没有返回-1
	int GetAccountIdx(TCHAR * user);

	//在parts中寻找钱数最多的项，返回它的索引值
	//若还没人开过红包，则返回-1
	int GetBestIdx(void);

	//每次调用PartsPop函数会按队列方式依次返回PART类型的变量用于外部读取parts保存的信息
	//若开红包信息（不包含没开过的那些份）全部返回完毕再调用则返回空PART变量并将popIdx清零
	PART PartsPop(void);
	
	//将popIdx清零，以便重新使用PartsPop函数
	void ClearPartsPopIdx(void);

	//返回parts中手气最佳（钱最多）的PART变量
	//若还没人开过红包则返回空PART变量
	PART GerBestPart(void);

private:
	//红包总钱数
	int money;
	//红包被拆分的份数
	int psize;
	//红包已经被抢走的份数
	int opened;	
	//动态数组首地址，保存红包各份信息，包括钱数和用户名
	//下标由0到(size-1)
	PART * parts;
	//每次调用PartsPop函数会按队列方式依次返回PART类型的变量用于外部读取parts保存的信息
	//popIdx为PartsPop函数的索引
	int popIdx;
};
