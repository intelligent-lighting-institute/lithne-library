/*
  Get signal strength of various nodes.
  The higher the value; the lower the signal strength (further away)
  Values range approximatily from 50 (very close) to 100 (very far)
  
  Distance is just one of the factors influencing signal strength. 
  Covering the XBee with your hand has about the same influence as walking 10 meters away
*/

#include <Lithne.h>;

long timer = millis();            // Variable to store when we last transmitted something
const int TRANSMIT_DELAY = 3000;  // How long before we can transmit again

void setup() 
{
  // For debugging (Serial.println()), we initialize a serial port
  Serial.begin(115200);
  // Initialize the Lithne communication. In case of a regular Arduino with only 1 serial port. Remove the ',Serial1' below.
  Lithne.init(115200, Serial1);
  
  // Add nodes to your network
  Lithne.addNode(COORDINATOR, XBeeAddress64(0x00000000, 0x00000000)); // Node 0 is always the PAN coordinator
  Lithne.addNode(1, XBeeAddress64(0x0013a200, 0x4071CD81));           // Another XBee
  Lithne.addNode(141, XBeeAddress64(0x0013a200, 0x4079CE52));         // Another XBee
  Lithne.addNode(151, XBeeAddress64(0x0013a200, 0x4079CEBD));         // Another XBee
  Lithne.addNode(BROADCAST, XBeeAddress64(0x00000000, 0x0000FFFF));   // Broadcast Address
}

void loop() 
{
  // send a DB request (Signal strength is returned in DeciBel) every so many seconds - Function defined below
  timedDBRequest(); 
  
  // Check if we receive a Lithne message - this is important as it actually retrieves the DB measurement
  if (Lithne.dbAvailable())
  {
    int nodeID     = Lithne.newDBForNode();  // Returns the ID of a Node that has a new measurement
    int dbStrength = Lithne.getDB( nodeID ); // Gets the value. Untill getDB is called, the measurement will be considered new
    
    Serial.print("New DB measurement for Node ID ");
    Serial.print(nodeID);
    Serial.print("\t");
    Serial.println(dbStrength);
  }
  
  /*
  // Alternatively you may Check if there is a new DB measurement for a particular node (1) in this case
  if ( Lithne.newDBForNode(1) ) 
  {
    int dbStrength = Lithne.getDB(1); // Actually retrieve the DB value from the node
  }
  */
}


void timedDBRequest() 
{
  if (abs(millis() - timer) > TRANSMIT_DELAY) // Check if it was long enough ago since we 
  {
    timer = millis();          // Keep track of the last sending time
    Lithne.sendDBRequest(141); // Actually send teh request
  }
}

