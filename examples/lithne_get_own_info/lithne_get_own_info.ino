/*  
    This example will show you several ways on how to acquire your own
    info from the local XBee attached. This information is acquired 
    through transmitting AT commands to the XBee.
    By doing so, you can acquire for instance the 64-bit address of the XBee
    and the PAN ID (local network ID).

    You can use this information so you don't have to hard-code it into
    your code.
*/


#include <Lithne.h>  //Include the Lithne Library

void setup()
{
  Serial.begin( 115200 );
  // Initialize the Lithne
  Lithne.begin( 115200, Serial1 );
  
  // Add nodes to your network
  Lithne.addNode(COORDINATOR, XBeeAddress64(0x00000000, 0x00000000)); // Node 0 is always the PAN coordinator
  Lithne.addNode(1, XBeeAddress64(0x0013a200, 0x4071CD81));           // Another XBee
  Lithne.addNode(141, XBeeAddress64(0x0013a200, 0x4079CE52));         // Another XBee
  Lithne.addNode(151, XBeeAddress64(0x0013a200, 0x4079CEBD));         // Another XBee
  Lithne.addNode(BROADCAST, XBeeAddress64(0x00000000, 0x0000FFFF));   // Broadcast Address
  
  /*  Because the XBee and Arduino need some time to get
      acquinted, we wait untill two seconds have passed.
      This will both give them sufficient time to boot.  */
  while( millis() < 2000 ){ }
  
  /*  The simplest way to acquire your own information is to
      call getMyInfo(). However, this function requests the
      information from the Arduino, but does not return it.
      You have to manualy request the information you want.  */ 
  /* Lithne.getMyInfo(); */
  
  /*  If you do not call getMyInfo() and directly call one
      of the other functions, the Arduino will request the
      information and then wait for a maximum of one second
      for a response. It will only do this when it does not
      yet have this information stored in its memory, e.g.:
      
      Serial.print("16-bit ADDR: ");
      Serial.println( Lithne.getMyAddress16(), HEX );
      */
      
  Serial.print("64-bit HIGH: ");
  Serial.println( Lithne.getMyAddress64().getMsb(), HEX );
  Serial.print("64-bit LOW : ");
  Serial.println( Lithne.getMyAddress64().getLsb(), HEX );
  Serial.print("16-bit ADDR: ");
  Serial.println( Lithne.getMyAddress16(), HEX );
  Serial.print("PAN ID     : ");
  Serial.println( Lithne.getMyPAN(), DEC );
  
  /*  You can now check who you are for instance. */
  Node * thisNode = Lithne.getNodeBy64( Lithne.getMyAddress64() );
  if ( thisNode != NULL)
  {
    Serial.print("Identity crisis solved! I am node ID ");
    Serial.println( thisNode->getID() );
    Serial.print("\t with 64 bit address ");
    Serial.print(Lithne.getMyAddress64().getMsb(), HEX);
    Serial.print(" ");
    Serial.println(Lithne.getMyAddress64().getLsb(), HEX);
  }
}

void loop()
{
  // Super boring loop
}
