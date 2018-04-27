#include <Canbus.h>
#include <defaults.h>
#include <global.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>
#include <stdlib.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>

// set the LCD address to 0x27 for a 16 chars 2 line display A FEW use address 0x3F, 20x4 uses 0x3F address
// Set the pins on the I2C chip used for LCD connections: addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

int iMsgCount; 
long int lTot;

void setup() {
  lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines (or 20x4) turn on backlight
  Serial.begin(9600); // For debug use
  Serial.println("CAN Read - Testing receipt of CAN Bus message");  
  delay(1000);
  
  if(Canbus.init(CANSPEED_125))  //Initialise MCP2515 CAN controller at the specified speed
    Serial.println("CAN Init ok");
  else
    Serial.println("Can't init CAN");

    lcd.setCursor(0,0);
    lcd.print("Starting...");
  delay(1000);

  iMsgCount = 0;
  lTot = 0;
}

void loop(){

tCAN message;
  
if (mcp2515_check_message()) 
  {
    if (mcp2515_get_message(&message)) 
  {
        
        if (message.id == 0x217) 
          {
               iMsgCount = iMsgCount + 1;
               
            // We need to take the average here and reset periodically, since it appears the float is
            // bouncing around more at < 1/2 tank.
               lcd.setCursor(0,0);
               lcd.print("ID 217 Dec Val");
               lcd.setCursor(1,1);
               lcd.print(message.data[4], DEC);
               
               lTot = lTot + long(message.data[4]);
               if (iMsgCount > 20)
               {
                lcd.setCursor(8, 1);
                lcd.print("Avg ");
                lcd.setCursor(13, 1);
                lcd.print(lTot/iMsgCount);
                iMsgCount = 0;
                lTot = 0;
               }
          }
     }}

}
