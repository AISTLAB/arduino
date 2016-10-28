/*
通用按键模板
让单片机按键处理同windows软件一样简单。
winxos 2013-3-22
*/

#ifndef KEY_MOUDEL_H
#define KEY_MOUDEL_H
#include "Port.h"
#define KEY_BUF_SIZE		6  	//按键数量
#define hasKey(a,b) a&(1<<b)
uint8 keybuf[KEY_BUF_SIZE];
#define delay_cyc 3 //防抖周期

extern void keyDown(uint8); //预定义， 调用者实现细节。
extern void keyUp(uint8);
extern void keyPress(uint8);

extern void keyinit()
{
	sint8 i=0;
	for(i=0;i<KEY_BUF_SIZE;i++)
		keybuf[i]=0;
}
extern void keyscan()
{
	int i;
	uint8 keyupcode=0,keypressedcode=0,keydowncode=0;
	for(i=0;i<KEY_BUF_SIZE;i++)
	{
		if((~KEYPORT)&(1<<i)) //当前键按下
		{
			if(keybuf[i]<0xff) keybuf[i]++; //防止溢出的自增
			if(keybuf[i]==delay_cyc) //上升沿触发
			{
				keydowncode |= (1<<i);
			}
			else if(keybuf[i]>delay_cyc) //按键保持状态
			{
				keypressedcode |= (1<<i);
			}
		}
		else
		{
			if(keybuf[i]>delay_cyc) //下降沿触发
			{
				keyupcode |= (1<<i);
			}
			keybuf[i]=0;
		}
	}
	if(keydowncode) keyDown(keydowncode);
	if(keyupcode) keyUp(keyupcode);
	if(keypressedcode) keyPress(keypressedcode);
}
#endif
