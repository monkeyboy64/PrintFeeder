#pragma once

#include <stdint.h>

template <typename Q>
class QueueItem {
public:
	QueueItem(Q val) : _val(val), _next(NULL) {}
	Q _val;
	QueueItem<Q> *_next;
};

template <typename T>
class Queue
{
public:
	Queue() : _first(NULL) , _last(NULL), _size(0) {}
	
	uint16_t size() { return _size; }

	void push(T val) {
		QueueItem<T> *item = new QueueItem<T>(val);
		
		if (_last == NULL) {
			_last = item;
			_first = _last;
		} else {
			_last->_next = item;
			_last = _last->_next;
		}

		++_size;
	}

	T pop() {
		T val;
		if (_first != NULL) {
			val = _first->_val;
			QueueItem<T> *temp = _first;

			_first = _first->_next;
			if (temp == _last) {
				_last == _first;
			}

			delete temp;

			--_size;
		}

		return val;
	}

private:
	QueueItem<T> *_first;
	QueueItem<T> *_last;
	uint16_t _size;
};

