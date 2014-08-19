#include "PrintBox.h"
#include "PrintController.h"

void PrintBox::setInputState(uint8_t state) {
    input_state = *(PrintBoxInputs *)(&state);
}

uint8_t PrintBox::getOutputState() {
    return *(uint8_t *)(&output_state);
}



bool PrintBox::print() {
	bool continuePrinting = false;
	bool done = false;

	while(!done) {
		unsigned long now = 0;
		switch (current_state) {
		case Idle:
			reported_error = false;
			
			// Nothing on top of sensor2, nothing to feed
			if (input_state.sensor2 == SENSOR_OFF) {
				current_state = FeedError;
				done = true;
			} else {
				if ((current_job == NULL) && (print_jobs.size() != 0)) {
					current_job = print_jobs.pop();
				}

				if (current_job != NULL) {
					feed_start_time = 0.0;
					output_start_time = 0.0;
				
					current_state = Feed;
				}
			}

			break;
	
		case Feed:
			now = millis();
			if (feed_start_time == 0) {
				feed_start_time = now;
				// start the state
				done = true;
			} else if (input_state.sensor1 == SENSOR_ON) {
				// sensor1 is on, so we've feed far enough
				current_state = Output;
			} else if ((now - feed_start_time) > FEED_STATE_MAX_TIME) {
				// We've been in the feed state for too long, there is a feed error
				current_state = FeedError;
			}
			break;
	
		case Output:
			now = millis();
			if (output_start_time == 0) {
				output_start_time = now;
				// start the state
				done = true;
			} else if (input_state.sensor1 == SENSOR_OFF) {
				// sensor1 has turned off, so the feed is done
				current_job->to_feed--;
				current_job->fed++;

				if (current_job->to_feed == 0) {
					// feed job is finished, so retire it
					controller->retirePrintJob(current_job);
					current_job = NULL;
				}

				current_state = Idle;
			} else if ((now - output_start_time) > OUTPUT_STATE_MAX_TIME) {
				// We've been in the output state for too long, there is an output error
				current_state = OutputError;
			}

			break;

		case IdleError:
			if (!reported_error) {
				
			}
			if ((input_state.sensor2 == SENSOR_ON) && (input_state.sensor1 == SENSOR_OFF)) {
				current_state = Idle;
			} else {
				done = true;
			}
			break;
		
		case FeedError:
			if ((input_state.sensor2 == SENSOR_ON) && (input_state.sensor1 == SENSOR_OFF)) {
				current_state = Idle;
			} else {
				done = true;
			} 
			break;

		case OutputError:
			if ((input_state.sensor2 == SENSOR_ON) && (input_state.sensor1 == SENSOR_OFF)) {
				current_state = Idle;
			} else {
				done = true;
			} 
			break;
		}
	}

	uint8_t state;
    // Write next_box_state
	switch (current_state) {
	case Idle:
        state = IDLE_STATE;
        continuePrinting = false;
		break;
	case Feed:
        state = FEED_STATE;
		continuePrinting = true;
		break;
	case Output:
		state = OUTPUT_STATE;
		continuePrinting = true;
		break;
	case IdleError:
	case FeedError:
	case OutputError:
		state = ERROR_STATE;
		continuePrinting = false;
		break;
	}

    // Set all the bits at once depending on the state
    output_state = *(PrintBoxOutputs *)(&state);

	return continuePrinting;
}

uint16_t PrintBox::addToQueue(PrintJobPtr print_job) {
	print_jobs.push(print_job);

	return 0;
}

