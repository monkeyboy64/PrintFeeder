#pragma once

#include "PrintBox.h"
#include "Messages.h"

#define PRINT_BOX_COUNT 5
// 1 Second wait between last job added and printing
#define WAIT_TO_PRINT_TIME 1000 

class JobError{
public:
	JobError() : box(0), error(None) {}
	JobError(uint16_t _box, ErrorType _error) : box(_box), error(_error) {}
	uint16_t box;
	ErrorType error;
};

class PrintController
{
public:
	PrintController(Messages &messages);
	virtual ~PrintController(void);

	uint16_t addPrintJob(uint16_t box, uint16_t to_feed);
	void retirePrintJob(PrintJobPtr print_job);
	void addError(uint16_t _box, ErrorType _error);

    bool readyToPrint();

	void print();

private:
	uint16_t readState();
	void outputState(uint16_t output_state);

	PrintBoxPtr print_boxes[PRINT_BOX_COUNT];
	PrintJobPtrQueue retired_jobs;
	Queue<JobError> errors; 

    uint16_t wait_to_print;

    Messages &_messages;
};

