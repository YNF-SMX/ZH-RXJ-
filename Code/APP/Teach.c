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

//���ߺ������ֶ���������
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

LogicParaDef TwistTask;
void TwistTest(LogicParaDef* Task, TechParaDef *Para) //Ť�߲���
{
    INITT(Task)
    switch (STEP)
    {
    case 1:
        if(HZ_AxGetStatus(FLMOTOR)==0)
        {
            if(HZ_AxGetCurPos(FLMOTOR)%GSS.axis[FLMOTOR].Axconver.PPR == 0)
            {
                if(OutGet(Q_TwistClamp) == OFF)
                {
                    OutSet(Q_TwistClamp,ON);
                }
            }

            //ŤȦȦ
            MotorMove(FLMOTOR,Para->Data.TwiedSpd, Para->Data.Cir*GSS.axis[FLMOTOR].Axconver.MPR, RELMODE);
            STEP = 2;
        }
        break;
    case 2:
        if(HZ_AxGetStatus(FLMOTOR)==0)
        {
            //OutSet(Q_Press,OFF);
            TRST(Task);
            STEP = 3;
        }
        break;
    case 3:
        if(TCNT(Task)>=Para->Data.PressOFF)
        {
            OutSet(Q_TwistClamp,OFF);
            TRST(Task);
            STEP = 6;
        }
        break;

    case 6:
        if(TCNT(Task)>=Para->Data.OutON)
        {
            PARAINIT(*Task);
        }
        break;
    }
}







LogicParaDef WindTask;
void WindingTest(LogicParaDef* Task)
{
    INITTASK(WindTask)
    switch (WindTask.step)
    {
    case 1: //ȷ��״̬��ȫ
        if(HZ_AxGetStatus(TRMOTOR)==0)
        {
            if(InGet(I_MlArm_Up)==ON||HZ_AxGetCurPos(MLMOTOR)==0) //�����治��ת̨��
            {
                Data.WindingState = 0;
                OutSet(Q_Turn,ON);	//ת̨���׼н�
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
                if(GUS.RankStop == 0 || GUS.RankStop == 2)
                {
                    LogicTask.RankDoTask.execute = 1;
                }
                HZ_AxSetCurPos(TRMOTOR,0);
                STEP = 3;
                if(GUS.SendEndEn == 1)
                {
                    MotorMove(TRMOTOR,GUS.TechPara.Data.TurnSpd,(GUS.TechPara.Data.TurnCir - GUS.SendCir)*GSS.axis[TRMOTOR].Axconver.MPR,RELMODE);//ת����ӦȦ��
                    STEP = 5;
                }
                else
                {
                    MotorMove(TRMOTOR,GUS.TechPara.Data.TurnSpd,GUS.TechPara.Data.TurnCir*GSS.axis[TRMOTOR].Axconver.MPR,RELMODE);//ת����ӦȦ��
                }
            }
        }
        break;

    case 5:
        if(HZ_AxGetStatus(TRMOTOR)==0)
        {
            MotorMove(TRMOTOR,GUS.SendSpeed,(GUS.SendCir)*GSS.axis[TRMOTOR].Axconver.MPR,RELMODE);//ת����ӦȦ��
            OutSet(Q_Rank,OFF);
            PARAINIT(LogicTask.RankDoTask);
            OutSet(Q_End,ON);
            STEP = 3;
        }
        break;

    case 3:
        if(HZ_AxGetStatus(TRMOTOR)==0)
        {   //ת������ӦȦ
            LogicTask.RankDoTask.execute= 0;
            OutSet(Q_Turn,OFF);	//ת̨�����ɿ�
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
    TwistTest(&TwistTask, &GUS.TechPara);
    switch (GUW.TeachWord) //1520
    {
    case 1:

        break;
    case 2:
        GUS.CNT = 0;
        break;
    case 5: //�������
        GSW.ClearAlarm = 1;
        GUW.Button.RunCommand = STOP;
        break;
    case 6:
        FRam_Write(0,&GUS,sizeof(GUS));
        break;
    case 7:
        HZ_Data_Write();
        break;
    case 8: //��������
        break;
    /**********�ֶ�����ʹ��**************/
    case 10:
        //����ת�̲���
        if(InGet(I_MlArm_Up) == ON)//��Ҫ�����������
        {
            if(WindTask.execute==0)
                WindTask.execute = 1;
        }
        break;
    case 11:
        //ȡ�߶�������
        if(1)
        {
            if(LogicTask.TakeLineTask.execute==0)
                LogicTask.TakeLineTask.execute = 1;
        }
        break;
    case 12:
        //Ť�߶�������
        if(1)
        {
            if(TwistTask.execute==0)
                TwistTask.execute = 1;
        }
        break;

    /***********************************/
    case 20: //�����߻���
        if(InGet(I_MlArm_Up)==ON) //�жϰ�ȫ
        {
            MotorGoHome(MLMOTOR);
        }
        break;
    case 21: //
        if(InGet(I_MlArm_Up)==ON) //�жϰ�ȫ
        {
            MotorGoHome(FLMOTOR);
        }
        break;
    case 22: //
        if(InGet(I_MlArm_Up)==ON) //�жϰ�ȫ
        {
            MotorGoHome(TRMOTOR);
        }
        break;
    case 23: //���Ƶ��յ�
        if(InGet(I_MlArm_Up)==ON) //�жϰ�ȫ
        {
            MotorMove(MLMOTOR,20,GUS.TechPara.Data.MoveLineLen,ABSMODE);
        }
        break;
    case 24: //���Ƶ�ԭ��
        if(InGet(I_MlArm_Up)==ON)//�жϰ�ȫ
        {
            MotorMove(MLMOTOR,20,0,ABSMODE);
        }
        break;

    case 26:  //ת̨����
        if(InGet(I_MlArm_Up)==ON)//�жϰ�ȫ
        {
            MotorMove(TRMOTOR,GUS.TechPara.Data.TurnSpd,GUS.TechPara.Data.TurnCir*GSS.axis[TRMOTOR].Axconver.MPR,RELMODE);//ת����ӦȦ��
        }
        break;
    case 27:  //Ť��һȦ
        MotorMove(FLMOTOR,GUS.TechPara.Data.TwiedSpd,GSS.axis[FLMOTOR].Axconver.MPR,RELMODE);
        break;

    case 28:  //

        break;
    case 29:
        LogicTask.CutlineTask.execute = 1;
        break;
    case 30:
        FeedTask.execute = 1;
        break;

    case 40:  //����
        if(OutGet(Q_TakeArm)==ON)
        {
            OutSet(Q_TakeArm,OFF);
        } else
        {
            OutSet(Q_TakeArm,ON);
        }

        break;
    case 41: //˫��
        if(HZ_AxGetCurPos(FLMOTOR)%GSS.axis[FLMOTOR].Axconver.PPR == 0)
        {
            if(OutGet(Q_TwistClamp)==ON)
            {
                OutSet(Q_TwistClamp,OFF);
            } else
            {
                OutSet(Q_TwistClamp,ON);
            }
        }
        break;
    case 42:
        if(InGet(I_MlArm_Up)==ON)//�жϰ�ȫ
        {
            if(OutGet(Q_Out)==ON)
            {
                OutSet(Q_Out,OFF);
            } else
            {
                OutSet(Q_Out,ON);
            }
        }
        break;

    default:
        break;
    }
    GUW.TeachWord = 0;
}
