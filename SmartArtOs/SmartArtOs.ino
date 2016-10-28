//this file no need to modify.
//schedule on arduino.
//winxos 2013-03-03
#include <LiquidCrystal.h>
#include <Schedule.h>
#include <IRremote.h>
#include <TimerOne.h>
int RECV_PIN = 3;
int OUT_PIN=4;
int LED_PIN=2;
int TMP_PIN=0;
int LCD_PIN=11;
int LIGHT_PIN=1;

const int ct=10;
int readbuf[ct];

int flag=0,bflag=1,cflag=1;
long oldcmd=0,oldt=0; //avoid recieved same data many times.
IRrecv irrecv(RECV_PIN);
decode_results results;
LiquidCrystal lcd(13,12,9,8,7,6);
int maxt,mint, temperature;

void showt(int n)
{
  lcd.print(n/10);
  lcd.print(".");
  lcd.print(n%10);
  lcd.print((char)223);
}
void bsort(int *a, int n)
{
  for (int i = 1; i < n; ++i)
  {
    int j = a[i];
    int k;
    for (k = i - 1; (k >= 0) && (j < a[k]); k--)
    {
      a[k + 1] = a[k];
    }
    a[k + 1] = j;
  }
}
long multiRead(const int pin)
{
  for(int i=0;i<ct;i++)
  {
    readbuf[i]=0;
    readbuf[i]=analogRead(pin);
  }
  bsort(readbuf,ct);
  return readbuf[ct/2];  
}
//main task
void deal_IR()
{
  if (irrecv.decode(&results)) 	
  {
    Serial.println(results.value);
    if(oldcmd!=results.value)
    {
      if(results.value==740239)      //power
      {
        bflag=1-bflag;
        digitalWrite(OUT_PIN,bflag);                //off
      }
      else if(results.value==412559) //mode btn
      {
        flag=1-flag;
        digitalWrite(LED_PIN,flag);
      }
      else if(results.value==928655) //slide btn
      {
        cflag=1-cflag;
        //digitalWrite(LCD_PIN,cflag);
      }
      else{
      }
      oldcmd=results.value;
    }
    else
    {
      oldcmd=0;
    }
    irrecv.resume(); // 接收下一个值
  }
}
void deal_temp()
{
  temperature=multiRead(TMP_PIN)*4.8828125;
  if(maxt<temperature)maxt=temperature;
  if(mint>temperature)mint=temperature;
  lcd.setCursor(0,0);
  lcd.print("H:");
  showt(maxt);
  lcd.print(" ");
  lcd.print("L:");
  showt(mint);
  lcd.print("  ");
  lcd.setCursor(4,1);
  lcd.print("Now: ");
  showt(temperature);
  lcd.print("C");
}
int light_lcd(int li) //deal with vue.
{
  if(li>800)return 1;  //dark
  if(li<600)return 0;
  if(li-oldt>20)return 1;
  oldt=li;
  return 0;
}
void deal_other()
{
  int t=multiRead(LIGHT_PIN);
  if(light_lcd(t)==1 && cflag) //dark, light lcd
  {
    digitalWrite(LCD_PIN,HIGH); //enviroument dark
  }
  else
  {
    digitalWrite(LCD_PIN,LOW); //light
  }
}

void init_os()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); //初始化红外遥控
  pinMode(OUT_PIN,OUTPUT);
  pinMode(LED_PIN,OUTPUT);
  pinMode(LCD_PIN,OUTPUT);
  digitalWrite(LED_PIN,HIGH);

  lcd.begin(16,2);
  lcd.clear();
  Schedule1.SCH_Add_Task(deal_IR, 0, 10); //0.1 sec
  Schedule1.SCH_Add_Task(deal_temp,0, 5000); //5sec	 	
  Schedule1.SCH_Add_Task(deal_other,0, 10); //0.1 sec
  oldt=multiRead(LIGHT_PIN);
  maxt=mint=multiRead(TMP_PIN)*4.8828125;
}

//
void setup() 
{
  init_os();
  Timer1.initialize(10000); //tick period
  Timer1.attachInterrupt(tic);
}
void tic() //timer1
{
  Schedule1.SCH_Update();
}
void loop() 
{
  // put your main code here, to run repeatedly: 
  Schedule1.SCH_Dispatch_Tasks();
}




