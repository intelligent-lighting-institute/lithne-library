#include <Lithne.h>;

long timer = millis();            // Variable to store when we last transmitted something
const int TRANSMIT_DELAY = 3000;  // How long before we can transmit again

void setup() 
{
  // Initialize the Lithne communication. In case of a regular Arduino with only 1 serial port. Remove the ',Serial1' below.
  Lithne.init(115200, Serial1);
  
  // Add nodes to your network
  Lithne.addNode(COORDINATOR, XBeeAddress64(0x00000000, 0x00000000)); // Node 0 is always the PAN coordinator
  Lithne.addNode(1, XBeeAddress64(0x0013a200, 0x4079CE40));           // Another XBee
  Lithne.addNode(BROADCAST, XBeeAddress64(0x00000000, 0x0000FFFF));   // Broadcast Address
}

void loop() 
{
  // send a DB request (Signal strength is returned in DeciBel) every so many seconds - Function defined below
  timedDBRequest(); 
  
  // Check if we receive a Lithne message - this is important as it also retrieves the DB measurement
  // It does however only return true when a regular communication message is received; not a DB measurement
  if ( Lithne.available() ) { }
  
  // Check if there is a new DB measurement for a particular node (1) in this case - remember to always call Lithe.available() before this.
  if ( Lithne.newDBMeasurement(1) ) 
  {
    int dbStrength = Lithne.getDB(1); // Actually retrieve the DB value from the node
    // Now we can use this variable for cool purposes =] (The higher the value; the lower the signal strength)
  }
  delay(10);
}


void timedDBRequest() 
{
  if (abs(millis() - timer) > TRANSMIT_DELAY) // Check if it was long enough ago since we 
  {
    timer = millis();        // Keep track of the last sending time
    Lithne.sendDBRequest(1); // Actually send teh request
  }
}

