//
// Created by fade on 2024/12/5.
//

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H
#include "random-event.h"
#include "vector"

namespace ns3
{

class EventManager
{
  std::vector<RandomEvent> m_events;
public:
  void AddEvent(const RandomEvent& event);
  EventManager();
  uint64_t GetEventNumber() const;
  RandomEvent GetRandomEventById(uint32_t id) const;
  static EventManager& GetInstance() {
    static EventManager instance;
    return instance;
  }
};

}
#endif //EVENT_MANAGER_H
