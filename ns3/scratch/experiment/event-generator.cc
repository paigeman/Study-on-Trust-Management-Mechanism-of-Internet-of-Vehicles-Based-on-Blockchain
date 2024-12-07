//
// Created by fade on 2024/12/5.
//
#include "event-generator.h"
#include "ns3/simulator.h"
#include "ns3/double.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("EventGenerator");

// Register this type，包含一些元数据
TypeId EventGenerator::GetTypeId()
{
    static TypeId tid = TypeId("ns3::EventGenerator")
        .SetParent<Application>().SetGroupName("experiment");
    return tid;
}

void EventGenerator::GenerateEvents(){
    auto & event_manager = EventManager::GetInstance();
    for (uint32_t i = 0; i < this->m_maxEventsPerSecond; ++i)
    {
        uint32_t eventId = event_manager.GetEventNumber();
        Vector eventLocation(this->m_randomX->GetValue(), this->m_randomY->GetValue(), 0);
        Time timestamp = Simulator::Now();
        std::ostringstream oss;
        oss << timestamp << ": Event #" << eventId << " at " << eventLocation;
        std::string description = oss.str();
        Time duration = Seconds(this->m_randomTime->GetValue());
        RandomEvent randomEvent(eventId,
            eventLocation, timestamp, description, true, duration);
        event_manager.AddEvent(randomEvent);
        NS_LOG_DEBUG(description);
    }
    this->ScheduleEvent();
}

EventGenerator::EventGenerator(const double m_x_min,
               const double m_y_min,
               const double m_x_max,
               const double m_y_max,
               const uint32_t m_max_events_per_second,
                   const Time& m_delay)
        : m_xMin(m_x_min),
          m_yMin(m_y_min),
          m_xMax(m_x_max),
          m_yMax(m_y_max),
          m_maxEventsPerSecond(m_max_events_per_second),
          m_delay(m_delay)
{
    this->m_randomX = CreateObject<UniformRandomVariable>();
    this->m_randomX->SetAttribute("Min", DoubleValue(m_x_min));
    this->m_randomX->SetAttribute("Max", DoubleValue(m_x_max));
    this->m_randomY = CreateObject<UniformRandomVariable>();
    this->m_randomY->SetAttribute("Min", DoubleValue(m_y_min));
    this->m_randomY->SetAttribute("Max", DoubleValue(m_y_max));
    this->m_randomTime = CreateObject<UniformRandomVariable>();
    this->m_randomTime->SetAttribute("Min", DoubleValue(0));
    // 事件的持续时间 最多1周 可调整 3600 * 24 * 7 = 604800s
    this->m_randomTime->SetAttribute("Max", DoubleValue(604800));
}

void EventGenerator::StartApplication(){
    this->ScheduleEvent();
}

void EventGenerator::StopApplication()
{
    if (m_eventId.IsPending())
    {
        Simulator::Cancel(m_eventId);
    }
}

void EventGenerator::ScheduleEvent(){
    this->m_eventId = Simulator::Schedule(this->m_delay, &EventGenerator::GenerateEvents, this);
}


