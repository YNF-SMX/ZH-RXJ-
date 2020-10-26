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

#define NORMALMODE 0 //����ģʽ
#define AGINGMODE 1  //�ϻ�ģʽ
#define STEPMODE 2   //��������
#define AXISDELAY 5  //ͣ����ʱ
#define ENDCASE 0xff //���һ��case

//#define MOVELINELEN   7500  //�������г�
#define BKMOVE		  5000  //�����ƶ�һȦ

typedef union
{
	u32 Para[40]; //4020-4098
	struct
	{
		u32 TurnClampDelay_CL;		//ת̨�н���ʱ  4020
		u32 TwTakeArm_DwDelay;      //Ť���½���ʱ  4022 ����2
		u32 TakeDownDelay; 			//�½���ʱ	    4024
		u32 TakeClampDelay_CL;		//צ����ʱ      4026
		u32 TakeClampDelay_OP;      //צ���ſ���ʱ  4028
		u32 BendClampDelay_CL;	//���߱պ���ʱ      4030
		u32	BendClampDelay_OP;  //                  4032
		u32 TwistedPos;			// 0 180 1 90       4034 1000*
		u32 TwistedClampDelay;  //Ť�߼н���ʱ      4036
		u32 FeedLineSpd;		//�����ٶ�         	4038
		s32 Feedlinelen;		//����λ��         	4040
		u32 Cir;				//Ť��Ȧ��         	4042
		u32 TurnSpd;		//ת̨�ٶ� 1---100%    	4044
		u32 MovelineSpd;	//�������ٶ� 1--100%   	4046
		float TurnCir;		//ת̨Ȧ��   ����С��  	4048
		u32 TwiedSpd;		//Ť���ٶ�             	4050
		u32 LineLen;  		//ʹ�õ�����߳���   	4052  ����2
		int LineSpd;		//ʹ�õ�������ٶ�   	4054  ����2
		int MoveLineLen;	//�������г�			4056
		u32 TurnSpdBack;	//�ذ�Ȧ���ٶ�			4058
		u32 TwON_TM;		//	4060                      ����2
		u32 TwOFF_TM;	//	4062                          ����2
		u32 RankDelay;	//	4064	
		u32 CutTapDelay;	//��������ʱ	4066		
		u32 LinePressDelay_OP;	// ����ѹ����ʱ       4068
		u32 TwistedClampOFFDelay;  //Ť���ɿ���ʱ      4070
		u32 LinePressDelay_CL;		//����ѹ���׹���ʱ  4072
		int LinePutPos;			//��������λ��    4074
		u32	TakeDownDelay2;     //����ʱ�½�ʱ�� 4076
		//����3 
		u32 TakeUpDelay;      //ȡ����ʱ��  4078
		u32 TakeUpDelay2;    //ȡ���� --����λ�� ����ʱ  4080
		
	}Data;
}TechParaDef;	//���߻�������


typedef struct //ϵͳ�������
{
    LogicParaDef AxisMoveTask[PULS_NUM];
	LogicParaDef WindingTask;	//����
	LogicParaDef TakeLineTask;	//ȡ��
	LogicParaDef TwistedLineTask; //Ť��
	LogicParaDef PutLineTask;	//����
	LogicParaDef RunAutoTask;	//�Զ�����
	LogicParaDef RankDoTask;	//������
	LogicParaDef FeedlineTask;	//����
	LogicParaDef CutlineTask;
} Task;
extern Task LogicTask;

extern LogicParaDef ResetBeakMotorTask;

typedef struct //ϵͳ�������ñ���
{
	int WindingState;	//���ƺ� ��־д1
	int TakeState;	  //��ȡ�߱�־
	int StepSS;
	int UphCahe[100];
} SysData;
extern SysData Data;

typedef enum //��������Ŷ���
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
