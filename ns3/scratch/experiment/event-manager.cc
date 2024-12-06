//
// Created by fade on 2024/12/5.
//
#include "event-manager.h"

using namespace ns3;

void
EventManager::AddEvent(const Ptr<RandomEvent>& event)
{
    this->m_events.emplace_back(event);
}

EventManager::EventManager()
{

}

uint64_t EventManager::GetEventNumber() const
{
    return this->m_events.size();
}

