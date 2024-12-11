//
// Created by fade on 2024/12/5.
//

#ifndef RANDOM_EVENT_H
#define RANDOM_EVENT_H
#include "ns3/header.h"
#include "ns3/vector.h"
#include "ns3/nstime.h"
#include <atomic>

namespace ns3
{

class RandomEvent : public Header
{
  // 事件编号
  uint32_t m_eventId;
  // 事件的位置
  Vector m_eventLocation;
  // 时间戳
  Time m_timestamp;
  // 事件描述
  std::string m_description;
  // 事件真值
  bool m_trueValue;
  // 持续时间
  Time m_duration;
  // 影响范围
  double m_influenceRange;
public:
  // 构造器
  RandomEvent(uint32_t m_event_id,
                const Vector& m_event_location,
                const Time& m_timestamp,
                const std::string& m_description,
                bool m_true_value,
                const Time& m_duration,
                double m_influenceRange);
  // getter and setter
  [[nodiscard]] uint32_t GetEventId() const
  {
    return this->m_eventId;
  }
  void setEventId(const uint32_t eventId)
  {
    this->m_eventId = eventId;
  }
  [[nodiscard]] Vector GetEventLocation() const
  {
    return this->m_eventLocation;
  }
  void setEventLocation(const Vector& eventLocation)
  {
    m_eventLocation = eventLocation;
  }
  [[nodiscard]] Time GetTimestamp() const
  {
    return this->m_timestamp;
  }
  void setTimestamp(const Time& timestamp)
  {
    this->m_timestamp = timestamp;
  }
  [[nodiscard]] std::string GetDescription() const
  {
    return m_description;
  }
  void setDescription(const std::string& m_description)
  {
    this->m_description = m_description;
  }
  [[nodiscard]] bool m_true_value() const
  {
    return m_trueValue;
  }
  void set_m_true_value(const bool m_true_value)
  {
    m_trueValue = m_true_value;
  }
  [[nodiscard]] Time GetDuration() const
  {
    return this->m_duration;
  }
  void setDuration(const Time& duration)
  {
    this->m_duration = duration;
  }
  [[nodiscard]] double GetInfluenceRange() const
  {
    return this->m_influenceRange;
  }
  void setInfluenceRange(double influenceRange)
  {
    m_influenceRange = influenceRange;
  }
  // Register this type，包含一些元数据
  static TypeId GetTypeId();
  // Inherited from header:
  TypeId GetInstanceTypeId() const override;
  // 序列化与反序列化相关
  uint32_t GetSerializedSize() const override;
  void Serialize(Buffer::Iterator start) const override;
  uint32_t Deserialize(Buffer::Iterator start) override;
  void Print(std::ostream& os) const override;
};

}

#endif //RANDOM_EVENT_H
