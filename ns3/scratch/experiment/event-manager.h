//
// Created by fade on 2024/12/5.
//

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H
#include "random-event.h"
#include "vector"
#include "mutex"

namespace ns3
{

class EventManager
{
  std::vector<RandomEvent> m_events;
  std::unordered_map<uint32_t, RandomEvent> m_eventsMap;
  mutable std::recursive_mutex m_mapMutex;
  mutable std::recursive_mutex m_vectorMutex;
  void removeExpiredEvents(std::vector<uint32_t> & set);
public:
  void AddEvent(const RandomEvent& event);
  EventManager();
  uint64_t GetEventNumber() const;
  RandomEvent GetRandomEventById(uint32_t id) const;
  static EventManager& GetInstance() {
    static EventManager instance;
    return instance;
  }
  // 获取所有未过期的事件
  std::vector<RandomEvent> GetAllLiveEvents();
};

}
#endif //EVENT_MANAGER_H
