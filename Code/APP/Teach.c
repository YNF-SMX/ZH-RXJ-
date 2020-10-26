/*
 * @Author: your name
 * @Date: 2020-06-19 14:36:56
 * @LastEditTime: 2020-06-30 22:46:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \ZH_ZDJ\Code\APP\Teach.c
 */
#include "Teach.h"
#include "DataDefine.h"
#include "FSM.h"
#include "userTool.h"
#include "iodefine.h"

//剪线函数，手动单独调用
void Cutline(LogicParaDef *Task)
{
    
}

LogicParaDef FeedTask;
void Feed()
{
    if(FeedTask.execute==1&&FeedTask.step==0)
    {
        FeedTask.step = 1;
    }
    switch(FeedTask.step)
    {
    
    }
}

LogicParaDef WindTask;
void WindingTest(LogicParaDef* Task)
{
    INITTASK(WindTask)
    switch (WindTask.step)
    {
    case 1: //确保状态安全
        if(HZ_AxGetStatus(TRMOTOR)==0)
        {
            if(InGet(I_MlArm_Up)==ON||HZ_AxGetCurPos(MLMOTOR)==0) //在上面不在转台上
            {
                Data.WindingState = 0;
                OutSet(Q_Turn,ON);	//转台气缸夹紧
                OutSet(Q_End,OFF);
                TRST(Task);
                STEP = 2;
            }
        }
        break;
    case 2:
        if(TCNT(Task)>=GUS.TechPara.Data.TurnClampDelay_CL)
        {
            if(HZ_AxGetStatus(TRMOTOR)==0)
            {
                if(GUS.RankStop == 0)
                {
                    LogicTask.RankDoTask.execute = 1;
                }
                HZ_AxSetCurPos(TRMOTOR,0);
                STEP = 3;
                if(GUS.SendEndEn == 1)
                {
                    MotorMove(TRMOTOR,GUS.TechPara.Data.TurnSpd,(GUS.TechPara.Data.TurnCir - GUS.SendCir)*GSS.axis[TRMOTOR].Axconver.MPR,RELMODE);//转动相应圈数
                    STEP = 5;
                }
                else
                {
                    MotorMove(TRMOTOR,GUS.TechPara.Data.TurnSpd,GUS.TechPara.Data.TurnCir*GSS.axis[TRMOTOR].Axconver.MPR,RELMODE);//转动相应圈数
                }
            }
        }
        break;

    case 5:
        if(HZ_AxGetStatus(TRMOTOR)==0)
        {
            MotorMove(TRMOTOR,GUS.SendSpeed,(GUS.SendCir)*GSS.axis[TRMOTOR].Axconver.MPR,RELMODE);//转动相应圈数
            OutSet(Q_Rank,OFF);
            PARAINIT(LogicTask.RankDoTask);
            OutSet(Q_End,ON);
            STEP = 3;
        }
        break;

    case 3:
        if(HZ_AxGetStatus(TRMOTOR)==0)
        {   //转动到对应圈
            LogicTask.RankDoTask.execute= 0;
            OutSet(Q_Rank,OFF);
            TRST(Task);
            STEP = 4;
        }
        break;

    case 4:
        PARAINIT(*Task);
        break;
    }
}


void Teach()
{
    if (GUR.RunStatus == RUN)
    {
        return;
    }
    Feed();
    WindingTest(&WindTask);
    switch (GUW.TeachWord) //1520
    {
    case 1:

        break;
    case 2:
        GUS.CNT = 0;
        break;
    case 5: //报警清除
        GSW.ClearAlarm = 1;
        GUW.Button.RunCommand = STOP;
        break;
    case 6:
        FRam_Write(0,&GUS,sizeof(GUS));
        break;
    case 7:
        HZ_Data_Write();
        break;
    case 8: //参数拷贝
        break;
    /**********手动测试使用**************/
    case 10:
        //绕线转盘测试
        if(InGet(I_MlArm_Up) == ON)//需要添加满足条件
        {
            if(WindTask.execute==0)
                WindTask.execute = 1;
        }
        break;
    case 11:
        //取线动作测试
        if(1)
        {
            if(LogicTask.TakeLineTask.execute==0)
                LogicTask.TakeLineTask.execute = 1;
        }
        break;
    case 12:
        //扭线动作测试
        if(1)
        {
            if(LogicTask.TwistedLineTask.execute==0)
                LogicTask.TwistedLineTask.execute = 1;
        }
        break;

    /***********************************/
    case 20: //上移线回零
        if(InGet(I_MlArm_Up)==ON) //判断安全
        {
            MotorGoHome(MLMOTOR);
        }
        break;
    case 21: //摆线回零
        if(InGet(I_MlArm_Up)==ON) //判断安全
        {
            MotorGoHome(FLMOTOR);
        }
        break;
    case 22: //扭线回零
        MotorGoHome(TWMOTOR);
        break;
    case 23: //右移到终点
        if(InGet(I_MlArm_Up)==ON) //判断安全
        {
            MotorMove(MLMOTOR,20,GUS.TechPara.Data.MoveLineLen,ABSMODE);
        }
        break;
    case 24: //左移到原点
        if(InGet(I_MlArm_Up)==ON)//判断安全
        {
            MotorMove(MLMOTOR,20,0,ABSMODE);
        }
        break;
    case 25:   //摆线扭线位
        if(OutGet(Q_TwistedClamp)==OFF&&InGet(I_TwistMotor_Org)==ON)
        {
            MotorMove(FLMOTOR,GUS.TechPara.Data.FeedLineSpd,GUS.TechPara.Data.Feedlinelen,RELMODE);
        }
        break;
    case 26:  //转台测试
        if(InGet(I_MlArm_Up)==ON)//判断安全
        {
            MotorMove(TRMOTOR,GUS.TechPara.Data.TurnSpd,GUS.TechPara.Data.TurnCir*GSS.axis[TRMOTOR].Axconver.MPR,RELMODE);//转动相应圈数
        }
        break;
    case 27:  //扭扎带测试
        MotorMove(TWMOTOR,GUS.TechPara.Data.TwiedSpd,GUS.TechPara.Data.Cir*GSS.axis[TWMOTOR].Axconver.MPR,RELMODE);//转动相应圈数
        break;
    case 28:  //送线
        LogicTask.FeedlineTask.execute = 1;
        break;
    case 29:
        LogicTask.CutlineTask.execute = 1;
        break;
    case 30:
        FeedTask.execute = 1;
        break;
    case 31:  //下移线 前后
        if(InGet(I_MlArm_Up)==ON)
        {
            //下一线在前面
            if(OutGet(Q_TwistedWork)==ON )
            {
                if(OutGet(Q_TwistedTake)==OFF)
                {
                    OutSet(Q_TwistedWork,OFF);
                }
            } else
            {
                if(OutGet(Q_TwistedTake)==OFF)
                {
                    OutSet(Q_TwistedWork,ON);
                }
            }
        }
        break;

    default:
        break;
    }
    GUW.TeachWord = 0;
}
