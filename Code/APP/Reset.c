/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-06-04 18:41:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Reset.c
 */
#include "reset.h"
#include "usertool.h"
#include "iodefine.h"
#include "logic.h"
LogicParaDef ResetTask;

void reset_handle(void)
{
    REST_TASK.execute = 1;
}

LogicParaDef MtRst[5];

void MotorGoHome(u8 ax)
{
    if(MtRst[ax].execute==0&&HZ_AxGetStatus(ax)==0)
        MtRst[ax].execute = 1;
}

void ResetMotor(void)
{
    for(int i=0; i<5; i++)
    {
        if(MtRst[i].execute==1&&MtRst[i].step==0)
        {
            MtRst[i].step=1;
            MtRst[i].err = 0;
        }
        switch(MtRst[i].step)
        {
        case 1:
            if(HZ_AxGetStatus(i)==0)
            {
                HZ_AxHome(i);
                TimerRst(MtRst[i]);
                MtRst[i].step = 2;
            }
            break;
        case 2:
            if(HZ_AxGetStatus(i)==0)
            {
//                HZ_AxSetCurve(i,200,100,1000,100,200,0);
//                HZ_AxMoveRel(i,-60);
//                TimerRst(MtRst[i]);
//                MtRst[i].step = 3;
                MtRst[i].step = 4;
            }
            else if(TimerCnt(MtRst[i])>=20000)
            {
                PARAINIT(MtRst[i]);
                MtRst[i].err = 1;
            }
            break;

        case 3:
            if(InPut_GetSta(GSS.axis[i].Axhomecfg.orgnum)==GSS.axis[i].Axhomecfg.orglev)
            {
                HZ_AxStop(i);
                MtRst[i].step=4;
            } else if(HZ_AxGetStatus(i)==0)
            {
                PARAINIT(MtRst[i]);
                MtRst[i].err = 1;
            }
//            if(i == 2)
//            {
//                HZ_AxStop(i);
//                MtRst[i].step=4;
//            }
            break;

        case 4:
            if(HZ_AxGetStatus(i)==0)
            {
                if(GUS.HomeOffset[i] != 0)
                {
                    HZ_AxSetCurve(i,200,100,2000,100,200,0);

                    if(GUS.HomeOffset[0] < -180) //上移线偏移保护
                    {
                        GUS.HomeOffset[0] = -180;
                    }
                    HZ_AxMoveRel(i,GUS.HomeOffset[i]);
                }
                MtRst[i].step = 5;
            }
            break;
        case 5:
            if(HZ_AxGetStatus(i)==0)
            {
                HZ_AxSetCurPos(i,0);
                PARAINIT(MtRst[i]);
            }
            break;
        }
    }
}

extern void initIO(void);

//设备复位函数
void Reset()
{
    if(GUR.RunStatus == D_RESET)
    {
        if(REST_TASK.execute == 1&&REST_TASK.step == 0)
        {
            REST_TASK.step = 1;
        }
        switch(REST_TASK.step)
        {
        case 1:
            GUW.Button.StepState = 0;
            GUW.Button.StepMode = 0;
            PARAINIT(LogicTask);
            PARAINIT(Data);
            initIO();
            TimerRst(REST_TASK);
            REST_TASK.step = 2;
            break;
        case 2:
            if(InGet(I_MlArm_Up)==ON && TimerCnt(REST_TASK)>= 500)
            {
                MotorGoHome(MLMOTOR);
                MotorGoHome(FLMOTOR);
                //MotorGoHome(TRMOTOR);
                REST_TASK.step = 0xf0;
            }
            else if(TimerCnt(REST_TASK)>=5000)
            {
                if(InGet(I_MlArm_Up)==OFF)
                {
                    AlarmSetBit(2,0,0);//抓排料上感应异常
                }
                PARAINIT(REST_TASK);
                GUW.Button.RunCommand = ERRSTOP;
            }
            break;
			
		case 0xf0:
				if(InGet(I_MlArm_Up)==ON && (HZ_AxGetCurPos(MLMOTOR)<=1800)) //机构冲突，上移线回到安全位置，转盘再复位，机构改好可去掉
				{
					MotorGoHome(TRMOTOR);
					REST_TASK.step = 3;
				}
			break;
        case 3:
            if( MtRst[MLMOTOR].execute==0 && MtRst[FLMOTOR].execute == 0 && MtRst[TRMOTOR].execute== 0)
            {
                //MotorGoHome(FLMOTOR);
                //OutSet(Q_CutTape,ON);
                TimerRst(REST_TASK);
                REST_TASK.step = 4;
            }
            break;
        case 4:
            if(TimerCnt(REST_TASK)>1000)
            {
                //OutSet(Q_CutTape,OFF);
                REST_TASK.step = 5;
            }
            break;
        case 5:
            if(MtRst[FLMOTOR].execute==0)
            {
                PARAINIT(REST_TASK);
                GSW.ClearAlarm = 1;
                GUR.HaveToReset = 0;
                GUW.Button.RunCommand = STOP;
            }
            break;
        }
    }
    else
    {
        PARAINIT(REST_TASK);
    }
}



