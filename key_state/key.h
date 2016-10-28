/*
ͨ�ð���ģ��
�õ�Ƭ����������ͬwindows���һ���򵥡�
winxos 2013-3-22
*/

#ifndef KEY_MOUDEL_H
#define KEY_MOUDEL_H
#include "Port.h"
#define KEY_BUF_SIZE		6  	//��������
#define hasKey(a,b) a&(1<<b)
uint8 keybuf[KEY_BUF_SIZE];
#define delay_cyc 3 //��������

extern void keyDown(uint8); //Ԥ���壬 ������ʵ��ϸ�ڡ�
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
		if((~KEYPORT)&(1<<i)) //��ǰ������
		{
			if(keybuf[i]<0xff) keybuf[i]++; //��ֹ���������
			if(keybuf[i]==delay_cyc) //�����ش���
			{
				keydowncode |= (1<<i);
			}
			else if(keybuf[i]>delay_cyc) //��������״̬
			{
				keypressedcode |= (1<<i);
			}
		}
		else
		{
			if(keybuf[i]>delay_cyc) //�½��ش���
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
