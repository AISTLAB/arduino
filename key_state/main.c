/*
������������ģ�ͣ��õ�Ƭ����������ͬwindows���һ���򵥡�
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
void keyDown(uint8 keycode); //�����½����Զ�����
void keyUp(uint8 keycode) //�����������Զ�����
{
	P1=keycode;
	delay(10);
}
void keyPress(uint8 keycode) //���������Զ�����
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
		keyscan(); //���Բ��õ�������ʱ����
		delay(100);
	}
}
