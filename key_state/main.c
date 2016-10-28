/*
按键触发机制模型，让单片机按键处理同windows软件一样简单。
winxos 2013-3-22
*/
#include "config.h"
#include "key.h"
void delay(int n)
{
	int i,j;
	for (i=0;i<n;i++) 
		for (j=0;j<123;j++);
}
void keyDown(uint8 keycode); //按键下降沿自动触发
void keyUp(uint8 keycode) //按键上升沿自动触发
{
	P1=keycode;
	delay(10);
}
void keyPress(uint8 keycode) //按键不松自动触发
{
	if(hasKey(keycode,KEY_LEFT)) P1=0x55;
	if(hasKey(keycode,KEY_OK)) P1=0x11;
}
void main(void)
{
	keyinit();
	while (1)
	{
		P1=0;
		keyscan(); //可以采用调度器定时触发
		delay(100);
	}
}
