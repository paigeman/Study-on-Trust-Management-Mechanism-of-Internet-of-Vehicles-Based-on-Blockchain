//
// Created by fade on 2024/12/4.
//
#include "event-message.h"

using namespace ns3;

TypeId
EventMessage::GetTypeId()
{
    static TypeId tid = TypeId("ns3::EventMessage").SetParent<Header>().SetGroupName("experiment");
    return tid;
}

TypeId
EventMessage::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t EventMessage::GetSerializedSize() const
{
    // m_messageId is 4 bytes
    // one Vector has three double, we have two Vectors
    // the last 8 bytes is for m_timestamp
    return 4 + 8 * 3 * 2 + 8;
}

void
EventMessage::Serialize(Buffer::Iterator start) const
{
    start.WriteHtonU32(this->m_messageId);
    start.WriteHtonU64(*reinterpret_cast<const uint64_t*>(&(this->m_eventLocation.x)));
    start.WriteHtonU64(*reinterpret_cast<const uint64_t*>(&(this->m_eventLocation.y)));
    start.WriteHtonU64(*reinterpret_cast<const uint64_t*>(&(this->m_eventLocation.z)));
    start.WriteHtonU64(*reinterpret_cast<const uint64_t*>(&(this->m_reporterLocation.x)));
    start.WriteHtonU64(*reinterpret_cast<const uint64_t*>(&(this->m_reporterLocation.y)));
    start.WriteHtonU64(*reinterpret_cast<const uint64_t*>(&(this->m_reporterLocation.z)));
    start.WriteHtonU64(static_cast<const uint64_t>((this->m_timestamp.GetNanoSeconds())));
}

uint32_t EventMessage::Deserialize(Buffer::Iterator start)
{
    Buffer::Iterator i = start;
    this->m_messageId = i.ReadNtohU32();
    const uint64_t eventLocationX = i.ReadNtohU64();
    const uint64_t eventLocationY = i.ReadNtohU64();
    const uint64_t eventLocationZ = i.ReadNtohU64();
    this->m_eventLocation.x = *reinterpret_cast<const double*>(&eventLocationX);
    this->m_eventLocation.y = *reinterpret_cast<const double*>(&eventLocationY);
    this->m_eventLocation.z = *reinterpret_cast<const double*>(&eventLocationZ);
    const uint64_t reporterLocationX = i.ReadNtohU64();
    const uint64_t reporterLocationY = i.ReadNtohU64();
    const uint64_t reporterLocationZ = i.ReadNtohU64();
    this->m_reporterLocation.x = *reinterpret_cast<const double*>(&reporterLocationX);
    this->m_reporterLocation.y = *reinterpret_cast<const double*>(&reporterLocationY);
    this->m_reporterLocation.z = *reinterpret_cast<const double*>(&reporterLocationZ);
    this->m_timestamp = NanoSeconds(i.ReadNtohU64());
    return i.GetDistanceFrom(start);
}

EventMessage::EventMessage(const uint32_t m_message_id,
                 const Vector& m_event_location,
                 const Vector& m_reporter_location,
                 const Time& m_timestamp)
        : m_messageId(m_message_id),
          m_eventLocation(m_event_location),
          m_reporterLocation(m_reporter_location),
          m_timestamp(m_timestamp)
{
}

void EventMessage::Print (std::ostream& os) const
{
    os << "MessageId: " << this->m_messageId << "\n"
       << "EventLocation: " << this->m_eventLocation << "\n"
       << "ReporterLocation: " << this->m_reporterLocation << "\n"
       << "Timestamp: " << this->m_timestamp << "\n";
}