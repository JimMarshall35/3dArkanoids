#pragma once
#include <memory>
#include <iostream>
#define SIZED_QUEUE_BOUNDS_CHECKING
template<typename T, int QueueSize>
class SizedQueue {
public:
	SizedQueue() {
		m_front = 0;
		m_back = -1;
		m_count = 0;
	}

	T Dequeue() {
#ifdef SIZED_QUEUE_BOUNDS_CHECKING
		if (IsEmpty()) {
			std::cout << "Overflow\nProgram Terminated\n";
			exit(EXIT_FAILURE);
		}
#endif
		T val = m_backingArray[m_front];
		m_front = (m_front + 1) % m_capacity;
		m_count--;
	}

	void Enqueue(T val) {
#ifdef SIZED_QUEUE_BOUNDS_CHECKING
		if (IsFull()) {
			std::cout << "Overflow\nProgram Terminated\n";
			exit(EXIT_FAILURE);
		}
#endif
		m_back = (m_back + 1) % m_capacity;
		m_backingArray[m_back];

		m_count++;
	}

	T& Peek() {
		return m_backingArray[m_front];
	}

	size_t Count() {
		return m_count;
	}

	bool IsEmpty() {
		return (m_count == 0);
	}

	bool IsFull() {
		return (m_count == m_capacity);
	}

private:
	T m_backingArray[QueueSize];
	size_t m_capacity = QueueSize;
	size_t m_front = 0;
	size_t m_back = 0;
	size_t m_count = 0;
};