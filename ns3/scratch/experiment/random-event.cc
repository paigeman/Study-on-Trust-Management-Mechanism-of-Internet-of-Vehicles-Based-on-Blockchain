//
// Created by fade on 2024/12/5.
//
#include "random-event.h"

using namespace ns3;

TypeId
RandomEvent::GetTypeId()
{
    static TypeId tid = TypeId("ns3::RandomEvent").SetParent<Header>().SetGroupName("Network");
    return tid;
}

TypeId
RandomEvent::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t RandomEvent::GetSerializedSize() const
{
    // m_eventId is 4 bytes
    // one Vector has three double
    // m_timestamp is 8 bytes
    // Size of the string length is 8 bytes
    // m_trueValue is 8 bytes 避免不同平台实现不同
    return 4 + 8 * 3 + 8 + 8 + this->m_description.size() + 8;
}

void
RandomEvent::Serialize(Buffer::Iterator start) const
{
    start.WriteHtonU32(this->m_eventId);
    start.WriteHtonU64(*reinterpret_cast<const uint64_t*>(&(this->m_eventLocation.x)));
    start.WriteHtonU64(*reinterpret_cast<const uint64_t*>(&(this->m_eventLocation.y)));
    start.WriteHtonU64(*reinterpret_cast<const uint64_t*>(&(this->m_eventLocation.z)));
    start.WriteHtonU64(static_cast<const uint64_t>((this->m_timestamp.GetNanoSeconds())));
    start.WriteHtonU64(this->m_description.size());
    start.Write(reinterpret_cast<const uint8_t *>(this->m_description.c_str()), this->m_description.size());
    start.WriteHtonU64(this->m_trueValue);
}

uint32_t RandomEvent::Deserialize(Buffer::Iterator start)
{
    Buffer::Iterator i = start;
    this->m_eventId = i.ReadNtohU32();
    const uint64_t eventLocationX = i.ReadNtohU64();
    const uint64_t eventLocationY = i.ReadNtohU64();
    const uint64_t eventLocationZ = i.ReadNtohU64();
    this->m_eventLocation.x = *reinterpret_cast<const double*>(&eventLocationX);
    this->m_eventLocation.y = *reinterpret_cast<const double*>(&eventLocationY);
    this->m_eventLocation.z = *reinterpret_cast<const double*>(&eventLocationZ);
    this->m_timestamp = NanoSeconds(i.ReadNtohU64());
    const uint64_t size = i.ReadNtohU64();
    char buffer[size + 1];
    i.Read(reinterpret_cast<uint8_t *>(buffer), size);
    buffer[size] = '\0';
    this->m_description = std::string(buffer);
    this->m_trueValue = static_cast<bool>(i.ReadNtohU64());
    return i.GetDistanceFrom(start);
}

void RandomEvent::Print(std::ostream& os) const
{
    os << "EventId: " << this->m_eventId << "\n"
       << "EventLocation: " << this->m_eventLocation << "\n"
       << "Timestamp: " << this->m_timestamp << "\n"
       << "Description: " << this->m_description << "\n"
       << "TrueValue: " << this->m_trueValue << "\n";
}

RandomEvent::RandomEvent(const uint32_t m_event_id,
                const Vector& m_event_location,
                const Time& m_timestamp,
                const std::string& m_description,
                const bool m_true_value)
        : m_eventId(m_event_id),
          m_eventLocation(m_event_location),
          m_timestamp(m_timestamp),
          m_description(m_description),
          m_trueValue(m_true_value) {}