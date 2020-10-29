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

/*ȫ�ֱ�����ͳһ����*/
Task LogicTask = {0}; //ͳһ��������
SysData Data = {0};   //ͳһ��������

extern void initIO(void);

void run_handle(void)
{
    if(GUW.Button.RunMode == 0)
    {   //�Զ�ģʽ ��������״̬���
        GUW.Button.StepMode = 0;
        GUW.Button.StepState = 0;
    }
    if(GUW.Button.StepMode==1&&GUW.Button.StepState ==0)
    {   //����ģʽ �Զ�����
        GUW.Button.StepState = 1;
        LogicTask.WindingTask.execute = 1;
    }
}

//��ͣ
void  scram_handle(void)
{
    for(int i=0; i<4; i++)
    {
        HZ_AxStop(i);
    }
    PARAINIT(LogicTask);
    GUR.HaveToReset = 1;
}

//���io��״̬ ȷ����̨�ڰ�ȫλ�ÿ�������
void CheckIOState(void)
{
}

//��λ����
void RankDo(LogicParaDef *Task)
{
    if(Task->execute == 0 && Task->step >0)
    {
        OutSet(Q_Rank,OFF);
        Task->execute = 0;
        STEP = 0;
    }
    INITT(Task)
    switch(STEP)
    {
    case 1:
    {
        OutSet(Q_Rank,ON);
        TRST(Task);
		if(GUS.RankStop == 0) //ѭ����λ
		{
			STEP = 4;
		}
		else if(GUS.RankStop == 2) //������λ
		{
			STEP = 6;
		}
        
    }
    break;
    case 4:
        if(TCNT(Task)>=GUS.TechPara.Data.RankDelay)
        {
            OutSet(Q_Rank,OFF);
            TRST(Task);
            STEP = 5;
        }
        break;
    case 5:
        if(TCNT(Task)>=GUS.TechPara.Data.RankDelay )
        {
            OutSet(Q_Rank,ON);
            TRST(Task);
            STEP = 4;
        }
        break;
		
	case 6:
        if(TCNT(Task)>=GUS.TechPara.Data.RankDelay)
        {
            OutSet(Q_Rank,OFF);            
        }
        break;
    }
}

//���߻���
/**
* ת������ ��������:ת����û�����ϻ������ϱ�ȡ��
*/
void Winding(LogicParaDef *Task, TechParaDef *Para)
{
    INITT(Task)
    switch (STEP)
    {
    case 1:
        if(Data.WindingState ==0&& HZ_AxGetStatus(TRMOTOR)==0)
        {
            if(InGet(I_MlArm_Up)==ON || HZ_AxGetCurPos(MLMOTOR)<200)
            {
                OutSet(Q_Turn,ON);
                OutSet(Q_End,OFF);
                TRST(Task);
                STEP = 2;
            }
        }
        break;
    case 2:
        if(TCNT(Task)>=Para->Data.TurnClampDelay_CL)
        {
            if(GUS.RankStop == 0 || GUS.RankStop == 2) //��λ����
            {
                LogicTask.RankDoTask.execute = 1;
            }
            if(LogicTask.TakeLineTask.execute == 0) //ȡ�߿���
            {
                LogicTask.TakeLineTask.execute = 1;
            }
            HZ_AxSetCurPos(TRMOTOR,0);
            MotorMove(TRMOTOR,Para->Data.TurnSpd,Para->Data.TurnCir*GSS.axis[TRMOTOR].Axconver.MPR,ABSMODE);//ת����ӦȦ��
            Data.StepSS = 0;
            STEP = 3;
        }
        break;
    case 3:
        if(HZ_AxGetStatus(TRMOTOR)==0)
        {   //ת������ӦȦ
            LogicTask.RankDoTask.execute= 0;
            OutSet(Q_Rank,OFF);
            TRST(Task);
            Data.StepSS = 1;
            STEP = 4;
        }
        break;
    case 4:
        Data.StepSS = 0;
        Data.WindingState = 1; //�������
        PARAINIT(*Task);
        break;
    }
}



//ץ�߷Ż���
/**
 * 1.�½�ȡ�� ��ס
 * 2.��̧���ƶ�������λ
 * 3.�½����ϣ����¼н���צ���ɿ�
 * 4.̧��
*/
void TakeLine(LogicParaDef *Task, TechParaDef *Para)
{
    if(Data.WindingState == 1 && Task->execute == 0)
    {
        Task->execute = 1;
    }
    INITT(Task)
    switch (STEP)
    {
    case 1:
        if(InGet(I_MlArm_Up) == ON && HZ_AxGetStatus(MLMOTOR)== 0)
        {
            OutSet(Q_TakeClamp, OFF);//��צ���ſ�
            OutSet(Q_TakeClamp2,OFF);
            MotorMove(MLMOTOR,Para->Data.MovelineSpd,Para->Data.MoveLineLen,ABSMODE);
            STEP = 2;
        }
        break;
    case 2:
        if(HZ_AxGetStatus(MLMOTOR)== 0 && HZ_AxGetStatus(TRMOTOR)==0)
        {
            if(LogicTask.WindingTask.execute == 0)
            {
                OutSet(Q_TakeArm,ON);
                TRST(Task);
                Data.StepSS = 1;	//����ͣ
                STEP = 3;
            }
        }
        break;
    case 3:
        if(InGet(I_MlArm_Dw) == ON )
        {
            OutSet(Q_TakeClamp, ON); //צ��ץ��
            OutSet(Q_TakeClamp2,ON);
            OutSet(Q_Turn,OFF);	//ת̨�����ɿ�
            TRST(Task);
            STEP = 4;
        }
        break;
    case 4:
        if (TCNT(Task) >= Para->Data.TakeClampDelay_CL)
        {
            OutSet(Q_TakeArm, OFF); //ȡ������
            TRST(Task);
            STEP = 5;
        }
        break;
    case 5:
        if(InGet(I_MlArm_Up) == ON)
        {
            Data.WindingState = 0;   //ת̨��ȡ��,�������״̬
            MotorMove(MLMOTOR,Para->Data.MovelineSpd,0,ABSMODE);
            STEP = 6;
            TRST(Task);
        }
        break;
    case 6:
        if(HZ_AxGetStatus(MLMOTOR)==0)
        {
            if(LogicTask.TwistedLineTask.execute==0)//�������Ѿ��ӳ�ȥ��
            {
                if(HZ_AxGetCurPos(TRMOTOR)%GSS.axis[TRMOTOR].Axconver.PPR)
                {
                    int pos=GSS.axis[TRMOTOR].Axconver.PPR-HZ_AxGetCurPos(TRMOTOR)%GSS.axis[TRMOTOR].Axconver.PPR;
                    MotorMove(TRMOTOR,Para->Data.TurnSpdBack,pos,RELMODE);
                }
                OutSet(Q_TakeArm, ON);
                TRST(Task);
                STEP = 7;
            }
        }
        break;
    case 7:
        if(InGet(I_MlArm_Dw) == ON && OutGet(Q_Press)==OFF)
        {
            OutSet(Q_Press,ON);
            OutSet(Q_TwistClamp,ON);
            TRST(Task);
            STEP = 8;
        }
        break;
    case 8:
        if(TCNT(Task)>= Para->Data.TwistedClampDelay)
        {
            OutSet(Q_TakeClamp,OFF);
            OutSet(Q_TakeClamp2,OFF);
			LogicTask.TwistedLineTask.execute = 1; //צ���ſ��Ϳ�ʼŤ�ߣ�����̨
            TRST(Task);
            STEP = 9;
        }
        break;
    case 9: 
        if(HZ_AxGetStatus(FLMOTOR)==0)	//�ſ�ʱ��  TCNT(Task)>= Para->Data.TakeClampDelay_OP
        {
			
			OutSet(Q_TakeArm,OFF);
			TRST(Task);
			STEP = 10;
			
            
        }
        break;
    case 10:
        if(InGet(I_MlArm_Up)==ON)
        {   //��̧��λ
           // LogicTask.TwistedLineTask.execute = 1;
            //if(LogicTask.WindingTask.execute == 1)
            {
                MotorMove(MLMOTOR,Para->Data.MovelineSpd,Para->Data.MoveLineLen,ABSMODE);
            }
            PARAINIT(*Task);
            Task->done = 1;
        }
        break;

    }
}

//Ť�߻���
/**
 * 1.�ƶ���Ť��λ��
 * 2.�½���צ�Ӽн�
 * 3.���ת��һȦ
 * 4.����צ���ɿ�
 * 5.̧��
*/
void TwistedLine(LogicParaDef *Task, TechParaDef *Para)
{
    INITT(Task)
    switch (STEP)
    {
    case 1:
        if(HZ_AxGetStatus(FLMOTOR)==0)
        {   //ŤȦȦ
            MotorMove(FLMOTOR,Para->Data.TwiedSpd, Para->Data.Cir*GSS.axis[FLMOTOR].Axconver.MPR, RELMODE);
            STEP = 2;
        }
        break;
    case 2:
        if(HZ_AxGetStatus(FLMOTOR)==0)
        {
            OutSet(Q_Press,OFF);
            TRST(Task);
            STEP = 3;
        }
        break;
    case 3:
        if(TCNT(Task)>=Para->Data.PressOFF)
        {
            OutSet(Q_TwistClamp,OFF);
            TRST(Task);
            STEP = 4;
        }
        break;
    case 4:
        if(HZ_AxGetCurPos(MLMOTOR)>200 && TCNT(Task)>=Para->Data.TwistedClampOFFDelay && InGet(I_MlArm_Up) == ON)
        {
            OutSet(Q_Out,ON);
            TRST(Task);
            STEP = 5;
        }
        break;
    case 5: 
        if(TCNT(Task)>=Para->Data.OutON)
        {
            OutSet(Q_Out,OFF);
            TRST(Task);
            STEP = 6;
        }
        break;
    case 6:
        if(TCNT(Task)>=Para->Data.OutON)
        {
            PARAINIT(*Task);
            Task->done = 1;
        }
        break;
    }
}

//�����߼�
void RunMain(LogicParaDef* Task)
{
}

//���ݳ�ʼ��
void DataReset()
{
    if(GUS.SendEndEn > 1)
    {
        GUS.SendEndEn = 0;
    }
    if(GUW.DataReset == 1)
    {

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
    //����
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
    //�����л�Ϊ�Զ��� �Զ��л�Ϊֹͣ
    if(GUW.Button.RunMode == 0 && GUW.Button.StepState != 0)
    {
        ton_et[10] = 0;
        GUW.Button.RunCommand = STOP;
        GUW.Button.StepState = 0;
    }
    //��̤����
    if(GUW.Button.FdStart == 1)
    {
        GUW.Button.FdStart = 0;
        if(GUW.Button.RunMode==0&&SysFsm.curState == RUN) //�Զ���������ģʽ
        {
            if(LogicTask.WindingTask.execute==0 && Data.WindingState == 0)  //���߿��б�־
            {   //�������ߺ�ȡ��
                LogicTask.WindingTask.execute = 1;
            }
        }
        else if(GUW.Button.RunMode == 1)  //����ģʽ
        {   //���������˻��ǵ����ſ�ʼ ��̤��������
            if((GUW.Button.StepState == 0 || GUW.Button.StepState == 2)&& SysFsm.curState == RUN)
            {
                GUW.Button.StepState = 1; //������ǰ״̬��������
                if(LogicTask.WindingTask.execute==0 && Data.WindingState == 0)  //���߿��б�־
                {
                    GUW.Button.StepMode = 1; //�������������еİ�ť
                    LogicTask.WindingTask.execute = 1;
                }
            }
        }
    }
    /******���������߼�*******/
    if(SysFsm.curState == RUN)
    {
        if(GUW.Button.RunMode == 0||GUW.Button.StepMode==1)
        {   //�Զ�����ģʽ������ �������б�־��д1
            RunMain(&LogicTask.RunAutoTask);
            TakeLine(&LogicTask.TakeLineTask,&GUS.TechPara);     //ȡ�߶���
            TwistedLine(&LogicTask.TwistedLineTask,&GUS.TechPara);//Ť�߶���
            Winding(&LogicTask.WindingTask,&GUS.TechPara);  //���߻���
        }
        if(GUW.Button.RunMode == 0)
        {   //�Զ�ģʽ�� �ѵ���ģʽ���
            GUW.Button.StepMode = 0;
            GUW.Button.StepState = 0;
        }
        if(GUW.Button.StepState == 1 && GUW.Button.StepMode == 1)
        {   //����ģʽ�� �ж����е���Ҫͣ�µ�caseʱ �������־д1
            if(Data.StepSS == 1)
            {   //��־д1 �������еı�־д0 ͣ�ڵ�ǰcase
                GUW.Button.StepMode = 0;
            }
        }
        if(GUW.Button.TestMode)
        {   //�ϻ�ģʽ�� ����Ҫ�Ƚ�̤���Զ�����
            if(Data.WindingState == 0)
            {
                GUW.Button.FdStart = 1;
            }
        }
    }
    RankDo(&LogicTask.RankDoTask);		//��������
    Cutline(&LogicTask.CutlineTask); 	//���ߣ���ʱû�õ�
    DataReset();						//������ʼ��
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
        if(ton_et[19] < 5000)//�˵�3��
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


