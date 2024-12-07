//
// Created by fade on 2024/12/5.
//

#ifndef RANDOM_EVENT_H
#define RANDOM_EVENT_H
#include "ns3/header.h"
#include "ns3/vector.h"
#include "ns3/nstime.h"

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
public:
  // 构造器
  RandomEvent(uint32_t m_event_id,
                const Vector& m_event_location,
                const Time& m_timestamp,
                const std::string& m_description,
                bool m_true_value,
                const Time& m_duration);
  // getter and setter
  [[nodiscard]] uint32_t m_event_id() const
  {
    return m_eventId;
  }
  void set_m_event_id(const uint32_t m_event_id)
  {
    m_eventId = m_event_id;
  }
  [[nodiscard]] Vector m_event_location() const
  {
    return m_eventLocation;
  }
  void set_m_event_location(const Vector& m_event_location)
  {
    m_eventLocation = m_event_location;
  }
  [[nodiscard]] Time m_timestamp1() const
  {
    return m_timestamp;
  }
  void set_m_timestamp(const Time& m_timestamp)
  {
    this->m_timestamp = m_timestamp;
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
