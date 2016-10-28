# KEY PRESS CALLBACK REALIZE #

In embedded programming, we always do our key press action inside the interrupt or main loop, and we also need to do something to deal with the keystroke, this method is very hardcore, i wrote this key callback library, so we can programming like vb or other higher way, and not care about so much details, such as:

    #include "btn.h"
    void btn1_pressed()
    {
      Serial.println("btn1 pressed.");
    }
    void btn2_up()
    {
      Serial.println("btn2 up.");
    }
    void setup() {
      // put your setup code here, to run once:
      Serial.begin(9600);
      pinMode(3,INPUT_PULLUP);
      pinMode(4,INPUT_PULLUP);
      add_keydown_listener(3, &btn1_pressed);//binded to pin 3
      add_keyup_listener(4, &btn2_up);//binded to pin 4
      delay(1000);
    }
    void loop() {
      // put your main code here, to run repeatedly:
      btn_scan();
      delay(10);
    }
    
We just needed using 

    add_keydown_listener(pin,&function_name)
    add_keyup_listener(pin,&function_name)

to bind ourself defined function to the event, we can also detect the key up event,

that's cool.

To do this, the library using function pointer tricks to bind our self defined function later, so it named callback function.

winxos
10/29/2016 1:37:23 AM 