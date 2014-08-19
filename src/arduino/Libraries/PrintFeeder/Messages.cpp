#include "Messages.h"

void Messages::sendAttach() {
    _cmdMessenger.sendCmd(kAttach, "Attach");
}

void Messages::sendAcknowledgeConnected() {
    _cmdMessenger.sendCmdStart(kAcknowledgeConnected);
    _cmdMessenger.sendCmdArg(_unique_id);
    _cmdMessenger.sendCmdArg("Ready");
    _cmdMessenger.sendCmdEnd();
}

void Messages::sendAcknowledgePrint(uint16_t box, uint16_t to_feed) {
    _cmdMessenger.sendCmdStart(kAcknowledgePrint);
    _cmdMessenger.sendCmdArg(_unique_id);
    _cmdMessenger.sendCmdArg(box);
    _cmdMessenger.sendCmdArg(to_feed);
    _cmdMessenger.sendCmdEnd();
}

void Messages::sendError(uint16_t box, uint16_t error) {
    _cmdMessenger.sendCmdStart(kError);
    _cmdMessenger.sendCmdArg(_unique_id);
    _cmdMessenger.sendCmdArg(box);
    _cmdMessenger.sendCmdArg(error);
    _cmdMessenger.sendCmdEnd();
}

void Messages::sendJobCompleted(uint16_t box, uint16_t fed) {
    _cmdMessenger.sendCmdStart(kJobCompleted);
    _cmdMessenger.sendCmdArg(_unique_id);
    _cmdMessenger.sendCmdArg(box);
    _cmdMessenger.sendCmdArg(fed);
    _cmdMessenger.sendCmdEnd();
}

void Messages::sendUnknownCommand() {
    _cmdMessenger.sendCmdStart(kUnknownCommand);
    _cmdMessenger.sendCmdArg(_unique_id);
    _cmdMessenger.sendCmdArg("Unknown Command");
    _cmdMessenger.sendCmdEnd();
}
