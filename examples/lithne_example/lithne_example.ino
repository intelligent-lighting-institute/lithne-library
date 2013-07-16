#include <Lithne.h>

// We are going to send something every second and we use this to store when we last sent something
long lastSend = 0;

void setup() 
{
  // Begin the Lithne communication; if you use a Lithne board, the XBee is connected on Serial1, in other cases use Serial
  Lithne.begin(115200, Serial1);
  // Now you may add nodes that are in your network to a list; specify any chosen ID and the XBeeAddress
  // The ID may be any unique number from 1 to 254 that you like. 0 is reserved for the COORDINATOR and 255 for BROADCAST
  Lithne.addNode(COORDINATOR, XBeeAddress64(0x00000000, 0x00000000));
  Lithne.addNode(BROADCAST  , XBeeAddress64(0x00000000, 0x0000FFFF)); 
  Lithne.addNode(1, XBeeAddress64(0x0013a200, 0x4071ce4b));  // Add as many nodes as you like 
  Lithne.addNode(7, XBeeAddress64(0x0013a200, 0x4071ce7b));
  
  Lithne.addScope("Office");      // Optionally determine to which scopes we belong; this is used to determine who listens to the message if it is broadcasted
  Lithne.addScope("Desk");        // You may add up to 10 scopes, you can for sintance use it in a hierarchical way; we are part of the office, and more specifically, the desk.
}

void loop() 
{
  // Check if we have a new message waiting for us
  if (Lithne.available()) 
  {
    // Process the incoming message in a user-made function (just to keep the loop clean-looking =]
    processLithneMessage();
  }
  // We send something to the cerver every now and then
  timedSender();
}


/********** COMPOSING AND SENDING A MESSAGE ****************/

void timedSender() 
{
  // Send something every 3 seconds. When broadcasting, don't do it more often than once every two seconds or you will flood the network
  // If you send to a specific node, you can do it much more often.
  if( millis() - lastSend > 3000 ) 
  {  
    lastSend = millis();
    Lithne.setFunction("timedSender");           // Always set a function name for a message
    Lithne.setRecipient( COORDINATOR );          // Determine the recipient of the message; in this case send to the network coordinator
    /* Alternatively you may use a nodeID that you determined in the addNode function, or an XBeeAddress
    Lithne.setRecipient( 1 ); 
    Lithne.setRecipient( XBeeAddress64(0x0013a200, 0x4071ce4b) ); 
    Lithne.setRecipient( BROADCAST ); 
    // If you decide to broadcast to the entire network, you may determine the scope of the message to allow nodes to state who should listen to the message
    Lithne.setScope("Office");                   
    */    
    // Now we add arguments to the message; the values that we wish to transmit
    Lithne.addArgument( int( millis()/1000 ) );     // Add 1st argument; in this case the number of seconds we have been running
    Lithne.addArgument( int( random(0,100) ) );     // You may add as many arguments as you like, for example purposes we also add a random number
    /* Alternatively you may send a string, only one string can be sent as content of a message
    Lithne.setStringArgument( "Hello World! =]" );
    */
    Lithne.send(); // Now we really send the message
  }
}


/********** PROCESSING A RECEIVED MESSAGE ****************/

void processLithneMessage()
{
  /* First check if we are in the scope of the message (is the message for us?). This is defined by addScope() function calls in teh setup
  *  If no scope is defined in the incoming message, this always returns true.
  */
  int messageScope = Lithne.getScope();
  if ( Lithne.hasScope( messageScope ) ) 
  {
    // Now check which function call is coming in. 
    if (Lithne.functionIs("setLed"))
    {
      // To be sure the message was formatted correctly, check if the number of arguments is as we expect it
      if (Lithne.getNumberOfArguments() == 1) 
      {
        // Now we get the first (and in this case only) argument from the message
        int value = Lithne.getArgument(0);
        // If the value is 1, we turn the debug led on pin 41 (located under the XBee on the Lithne board) on
        // For a regular Arduino, change 41 to 13 which is the default LED pin on Arduino
        if ( value == 1 )  { digitalWrite(41, HIGH); }
        else               { digitalWrite(41, LOW);  }
        
        // For example purposes, we return the value of the LED to the sender of this message.
        Lithne.setFunction("ledValue");                 // Specify the function, make this a meaningful name and use the same on the other side
        Lithne.setRecipient( Lithne.getSender64() );    // Determine the recipient of the message; in this case we return to the sender of the message
        Lithne.addArgument( value );                  // Now we add the value that we received as an argument to the message
        Lithne.send();                                  // Now we really send the message
      }
    }
  }
}
