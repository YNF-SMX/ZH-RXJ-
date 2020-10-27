/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-07-01 11:45:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\iodefine.h
 */
#ifndef __IODEFINE_H
#define __IODEFINE_H
typedef enum //输出口定义
{
    X1,
    X2,
    X3,
    X4,
    X5,
    X6,
    X7,
    X8,
    X9,
    X10,
    X11,
    X12,
    X13,
    X14,
    X15,
    X16,
    X17,
    X18,
    X19,
    X20,
} OutNum;

typedef enum //输入口定义
{
    Y1,
    Y2,
    Y3,
    Y4,
    Y5,
    Y6,
    Y7,
    Y8,
    Y9,
    Y10,
    Y11,
    Y12,
    Y13,
    Y14,
    Y15,
    Y16,
} InPutNum;

typedef enum
{
    EX1,
    EX2,
    EX3,
    EX4,
    EX5,
    EX6,
    EX7,
    EX8,
    EX9,
    EX10,
    EX11,
    EX12,
    EX13,
    EX14,
    EX15,
    EX16,
    EX17,
    EX18,
    EX19,
    EX20,
    EX21,
    EX22,
    EX23,
    EX24,
    EX25,
    EX26,
    EX27,
    EX28,
    EX29,
    EX30,
    EX31,
    EX32
} ExInPutNum;
typedef enum
{
    EY1,
    EY2,
    EY3,
    EY4,
    EY5,
    EY6,
    EY7,
    EY8,
    EY9,
    EY10,
    EY11,
    EY12,
    EY13,
    EY14,
    EY15,
    EY16,
    EY17,
    EY18,
    EY19,
    EY20,
    EY21,
    EY22,
    EY23,
    EY24,
    EY25,
    EY26,
    EY27,
    EY28,
    EY29,
    EY30,
    EY31,
    EY32
} ExOutPutNum;
#define CARD1 0
#define CARD2 1
#define CARD3 2


/*********************绕线机*************************/
/*			输入口定义		*/
#define I_MLM_Org			CARD1,X1	//上移线原点
#define I_FLM_Org	    	CARD1,X2    //转线原点
#define I_Turn_Org			CARD1,X3	//转台原点
//#define I_TwistMotor_Org	CARD1,X4	//扭轧带原点

#define I_Start				CARD1,X6	//启动
#define I_Scram   			CARD1,X7	//急停

#define I_MlArm_Up			CARD1,X8	//上抓料
#define I_MlArm_Dw			CARD1,X9    // 下抓料


/*			输出口定义		*/
#define Q_TakeArm		CARD1,Y1	//出线下降
#define Q_TakeClamp     CARD1,Y2	//取线爪
#define Q_Rank			CARD1,Y3    //排位气缸
#define Q_TwistClamp	CARD1,Y4	//双夹气缸
#define Q_Press			CARD1,Y5	//压滚
#define Q_Out			CARD1,Y6	//出料
#define Q_TakeClamp2	CARD1,Y7	//取线小夹子

#define Q_Turn			CARD1,Y8	//转台+夹气缸
//预留
#define Q_End			CARD1,Y9	//送尾端



#endif
