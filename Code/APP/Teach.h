/*
 * @Author: your name
 * @Date: 2020-06-19 14:36:56
 * @LastEditTime: 2020-06-30 22:45:23
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \ZH_ZDJ\Code\APP\Teach.h
 */ 
#ifndef _TEACH_H_
#define	_TEACH_H_

#include "Logic.h"

extern void Teach(void);
extern void MoveTar_exc(void);
extern s32 GetAxState(u8 ax);
extern void MoveToTarInput(u8 ax, s32 pos,u32 spd, u8 card, u8 num);
extern void Cutline(LogicParaDef *Task);
#endif
