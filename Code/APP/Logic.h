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
		u32 RankDelay;				//排位速度		4022
		u32 MovelineSpd;			//上移线速度 1--100%   	4024
		int MoveLineLen;			//上移线行程			4026
		u32 TakeClampDelay_CL;		//取线爪子闭合延时      4028		
		u32 TakeClampDelay_OP;  //爪子张开延时  	4030
		u32 TurnSpdBack;	//回半圈的速度			4032
		u32 TurnSpd;		//转台速度 1---100%    	4034
		float TurnCir;		//转台圈数   存在小数  	4036
		u32 TwistedClampDelay; //爪子和滚轮压紧时间 4038
		u32 Cir;			//扭线圈数         	4040
		u32 TwiedSpd;		//扭线速度             	4042
		u32 TwistedClampOFFDelay;  	//扭线松开延时      4044
		u32 PressOFF;		//滚轮松开时间     4046
		u32 OutON;          //打开时间         4048
		u32 OutOFF;		    //出料关闭时间     4050
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
