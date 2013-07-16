#include <Lithne.h>

long lastSend = 0;

void setup() 
{
  Lithne.begin(115200, Serial1);
  // You may add Nodes that you would like to specifically address
  // Lithne.addNode(10, XBeeAddress64(0x0013a200,0x4071ce92));
}


void loop() 
{
  // Send something every second
  if( millis() - lastSend > 1000 ) 
  {
    // Store when the last message was sent
    lastSend = millis();
    
    // Send to coordinator by default
    Lithne.println("Running for " + String( int( millis()/1000 ) ) + " seconds" );
    
    // Or specify the recipient by ID as defined in Lithne.addNode(ID, XBEeeAddress64());
    // Lithne.println(10, "Running for " + String( int( millis()/1000 ) ) + " seconds" );
  }
}
