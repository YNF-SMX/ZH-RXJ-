/*
 * @Author: your name
 * @Date: 2020-06-19 14:36:56
 * @LastEditTime: 2020-07-01 14:03:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \ZH_ZDJ\Code\APP\Logic.c
 */

#include "Logic.h"
#include "math.h"
#include "DataDefine.h"
#include "interlayer_config.h"
#include "FSM.h"
#include "string.h"
#include "USERTOOL.H"
#include "teach.h"
#include "iodefine.h"
#include "AxisMove.h"

/*全局变量做统一处理*/
Task LogicTask = {0}; //统一的任务类
SysData Data = {0};   //统一的数据类

extern void initIO(void);

void run_handle(void)
{
    if(GUW.Button.RunMode == 0)
    {  //自动模式 将单步的状态清除
        GUW.Button.StepMode = 0;
        GUW.Button.StepState = 0;
    }
    if(GUW.Button.StepMode==1&&GUW.Button.StepState ==0)
    {  //单步模式 自动运行
        GUW.Button.StepState = 1;
        LogicTask.WindingTask.execute = 1;
        LogicTask.TakeLineTask.execute = 1;
    }
}

//急停
void  scram_handle(void)
{
	for(int i=0;i<4;i++)
	{
		HZ_AxStop(i);
	}
	PARAINIT(LogicTask);
	GUR.HaveToReset = 1;
}

//检查io口状态 确定机台在安全位置可以运行
void CheckIOState(void)
{
}

//排位上下
void RankDo(LogicParaDef *Task)
{

}

//绕线机构
/**
 * 转盘绕线
*/
void Winding(LogicParaDef *Task, TechParaDef *Para)
{
    INITT(Task)
    switch (STEP)
    {
		case 1:
		break;
    }
}



//抓线放机构
/**
 * 1.下降取线 夹住
 * 2.上抬，移动到放料位
 * 3.下降放料，底下夹紧，爪子松开
 * 4.抬起
*/
void TakeLine(LogicParaDef *Task, TechParaDef *Para)
{
    INITT(Task)
    switch (STEP)
    {
    
    }
}

//扭线机构
/**
 * 1.移动到扭线位置
 * 2.下降，爪子夹紧
 * 3.电机转动一圈
 * 4.底下爪子松开
 * 5.抬起
*/
void TwistedLine(LogicParaDef *Task, TechParaDef *Para)
{
    INITT(Task)
    switch (STEP)
    {
    
    }
}

//运行逻辑
void RunMain(LogicParaDef* Task)
{
    if(Data.WindingState == 1)
    {//当自动模式时 绕线标志好了 就自动启动取线任务
        LogicTask.TakeLineTask.execute = 1;
    }

}

//数据初始化
void DataReset()
{
    if(GUS.SendEndEn > 1)
    {
        GUS.SendEndEn = 0;
    }
    if(GUW.DataReset == 1)
    {
        GUS.TechPara.Data.TurnClampDelay_CL = 50;		//转台夹紧延时  4020
        GUS.TechPara.Data.TakeDownDelay = 20; 			//下降延时	    4024
        GUS.TechPara.Data.TakeClampDelay_CL = 20;		//爪子延时      4026
        GUS.TechPara.Data.TakeClampDelay_OP = 50;      //爪子张开延时  4028
        GUS.TechPara.Data.BendClampDelay_CL = 50;	//弯线闭合延时      4030
        GUS.TechPara.Data.BendClampDelay_OP = 300;  //                  4032
        GUS.TechPara.Data.TwistedPos = 0;			//转角扭线开关 0 180平扭 1 90°扭4034
        GUS.TechPara.Data.TwistedClampDelay = 100;  //扭线夹紧延时      4036
        GUS.TechPara.Data.FeedLineSpd = 50;		//摆线速度         4038
        GUS.TechPara.Data.Feedlinelen = 0;		//摆线位置         4040
        GUS.TechPara.Data.Cir = 3;								//扭线圈数         4042
        GUS.TechPara.Data.TurnSpd = 50;		//转台速度 1---100%    4044
        GUS.TechPara.Data.MovelineSpd = 50;	//上移线速度 1--100%   4046
        GUS.TechPara.Data.TurnCir = 2;		//转台圈数   存在小数  4048
        GUS.TechPara.Data.TwiedSpd = 50;		//扭线速度             4050
        GUS.TechPara.Data.MoveLineLen = 500;	//上移线行程		4056
        GUS.TechPara.Data.TurnSpdBack = 30;	//回半圈的速度		4058
		GUS.TechPara.Data.TwON_TM = 100;		//	4060                      机型2
		GUS.TechPara.Data.TwOFF_TM = 100;	//	4062                          机型2
		GUS.TechPara.Data.RankDelay = 600;	//	4064
        GUS.TechPara.Data.CutTapDelay = 30;	//剪扎带延时	4066
        GUS.TechPara.Data.LinePressDelay_OP = 200;	// 送线压开延时       4068
        GUS.TechPara.Data.TwistedClampOFFDelay = 100;  //扭线松开延时      4070
        GUS.TechPara.Data.LinePressDelay_CL=0;		//送线压气缸关延时  4072
        GUS.TechPara.Data.LinePutPos=0;			//摆线仍料位置    4074
		GUS.TechPara.Data.TakeDownDelay2 = 200;	//取线下降 4076

        GUS.spd[0].acctime = 100;
        GUS.spd[0].dectime = 100;
        GUS.spd[0].endspeed = 200;
        GUS.spd[0].startspeed = 200;
        GUS.spd[0].runspeed = 10000;
        GUS.spd[1].acctime = 100;
        GUS.spd[1].dectime = 100;
        GUS.spd[1].endspeed = 200;
        GUS.spd[1].startspeed = 200;
        GUS.spd[1].runspeed = 5000;
        GUS.spd[2].acctime = 120;
        GUS.spd[2].dectime = 130;
        GUS.spd[2].endspeed = 200;
        GUS.spd[2].startspeed = 200;
        GUS.spd[2].runspeed = 10000;
        GUS.spd[3].acctime = 100;
        GUS.spd[3].dectime = 100;
        GUS.spd[3].endspeed = 200;
        GUS.spd[3].startspeed = 200;
        GUS.spd[3].runspeed = 10000;
        GUS.spd[4].acctime = 100;
        GUS.spd[4].dectime = 100;
        GUS.spd[4].endspeed = 200;
        GUS.spd[4].startspeed = 200;
        GUS.spd[4].runspeed = 3000;
        GUS.HomeOffset[0] = 0;
        GUS.HomeOffset[1] = 0;
        GUS.HomeOffset[2] = 0;
        GUS.HomeOffset[3] = 0;
        GUS.HomeOffset[4] = 0;

        GUS.LineCnt = 0;
        GUS.CNT = 0;
        GUS.IfAutoRst = 0;

        GUW.DataReset = 0;
    }
}


s32 Product(u32 Sum,u32 num);
LogicParaDef ResetBeakMotorTask;
void Logic()
{
	//按键
    if (Triger(1, ON, InGet(I_Start), 3))
    {
        GUW.Button.FdStart = 1;
    }
    if(Triger(2,OFF,InGet(I_Scram), 5))
    {
        GUW.Button.RunCommand = SCRAM;
    }
    if(ton(3,InGet(I_Scram) == OFF,10))
    {
        GUW.Button.RunCommand = SCRAM;
    }
    if(Triger(3,ON,InGet(I_Scram), 10))
    {
        GUW.Button.RunCommand = D_RESET;
    }
    //单步切换为自动是 自动切换为停止
    if(GUW.Button.RunMode == 0 && GUW.Button.StepState != 0)
    {
        ton_et[10] = 0;
        GUW.Button.RunCommand = STOP;
        GUW.Button.StepState = 0;
    }
	//脚踏启动
    if(GUW.Button.FdStart == 1)
    {
        GUW.Button.FdStart = 0;
        if(GUW.Button.RunMode==0&&SysFsm.curState == RUN) //自动连续运行模式
        {
            if(LogicTask.WindingTask.execute==0 && Data.WindingState == 0)  //绕线空闲标志
            { //启动绕线和取线
                LogicTask.WindingTask.execute = 1;
                LogicTask.TakeLineTask.execute = 1;
            }
        }
        else if(GUW.Button.RunMode == 1)  //单步模式
        {   //单步走完了或是单步才开始 脚踏才起作用 
            if((GUW.Button.StepState == 0 || GUW.Button.StepState == 2)&& SysFsm.curState == RUN)
            {   
                GUW.Button.StepState = 1; //单步当前状态是运行中 
                if(LogicTask.WindingTask.execute==0 && Data.WindingState == 0)  //绕线空闲标志
                {
                    GUW.Button.StepMode = 1; //触摸屏单步运行的按钮
                    LogicTask.WindingTask.execute = 1;
                    LogicTask.TakeLineTask.execute = 1;
                }
            }
        }
    }
    /******工程运行逻辑*******/
    if(SysFsm.curState == RUN) 
    {
        if(GUW.Button.RunMode == 0||GUW.Button.StepMode==1)
        { //自动运行模式或者是 单步运行标志被写1
            RunMain(&LogicTask.RunAutoTask);             
            TakeLine(&LogicTask.TakeLineTask,&GUS.TechPara);     //取线动作
            TwistedLine(&LogicTask.TwistedLineTask,&GUS.TechPara);//扭线动作
            Winding(&LogicTask.WindingTask,&GUS.TechPara);  //绕线机构
        }
        if(GUW.Button.RunMode == 0)
        {//自动模式下 把单步模式清空
            GUW.Button.StepMode = 0;
            GUW.Button.StepState = 0;
        }
        if(GUW.Button.StepState == 1 && GUW.Button.StepMode == 1)
        { //单步模式下 判断运行到需要停下的case时 将这个标志写1 
            if(Data.StepSS == 1)
            {  //标志写1 单步运行的标志写0 停在当前case
                GUW.Button.StepMode = 0;
            }
        }
        if(GUW.Button.TestMode)
        { //老化模式下 不需要踩脚踏就自动运行
            if(Data.TakeState == 1)
            {
                GUW.Button.FdStart = 1;
            }
        }
    }
    RankDo(&LogicTask.RankDoTask);		//排线上下
    Cutline(&LogicTask.CutlineTask); 	//剪线，暂时没用到
    DataReset();						//参数初始化
    GUW.UPH = Product(GUS.CNT,1);
}

s32 Product(u32 Sum,u32 num)
{
    static int i = 0;
    static int j = 0;
    static int LastCurrNum = 0;
    static int RealNum = 0;
    static int SumTime = 0;
    static int UPH = 0;
    if(num >99)
    {
        num = 99;
    }
    if(i > num)
    {
        i = 0;
    }
    if(LastCurrNum != Sum)
    {
        LastCurrNum  = Sum;
        if(ton_et[19] < 5000)//滤掉3秒
        {
            Data.UphCahe[i] = ton_et[19];
            i++;
        }
        ton_et[19] = 0;
    }

    SumTime = 0;
    RealNum = 0;
    for(j=0; j<100; j++)
    {
        if(Data.UphCahe[j] > 0)
        {
            RealNum++;
            SumTime = SumTime + Data.UphCahe[j];
        }
    }
    UPH = 3600000/(SumTime/RealNum);
    if(UPH > 20000)
    {
        UPH = 0;
    }
    return UPH;
}


