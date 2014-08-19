#pragma once

#include <Arduino.h>
#include <Queue.h>

#include "Errors.h"

class PrintController;

struct PrintJob;
typedef PrintJob* PrintJobPtr;
typedef Queue<PrintJobPtr> PrintJobPtrQueue;

class PrintBox;
typedef PrintBox* PrintBoxPtr;

                          // redLED, greenLED, motor2, motor1
#define IDLE_STATE	 0x4  //      0,        1,      0,      0
#define OUTPUT_STATE 0x6  //      0,        1,      1,      0
#define FEED_STATE   0x7  //      0,        1,      1,      1
#define ERROR_STATE  0x8  //      1,        0,      0,      0

#define FEED_STATE_MAX_TIME 500 // 1/2 second
#define OUTPUT_STATE_MAX_TIME 500 // 1/2 second

#define SENSOR_ON  0
#define SENSOR_OFF 1

enum ErrorType {
	None = 0,
	IdleError_Error = E_IDLE,
	FeedError_Error = E_FEED,
	OutputError_Error = E_OUTPUT,
};

enum PrintState {
	Idle,
	Feed,
	Output,
	IdleError,
	FeedError,
	OutputError,
};

struct PrintBoxInputs {
    uint8_t sensor1  : 1;
    uint8_t sensor2  : 1;
};

struct PrintBoxOutputs {
    uint8_t motor1   : 1;
    uint8_t motor2   : 1;
    uint8_t redLED   : 1;
    uint8_t greenLED : 1;
};

class PrintJob {
public:
	PrintJob(uint16_t _box, uint16_t _to_feed): box(_box), to_feed(_to_feed), fed(0) {}
	uint16_t box;
	uint16_t to_feed;
	uint16_t fed;
};

class PrintBox{
public:
	PrintBox(PrintController *_controller) : controller(_controller) {}

    void setInputState(uint8_t state);
    uint8_t getOutputState();

    bool print();
	uint16_t addToQueue(PrintJobPtr print_job);

private:
    PrintBoxInputs input_state;     // state coming from the hardware
    PrintBoxOutputs output_state;   // state going to the hardware
	PrintState current_state;  // current state in the state machine
	Queue<PrintJobPtr> print_jobs;  // the set of jobs that need printed
	PrintJobPtr current_job;        // the job that is currently printing
	unsigned long feed_start_time;  // when the FEED state started
	unsigned long output_start_time; // when the output state started
	PrintController *controller;
	bool reported_error;            // has the current error been reported
};