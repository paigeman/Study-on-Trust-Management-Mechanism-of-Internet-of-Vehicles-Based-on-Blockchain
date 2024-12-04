//
// Created by fade on 2024/12/4.
//

#ifndef EVENT_MESSAGE_H
#define EVENT_MESSAGE_H
#include "ns3/header.h"
#include "ns3/vector.h"
#include "ns3/timer.h"

namespace ns3 {

class EventMessage : public Header {
  // 消息编号
  uint32_t m_messageId;
  // 消息类型 todo
  // 事件位置坐标
  Vector m_eventLocation;
  // 事件描述 todo
  // 报告者位置坐标
  Vector m_reporterLocation;
  // 时间戳
  Time m_timestamp;
public:
  // constructor
  EventMessage(uint32_t m_message_id,
                 const Vector& m_event_location,
                 const Vector& m_reporter_location,
                 const Time& m_timestamp);
  // getter and setter
  void SetMessageId(const uint32_t id) { this->m_messageId = id; }
  uint32_t GetMessageId() const { return this->m_messageId; }
  void SetEventLocation(const Vector &location) { this->m_eventLocation = location; }
  Vector GetEventLocation() const { return this->m_eventLocation; }
  void SetReporterLocation(const Vector &location) { this->m_reporterLocation = location; }
  Vector GetReporterLocation() const { return this->m_reporterLocation; }
  void SetTimestamp(const Time &timestamp) { this->m_timestamp = timestamp; }
  Time GetTimestamp() const { return this->m_timestamp; }
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
#endif //EVENT_MESSAGE_H
