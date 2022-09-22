
#include <SPI.h>     
#include <mcp2515.h>
struct can_frame canMsg;
MCP2515 mcp2515(53);
void setup(){
  Serial.begin(9600);                //Begins Serial Communication at 9600 baudrate
  SPI.begin();                       //Begins SPI communication
 
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();                  //Sets CAN at normal mode
}
void loop(){
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK){
    Serial.print(canMsg.can_id);Serial.print("\t");
    Serial.print(canMsg.can_dlc);;Serial.print("\t");
    for(int i=0;i< canMsg.can_dlc;i++){
      Serial.print(canMsg.data[i]);
    }
    Serial.print("\r\n");
    delay(1000);
  }
  else{
    Serial.print("Error \r\n");
    delay(1000);
  }
}
