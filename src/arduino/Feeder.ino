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
    s_cmdMessenger.attach(kConnected, OnConnected);
}

void OnUnknownCommand()
{
    s_cmdMessenger.sendCmd(kUnknownCommand, "Command without attached callback");
}

void OnPrint()
{
    uint16_t box = (uint16_t) s_cmdMessenger.readInt16Arg();
    uint16_t to_feed = (uint16_t) s_cmdMessenger.readInt16Arg();

    uint16_t rval = s_printController.addPrintJob(box, to_feed);
    if (rval == 0) {
        s_messages.sendAcknowledgePrint(box, to_feed);
    } else {
        s_messages.sendError(box, rval);
    }
}

void OnConnected() {
    uint16_t unique_id = (uint16_t) s_cmdMessenger.readInt16Arg();
    s_messages.setUniqueId(unique_id);
}

// Setup function
void setup() 
{
    // Listen on serial connection for messages from the PC
    Serial.begin(115200); 

    // Adds newline to every command
    //s_cmdMessenger.printLfCr();   

    // Attach my application's user-defined callback methods
    attachCommandCallbacks();

    // Let the PC Feed program now that we are ready to attach
    // It should send back a message with out unique id
    s_messages.sendAttach();
}

// Loop function
void loop() 
{
    // Process incoming serial data, and perform callbacks
    s_cmdMessenger.feedinSerialData();

    if (s_printController.readyToPrint()) {
        s_printController.print();
    }

    delay(10);
}