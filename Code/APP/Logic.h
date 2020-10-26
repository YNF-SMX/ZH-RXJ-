/*
 * @Descripttion:
 * @version:
 * @Author: yfs
 * @Date: 2020-02-24 10:31:18
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-07-01 14:02:12
 */
#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "bsplib.h"
#include "logicdata.h"

#define NORMALMODE 0 //正常模式
#define AGINGMODE 1  //老化模式
#define STEPMODE 2   //单步调试
#define AXISDELAY 5  //停轴延时
#define ENDCASE 0xff //最后一个case

//#define MOVELINELEN   7500  //上移线行程
#define BKMOVE		  5000  //鸟嘴移动一圈

typedef union
{
	u32 Para[40]; //4020-4098
	struct
	{
		u32 TurnClampDelay_CL;		//转台夹紧延时  4020
		u32 TwTakeArm_DwDelay;      //扭线下降延时  4022 机型2
		u32 TakeDownDelay; 			//下降延时	    4024
		u32 TakeClampDelay_CL;		//爪子延时      4026
		u32 TakeClampDelay_OP;      //爪子张开延时  4028
		u32 BendClampDelay_CL;	//弯线闭合延时      4030
		u32	BendClampDelay_OP;  //                  4032
		u32 TwistedPos;			// 0 180 1 90       4034 1000*
		u32 TwistedClampDelay;  //扭线夹紧延时      4036
		u32 FeedLineSpd;		//摆线速度         	4038
		s32 Feedlinelen;		//摆线位置         	4040
		u32 Cir;				//扭线圈数         	4042
		u32 TurnSpd;		//转台速度 1---100%    	4044
		u32 MovelineSpd;	//上移线速度 1--100%   	4046
		float TurnCir;		//转台圈数   存在小数  	4048
		u32 TwiedSpd;		//扭线速度             	4050
		u32 LineLen;  		//使用电机送线长度   	4052  机型2
		int LineSpd;		//使用电机送线速度   	4054  机型2
		int MoveLineLen;	//上移线行程			4056
		u32 TurnSpdBack;	//回半圈的速度			4058
		u32 TwON_TM;		//	4060                      机型2
		u32 TwOFF_TM;	//	4062                          机型2
		u32 RankDelay;	//	4064	
		u32 CutTapDelay;	//剪扎带延时	4066		
		u32 LinePressDelay_OP;	// 送线压开延时       4068
		u32 TwistedClampOFFDelay;  //扭线松开延时      4070
		u32 LinePressDelay_CL;		//送线压气缸关延时  4072
		int LinePutPos;			//摆线仍料位置    4074
		u32	TakeDownDelay2;     //剪线时下降时间 4076
		//机型3 
		u32 TakeUpDelay;      //取线上时间  4078
		u32 TakeUpDelay2;    //取线上 --剪线位置 上延时  4080
		
	}Data;
}TechParaDef;	//绕线机构参数


typedef struct //系统任务队列
{
    LogicParaDef AxisMoveTask[PULS_NUM];
	LogicParaDef WindingTask;	//绕线
	LogicParaDef TakeLineTask;	//取线
	LogicParaDef TwistedLineTask; //扭线
	LogicParaDef PutLineTask;	//放线
	LogicParaDef RunAutoTask;	//自动运行
	LogicParaDef RankDoTask;	//排线上
	LogicParaDef FeedlineTask;	//送线
	LogicParaDef CutlineTask;
} Task;
extern Task LogicTask;

extern LogicParaDef ResetBeakMotorTask;

typedef struct //系统程序自用变量
{
	int WindingState;	//线绕好 标志写1
	int TakeState;	  //线取走标志
	int StepSS;
	int UphCahe[100];
} SysData;
extern SysData Data;

typedef enum //触发器编号定义
{
    tg0,
    tg1,
    tg2,
    tg3,
    tg4,
    tg5,
    tg6,
    tg7,
    tg8,
    tg9,
    tg10,
    tg11,
} TgNum;
 

extern void Logic(void);

#endif
