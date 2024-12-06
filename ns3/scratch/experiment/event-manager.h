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
  std::vector<Ptr<RandomEvent>> m_events;
  static EventManager& GetInstance() {
    static EventManager instance;
    return instance;
  }
public:
  void AddEvent(const Ptr<RandomEvent>& event);
  EventManager();
  uint64_t GetEventNumber() const;
};

}
#endif //EVENT_MANAGER_H
