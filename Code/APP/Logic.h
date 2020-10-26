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
		u32 RankDelay;				//��λ�ٶ�		4022
		u32 MovelineSpd;			//�������ٶ� 1--100%   	4024
		int MoveLineLen;			//�������г�			4026
		u32 TakeClampDelay_CL;		//ȡ��צ�ӱպ���ʱ      4028		
		u32 TakeClampDelay_OP;  //צ���ſ���ʱ  	4030
		u32 TurnSpdBack;	//�ذ�Ȧ���ٶ�			4032
		u32 TurnSpd;		//ת̨�ٶ� 1---100%    	4034
		float TurnCir;		//ת̨Ȧ��   ����С��  	4036
		u32 TwistedClampDelay; //צ�Ӻ͹���ѹ��ʱ�� 4038
		u32 Cir;			//Ť��Ȧ��         	4040
		u32 TwiedSpd;		//Ť���ٶ�             	4042
		u32 TwistedClampOFFDelay;  	//Ť���ɿ���ʱ      4044
		u32 PressOFF;		//�����ɿ�ʱ��     4046
		u32 OutON;          //��ʱ��         4048
		u32 OutOFF;		    //���Ϲر�ʱ��     4050
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
