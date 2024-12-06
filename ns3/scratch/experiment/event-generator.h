//
// Created by fade on 2024/12/5.
//

#ifndef EVENT_GENERATOR_H
#define EVENT_GENERATOR_H
#include "ns3/application.h"
#include "event-manager.h"

namespace ns3
{
class EventGenerator : public Application
{
  // 地图区域范围
  double m_xMin;
  double m_yMin;
  double m_xMax;
  double m_yMax;
  // 每秒生成的事件数量
  uint32_t m_maxEventsPerSecond;
  //
};
}
#endif //EVENT_GENERATOR_H
