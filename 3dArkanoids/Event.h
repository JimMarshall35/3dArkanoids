#pragma once
#include <functional>
#include <vector>
#include <iostream>

template<typename EventArgs>
class EventListener {
public:
	virtual void OnEvent(EventArgs e) = 0;
};

/// <summary>
/// </summary>
/// <typeparam name="EventArgs">the args the event will take</typeparam>
template<typename EventArgs>
class Event
{
public:
	Event<EventArgs>& operator+=(EventListener<EventArgs>* listener) {
		auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
		if (it != m_listeners.end()) {
			std::cout << "function " << listener << " was subscribed to twice" << std::endl;
		}
		else {
			m_listeners.push_back(listener);
		}
		return *this;
	}
	Event<EventArgs>& operator-=(EventListener<EventArgs>* listener) {
		auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
		if (it != m_listeners.end()) {
			m_listeners.erase(it);
		}
		else {
			std::cout << "you tried to unsubscribe function " << listener << " but it was not found" << std::endl;
		}
		return *this;
	}
	Event<EventArgs>& operator()(EventArgs e) {
		for (auto listener : m_listeners) {
			listener->OnEvent(e);
		}
		return *this;
	}
private:
	std::vector<EventListener<EventArgs>*> m_listeners;
};