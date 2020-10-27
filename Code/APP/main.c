/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-06-30 15:23:27
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\main.c
 */
#include "interlayer.h"
#include "bsp_config.h"
#include "appinit.h"
#include "maintask.h"
#include "can_master.h"
#include "LockNew.h"

const uint32_t Code_Versions __attribute__((at(0x08040800))) = 0xA05;

int unlocknum;
int rigcode;
u32 DateNum;	//日期

//检查锁机状态
void Logcheckin(void)
{
    GUR.year = GUS.SaveCheck.Date%100;  //到期年
    GUR.month = GUS.SaveCheck.Date/100%100;  //到期月
    GUR.day = GUS.SaveCheck.Date/10000;     //到期日
    if(GUR.year>25)
    {
        return;
    }
    /******		日期判断	****/
    if(GUS.SaveCheck.lockflag != 0xab)	//没有解锁过
    {
        AlarmSetBit(4,0,4);//锁机状态
    }
    else
    {
        if(GSR.date.year>GUR.year||\
                (GSR.date.year==GUR.year&&GSR.date.month>GUR.month)\
                ||(GSR.date.year==GUR.year&&GSR.date.month==GUR.month&&GSR.date.day>GUR.day))
        {
            AlarmSetBit(4,0,4);  //锁机了
            GUS.SaveCheck.lockflag = 0;
            GUS.SaveCheck.Date = 0;
        }
    }

    /***********注册函数***************/
    if(GUW.Button.Login == 1)
    {
        GUR.RegistState = 0;
        GUW.Button.Login = 0;
        if(Regist(GSR.CID[1],GSR.CID[0],GSW.LockPara.decode[0],&DateNum)==0)
        {   //成功
            GUR.RegistState = 1;
            GUS.SaveCheck.lockflag = 0xab;
            GUS.SaveCheck.Date = DateNum;
            FRam_Write(0,&GUS,sizeof(GUS));
            while(FRAM_GetSta()!= 0)
            {
                FRam_Exec();
            }
        } else
        {   //失败
            GUR.RegistState = 2;
            AlarmSetBit(4,0,4);//锁机状态
        }
    }

}

Date Dset;
Time Tset;
int settime;
int main()
{
//    sys_init();
    sys_init_IAP();
    bsp_init();
    bsp_exec();

    AppInit();
    CID_init(ZYG,GSR.CID);
    while(2 != HZ_ExCanNetStateGet())
    {
        InterLayer();
        bsp_exec();
    }
    while(1)
    {
        InterLayer();
        bsp_exec();
        MainTask();
        Refresh(&GSW.LockPara.refreshId,GSR.CID);
#if LOCK
        //Logcheckin();
#endif
        if(settime==1)
        {
            settime = 0;
            RTC_Set_Date(Dset.year,Dset.month,Dset.day,Dset.week);
            RTC_Set_Time(Tset.hour,Tset.min,Tset.sec,Tset.ampm);
        }
    }
}


int pwr_Task(void)
{
    //HZ_Data_Write();
    FRam_Write(0,&GUS,sizeof(GUS));
    while(FRAM_GetSta()!= 0)
    {
        FRam_Exec();
    }
    return 0;
}

void SysTick_Handler(void)
{

}






