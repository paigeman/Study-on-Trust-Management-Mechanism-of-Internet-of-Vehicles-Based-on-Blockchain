//
// Created by fade on 2024/12/5.
//
#include "event-manager.h"
#include "ns3/simulator.h"

using namespace ns3;

void
EventManager::AddEvent(const RandomEvent& event)
{
    {
        std::lock_guard lock(m_vectorMutex);
        this->m_events.emplace_back(event);
    }
    {
        std::lock_guard lock(this->m_mapMutex);
        this->m_eventsMap.insert(std::make_pair(event.GetEventId(), event));
    }
}

EventManager::EventManager()
{

}

uint64_t EventManager::GetEventNumber() const
{
    std::lock_guard lock(m_vectorMutex);
    return this->m_events.size();
}

RandomEvent
EventManager::GetRandomEventById(const uint32_t id) const
{
    std::lock_guard lock(m_vectorMutex);
    return this->m_events.at(id);
}

std::vector<RandomEvent> EventManager::GetAllLiveEvents()
{
    std::vector<RandomEvent> events;
    const double currentTime = Simulator::Now().GetSeconds();
    {
        std::vector<uint32_t> idsToBeRemoved;
        std::lock_guard lock(m_mapMutex);
        for (const auto& [fst, snd] : this->m_eventsMap) {
            if (snd.GetTimestamp().GetSeconds() + snd.GetDuration().GetSeconds() > currentTime) {
                events.push_back(snd);
            } else
            {
                idsToBeRemoved.push_back(fst);
            }
        }
        if (!idsToBeRemoved.empty())
        {
            this->removeExpiredEvents(idsToBeRemoved);
        }
    }
    return events;
}

void EventManager::removeExpiredEvents(std::vector<uint32_t> & set)
{
    std::lock_guard lock(m_mapMutex);
    for (auto it = set.begin(); it != set.end(); ++it)
    {
        this->m_eventsMap.erase(*it);
    }
}


