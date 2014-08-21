#include <CmdMessenger.h>

#include <Errors.h>
#include <Messages.h>
#include <PrintBox.h>
#include <PrintController.h>
#include <Queue.h>


#include <CmdMessenger.h>  // CmdMessenger
#include <PrintController.h>  // PrintController


// Attach a new CmdMessenger object to the default Serial port
CmdMessenger s_cmdMessenger = CmdMessenger(Serial);

Messages s_messages = Messages(s_cmdMessenger);

// Create a new PrintController
PrintController s_printController = PrintController(s_messages);

void attachCommandCallbacks()
{
  s_cmdMessenger.attach(OnUnknownCommand);
  s_cmdMessenger.attach(kPrint, OnPrint);
  s_cmdMessenger.attach(kConnect, OnConnect);
}

void OnUnknownCommand()
{
  s_cmdMessenger.sendCmd(kUnknownCommand, "Command without attached callback");
}

void OnPrint()
{
  if (s_messages.connected()) {
    uint16_t box = (uint16_t) s_cmdMessenger.readInt16Arg();
    uint16_t to_feed = (uint16_t) s_cmdMessenger.readInt16Arg();
  
    uint16_t rval = s_printController.addPrintJob(box, to_feed);
    if (rval == 0) {
      s_messages.sendAcknowledgePrint(box, to_feed);
    } else {
      s_messages.sendError(box, rval);
    }
  }
}

void OnConnect() {

  s_messages.onConnect();

}

// Setup function
void setup()
{
  // Listen on serial connection for messages from the PC
  Serial.begin(115200);

  // Adds newline to every command
  s_cmdMessenger.printLfCr();

  // Attach my application's user-defined callback methods
  attachCommandCallbacks();
}

int try_connect = 0;
#define TRY_CONNECT_TIMEOUT 3000
// Loop function
void loop()
{
  // Send the Online message until connected to the computer
  if (!s_messages.connected()) {
    if ((millis() - try_connect) > TRY_CONNECT_TIMEOUT) {
      try_connect = millis();
      // Send the status to the PC that says the Arduino has booted
      s_cmdMessenger.sendCmd(kOnline, "PrintController has started!");
    }
    
  } else {
    if (s_printController.readyToPrint()) {
      s_printController.print();
    }
  }

  // Process incoming serial data, and perform callbacks
  s_cmdMessenger.feedinSerialData();

  delay(10);
}

