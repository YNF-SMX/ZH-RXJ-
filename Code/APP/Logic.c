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
    {  //�Զ�ģʽ ��������״̬���
        GUW.Button.StepMode = 0;
        GUW.Button.StepState = 0;
    }
    if(GUW.Button.StepMode==1&&GUW.Button.StepState ==0)
    {  //����ģʽ �Զ�����
        GUW.Button.StepState = 1;
        LogicTask.WindingTask.execute = 1;
        LogicTask.TakeLineTask.execute = 1;
    }
}

//��ͣ
void  scram_handle(void)
{
	for(int i=0;i<4;i++)
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

}

//���߻���
/**
 * ת������
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



//ץ�߷Ż���
/**
 * 1.�½�ȡ�� ��ס
 * 2.��̧���ƶ�������λ
 * 3.�½����ϣ����¼н���צ���ɿ�
 * 4.̧��
*/
void TakeLine(LogicParaDef *Task, TechParaDef *Para)
{
    INITT(Task)
    switch (STEP)
    {
    
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
    
    }
}

//�����߼�
void RunMain(LogicParaDef* Task)
{
    if(Data.WindingState == 1)
    {//���Զ�ģʽʱ ���߱�־���� ���Զ�����ȡ������
        LogicTask.TakeLineTask.execute = 1;
    }

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
        GUS.TechPara.Data.TurnClampDelay_CL = 50;		//ת̨�н���ʱ  4020
        GUS.TechPara.Data.TakeDownDelay = 20; 			//�½���ʱ	    4024
        GUS.TechPara.Data.TakeClampDelay_CL = 20;		//צ����ʱ      4026
        GUS.TechPara.Data.TakeClampDelay_OP = 50;      //צ���ſ���ʱ  4028
        GUS.TechPara.Data.BendClampDelay_CL = 50;	//���߱պ���ʱ      4030
        GUS.TechPara.Data.BendClampDelay_OP = 300;  //                  4032
        GUS.TechPara.Data.TwistedPos = 0;			//ת��Ť�߿��� 0 180ƽŤ 1 90��Ť4034
        GUS.TechPara.Data.TwistedClampDelay = 100;  //Ť�߼н���ʱ      4036
        GUS.TechPara.Data.FeedLineSpd = 50;		//�����ٶ�         4038
        GUS.TechPara.Data.Feedlinelen = 0;		//����λ��         4040
        GUS.TechPara.Data.Cir = 3;								//Ť��Ȧ��         4042
        GUS.TechPara.Data.TurnSpd = 50;		//ת̨�ٶ� 1---100%    4044
        GUS.TechPara.Data.MovelineSpd = 50;	//�������ٶ� 1--100%   4046
        GUS.TechPara.Data.TurnCir = 2;		//ת̨Ȧ��   ����С��  4048
        GUS.TechPara.Data.TwiedSpd = 50;		//Ť���ٶ�             4050
        GUS.TechPara.Data.MoveLineLen = 500;	//�������г�		4056
        GUS.TechPara.Data.TurnSpdBack = 30;	//�ذ�Ȧ���ٶ�		4058
		GUS.TechPara.Data.TwON_TM = 100;		//	4060                      ����2
		GUS.TechPara.Data.TwOFF_TM = 100;	//	4062                          ����2
		GUS.TechPara.Data.RankDelay = 600;	//	4064
        GUS.TechPara.Data.CutTapDelay = 30;	//��������ʱ	4066
        GUS.TechPara.Data.LinePressDelay_OP = 200;	// ����ѹ����ʱ       4068
        GUS.TechPara.Data.TwistedClampOFFDelay = 100;  //Ť���ɿ���ʱ      4070
        GUS.TechPara.Data.LinePressDelay_CL=0;		//����ѹ���׹���ʱ  4072
        GUS.TechPara.Data.LinePutPos=0;			//��������λ��    4074
		GUS.TechPara.Data.TakeDownDelay2 = 200;	//ȡ���½� 4076

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
            { //�������ߺ�ȡ��
                LogicTask.WindingTask.execute = 1;
                LogicTask.TakeLineTask.execute = 1;
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
                    LogicTask.TakeLineTask.execute = 1;
                }
            }
        }
    }
    /******���������߼�*******/
    if(SysFsm.curState == RUN) 
    {
        if(GUW.Button.RunMode == 0||GUW.Button.StepMode==1)
        { //�Զ�����ģʽ������ �������б�־��д1
            RunMain(&LogicTask.RunAutoTask);             
            TakeLine(&LogicTask.TakeLineTask,&GUS.TechPara);     //ȡ�߶���
            TwistedLine(&LogicTask.TwistedLineTask,&GUS.TechPara);//Ť�߶���
            Winding(&LogicTask.WindingTask,&GUS.TechPara);  //���߻���
        }
        if(GUW.Button.RunMode == 0)
        {//�Զ�ģʽ�� �ѵ���ģʽ���
            GUW.Button.StepMode = 0;
            GUW.Button.StepState = 0;
        }
        if(GUW.Button.StepState == 1 && GUW.Button.StepMode == 1)
        { //����ģʽ�� �ж����е���Ҫͣ�µ�caseʱ �������־д1 
            if(Data.StepSS == 1)
            {  //��־д1 �������еı�־д0 ͣ�ڵ�ǰcase
                GUW.Button.StepMode = 0;
            }
        }
        if(GUW.Button.TestMode)
        { //�ϻ�ģʽ�� ����Ҫ�Ƚ�̤���Զ�����
            if(Data.TakeState == 1)
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


