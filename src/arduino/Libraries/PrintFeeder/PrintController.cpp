#include "PrintController.h"

PrintController::PrintController(Messages &messages) : _messages(messages)
{
	for(uint16_t i = 0; i < PRINT_BOX_COUNT; ++i) {
		print_boxes[i] = new PrintBox(this);
	}
}

PrintController::~PrintController(){
	for(uint16_t i = 0; i < PRINT_BOX_COUNT; ++i) {
		delete print_boxes[i];
	}
}

bool PrintController::readyToPrint() {
    bool rval = false;
    
    if (wait_to_print == 0) {
        wait_to_print = millis();
    } else if ((millis() - wait_to_print) > WAIT_TO_PRINT_TIME) {
        rval = true;
    }
}

uint16_t PrintController::addPrintJob(uint16_t box, uint16_t to_feed) {
	// Reset timer since we are adding another print job
    wait_to_print = 0;
    
    int rval = E_UNDEFINED;

	if (box < PRINT_BOX_COUNT)
	{
		PrintJobPtr print_job = new PrintJob(box, to_feed);
		print_boxes[box]->addToQueue(print_job);
		rval = E_SUCCESS;
	} else {
		rval = E_INVALID_BOX;
	}

	return rval;
}

void PrintController::retirePrintJob(PrintJobPtr print_job) {
	retired_jobs.push(print_job);
}

void PrintController::addError(uint16_t box, ErrorType error) {
	errors.push(JobError(box, error));
}

void PrintController::print() {
	// Printing to reset the wait_timer
    wait_to_print = 0;
    
    bool continuePrinting = true;

	while(continuePrinting) {
		
		// TODO
        //input_state = readHardwareState();
		
		for(uint16_t i = 0; i < PRINT_BOX_COUNT; ++i) {
			continuePrinting |= print_boxes[i]->print();
		}
		
		// TODO
        //outputHardwareState();

		// report any errors
		while(errors.size() != 0) {
			JobError error = errors.pop();
            _messages.sendError(error.box, error.error);
		}

		// Send back message for the retired print jobs
		while (retired_jobs.size() != 0) {
			PrintJobPtr print_job = retired_jobs.pop();
            _messages.sendJobCompleted(print_job->box, print_job->fed);
			delete print_job;
		}
	}
}

uint16_t PrintController::readState(){
	return 0x0;
}

void PrintController::outputState(uint16_t output_state){
}


