//
// Created by fade on 2024/12/4.
//

#ifndef EVENT_MESSAGE_H
#define EVENT_MESSAGE_H
#include "random-event.h"

namespace ns3 {

// 事件消息
class EventMessage : public Header {
  // 车辆id
  uint32_t m_vehicleId;
  // 消息编号
  uint32_t m_messageId;
  // 消息类型 todo
  // 报告者位置坐标
  Vector m_reporterLocation;
  // 时间戳
  Time m_timestamp;
  // 随机事件
  RandomEvent m_randomEvent;
public:
  // constructor
  EventMessage(uint32_t vehicleId,
                 uint32_t m_message_id,
                 const Vector& m_reporter_location,
                 const Time& m_timestamp,
                 const RandomEvent& randomEvent);
  // getter and setter
  void SetMessageId(const uint32_t id) { this->m_messageId = id; }
  uint32_t GetMessageId() const { return this->m_messageId; }
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
  // 简陋的id生成方案
  static uint32_t GenerateId() {
    static std::atomic<uint32_t> nextId{0};
    return nextId++;
  }
};

}
#endif //EVENT_MESSAGE_H
