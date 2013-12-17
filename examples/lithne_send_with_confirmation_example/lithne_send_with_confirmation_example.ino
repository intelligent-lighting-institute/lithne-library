#include <Lithne.h>

// We are going to send something every second and we use this to store when we last sent something
unsigned long sendTime = 0;
static const int SEND_TIMEOUT = 5000;


void setup() 
{
  // Begin the Lithne communication; if you use a Lithne board, the XBee is connected on Serial1, in other cases use Serial
  Lithne.begin(115200, Serial1);
  Serial.begin(115200); // For debugging we open the normal Serial Port
  // Now you may add nodes that are in your network to a list; specify any chosen ID and the XBeeAddress
  // The ID may be any unique number from 1 to 254 that you like. 0 is reserved for the COORDINATOR and 255 for BROADCAST
  Lithne.addNode(COORDINATOR, XBeeAddress64(0x00000000, 0x00000000));
  Lithne.addNode(BROADCAST  , XBeeAddress64(0x00000000, 0x0000FFFF)); 
  Lithne.addNode(1, XBeeAddress64(0x0013a200, 0x4079ce40));  // Add as many nodes as you like 
  
  Serial.println("Lithne Setup Complete");
}

void loop() 
{
  // We send something to the cerver every now and then
  timedSender();
}


/********** COMPOSING AND SENDING A MESSAGE ****************/

void timedSender() 
{
  // Send something every 3 seconds. When broadcasting, don't do it more often than once every two seconds or you will flood the network
  // If you send to a specific node, you can do it much more often.
  if( millis() - sendTime > 3000 ) 
  {  
    sendTime = millis();
    Lithne.setFunction("testFunction");          // Always set a function name for a message
    Lithne.setRecipient( COORDINATOR );          // Determine the recipient of the message; in this case send to the network coordinator
    Lithne.send();                               // Now we really send the message
    
    // Now we wait... as long as we have not timed out, and the message is not yet delivered 
    boolean timedOut = false;
    
    /* IMPORTANT NOTE - messageDelivered will only work for messages that were unicasted, NOT BROADCASTED */
    while( !Lithne.messageDelivered() && !timedOut)
    {
      Lithne.available(); // Check the incoming messages while we wait - this is where the confirmation comes in...
      if ( millis() - sendTime > SEND_TIMEOUT )
      {
        timedOut = true;
      }
    }
    // Now do something with the fact that the message either timed out or was successfully delievered
    if( !timedOut )
    {
      Serial.print("Message delivered in ");
      Serial.print(millis() - sendTime);
      Serial.println(" milliseconds");
    }
    else
    {
      Serial.print("Message timed out after ");
      Serial.print(millis() - sendTime);
      Serial.println(" milliseconds");
    }
  }
}
