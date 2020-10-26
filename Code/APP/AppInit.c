/*
 * @Descripttion:
 * @version:
 * @Author: yfs
 * @Date: 2019-12-27 09:17:32
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-07-01 11:10:36
 */

#include "AppInit.h"
#include "Algorithm.h"
#include "can_master.h"
#include "UserTool.h"
#include "stdio.h"
#include "iodefine.h"

//变量声明  所有app程序的初始变量实例及声明
GlobalDataDef GlobalData = {0}; //定义用户通讯数据库
u8 uart_sendbuf[2][300];		//定义通讯接收和发送缓存
u8 uart_recvbuf[2][300];
u8 eth_txbuf[300];
u8 eth_rxbuf[300];

/****************项目使用结构的实例或声明***************/


/****************end define***************/

/**
 * @author: yfs
 * @Date: 2020-01-14 16:50:27
 * @description: 板卡初次上电flash全-1数据需要初始化
 * @para:
 * @return:
 */
void DataInit()
{
    /*
		此处添加初始化存储区数据，保证初次上电没使用的数据安全
    */
	for(int i=0;i<5;i++)
	{
		GSS.axis[i].Axconver.MPR = 1;
		GSS.axis[i].Axconver.PPR = 1;
		GSS.axis[i].Axhomecfg.homemode = 0;
		GSS.axis[i].Axhomecfg.orgnum = i;
		GSS.axis[i].Axhomecfg.orglev = ON;
		GSS.axis[i].Axhomecfg.homespeedfast = 800;
		GSS.axis[i].Axhomecfg.homespeedslow = 100;
	}

	GSS.axis[MLMOTOR].Axlimitcfg.alarmmode = ON;
	GSS.axis[FLMOTOR].Axlimitcfg.alarmmode = ON;
	GSS.axis[TRMOTOR].Axlimitcfg.alarmmode = ON;
	GSS.axis[MLMOTOR].Axconver.MPR = 4000;
	GSS.axis[MLMOTOR].Axconver.PPR = 4000;
	GSS.axis[TRMOTOR].Axconver.MPR = 1000;
	GSS.axis[TRMOTOR].Axconver.PPR = 1000;
	GSS.axis[FLMOTOR].Axconver.MPR = 1000;
	GSS.axis[FLMOTOR].Axconver.PPR = 1000;
	
	if(GUS.spd[0].acctime == 0||GUS.spd[0].acctime>500)
	{
		for(int i=0;i<5;i++)
		{
			GUS.spd[i].acctime = 100;
			GUS.spd[i].dectime = 100;
			GUS.spd[i].endspeed = 200;
			GUS.spd[i].startspeed = 200;
			GUS.spd[i].runspeed = 50000;
		}
		GUW.DataReset = 1;
	}
}

void initIO(void)
{
    int i;
    for (i = 0; i < 24; i++) //初始化 输出口
        OutPut_SetSta(i, OFF);
    for (i = 0; i < 32; i++)
        HZ_ExOutPutSet(1, i, OFF);
    for (i = 0; i < PULS_NUM; i++) //轴口使能,电平和普通输出相反
        EN_SetSta(i, 0);

}

void AppInit()
{
    //配置地址表完成modbus
    init_config(&GlobalData, GSR.ErrorCode, &GlobalData.SpaceType.Save, GLOBALDATALEN, USERWRITELEN, USERREADLEN, USERSAVELEN);
    //数据读取
	FRam_Read(0,&GUS,sizeof(GUS));
	while(FRAM_GetSta()!= 0)
    {
        FRam_Exec();
    }
	DataInit();
	for(int i=0;i<5;i++)
	{
		GSS.axis[i].AxSpd = GUS.spd[i];
	}
	
    AxisConfig(GSS.axis);	//配置轴原点和限位
    initIO();
    GSW.JogMode = 2;
    GSR.HardWare_Ver[0] = 2019724;
#if LOCK 
	GSR.SoftWare_Ver[0] = 4020100;  //统一版本  106 改捆线原点感应报警延时 107加送线电机判断
#else
	GSR.SoftWare_Ver[0] = 300104;  //统一版本
#endif
	//char softstr[40] = {0};
    //sprintf(softstr, "yd_%s", __DATE__);
    //memcpy(GSR.SoftWare_Ver, softstr, 40);
    
    //初始化状态机，将设备状态初始是错误停
    InitFsm(&SysFsm);
	
	if(GUS.IfAutoRst==1)
	{
		GUR.HaveToReset = 1;
		GUW.Button.RunCommand = D_RESET;
	}
	else
	{
		GUR.HaveToReset = 1;
	}
}
