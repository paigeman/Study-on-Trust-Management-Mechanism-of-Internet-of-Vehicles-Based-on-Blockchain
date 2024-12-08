//
// Created by fade on 2024/12/5.
//

#ifndef EVENT_GENERATOR_H
#define EVENT_GENERATOR_H
#include "ns3/application.h"
#include "event-manager.h"
#include "ns3/random-variable-stream.h"

namespace ns3
{
class EventGenerator : public Application
{
  // 地图区域范围
  double m_xMin;
  double m_yMin;
  double m_xMax;
  double m_yMax;
  // 每秒（每次）生成的事件数量
  uint32_t m_maxEventsPerSecond;
  // 调度事件过期的延迟
  Time m_delay;
  // 用于生成位置的随机变量
  Ptr<UniformRandomVariable> m_randomX;
  Ptr<UniformRandomVariable> m_randomY;
  // 用于生成持续时间的随机变量
  Ptr<UniformRandomVariable> m_randomTime;
  // 用于生成影响范围的随机变量
  Ptr<UniformRandomVariable> m_randomRange;
  // 调度器事件的id
  EventId m_eventId;
  void StartApplication() override;
  void StopApplication() override;
public:
  // Register this type，包含一些元数据
  static TypeId GetTypeId();
  // 生成随机事件
  void GenerateEvents();
  EventGenerator(double m_x_min,
                   double m_y_min,
                   double m_x_max,
                   double m_y_max,
                   uint32_t m_max_events_per_second,
                   const Time& m_delay);
  void ScheduleEvent();
};
}
#endif //EVENT_GENERATOR_H
