#pragma once

#include "CmdMessenger.h"

// This is the list of recognized commands. These can be commands that can either be sent or received. 
// In order to receive, attach a callback function to these events
enum
{
    // Receive Messages
    kPrint,                  // Receive, with box and to_feed amount
    kConnected,              // Receive, with a unique id, acknowledge with kAttach
    
    // Send Messages.  All messages are sent with unique_id except kAttach
    kAttach,                 // Send, , when ready to attach and receive unique id
    kAcknowledgeConnected,   // Send, unique_id, when connected, send back unique id
    kAcknowledgePrint,       // Send, box and to_feed amount, Acknowledge that we received a print job
    kError,                  // Send, box and error, There was an error
    kJobCompleted,           // Send, box and fed, when a print job is completed,
    kUnknownCommand,
};

class Messages {
public:    
    Messages(CmdMessenger &cmdMessenger) : _cmdMessenger(cmdMessenger), _unique_id(400) {}
    
    void sendAttach();
    void sendAcknowledgeConnected();
    void sendAcknowledgePrint(uint16_t box, uint16_t to_feed);
    void sendError(uint16_t box, uint16_t error);
    void sendJobCompleted(uint16_t box, uint16_t fed);
    void sendUnknownCommand();

    void setUniqueId(uint16_t unique_id) { _unique_id = unique_id; }


private:
    CmdMessenger &_cmdMessenger;
    uint16_t _unique_id;
};
