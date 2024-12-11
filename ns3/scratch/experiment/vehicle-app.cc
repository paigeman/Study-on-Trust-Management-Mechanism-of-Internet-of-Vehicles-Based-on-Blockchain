//
// Created by fade on 2024/12/7.
//
#include "vehicle-app.h"

#include "event-manager.h"
#include "event-message.h"

#include "ns3/ipv4.h"
#include "ns3/mobility-model.h"
#include "ns3/simulator.h"
#include "ns3/udp-socket-factory.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("VehicleApp");

TypeId VehicleApp::GetTypeId()
{
    static TypeId tid = TypeId("ns3::VehicleApp")
        .SetParent<Application>().SetGroupName("experiment");
    return tid;
}

void VehicleApp::StartApplication()
{
    // 为了时延考虑，使用UDP
    this->m_serverSocket = Socket::CreateSocket(GetNode(), UdpSocketFactory::GetTypeId());
    // 地址可改为车辆的IP地址
    const auto local = InetSocketAddress(Ipv4Address::GetAny(), this->m_serverPort);
    this->m_serverSocket->Bind(local);
    this->m_serverSocket->SetRecvCallback(MakeCallback(&VehicleApp::HandleRead, this));
    this->ScheduleEvent();
}

void VehicleApp::StopApplication()
{
    if (m_serverSocket)
    {
        this->m_serverSocket->Close();
    }
    if (this->m_eventId.IsPending())
    {
        Simulator::Cancel(this->m_eventId);
    }
}

void VehicleApp::CheckNearbyEvents()
{
    Ptr<MobilityModel> mobility = GetNode()->GetObject<MobilityModel>();
    const Vector position = mobility->GetPosition();
    auto & eventManager = EventManager::GetInstance();
    auto&& randomEvents = eventManager.GetAllLiveEvents();
    std::ostringstream oss;
    oss << Simulator::Now() << ": The number of live events: " << randomEvents.size()
        << ". The number of total events: " << eventManager.GetEventNumber();
    NS_LOG_DEBUG(oss.str());
    for (const auto& event : randomEvents)
    {
        // 事件没被感知过，判断是否在范围内
        // 某一次不在范围内，不代表以后不在范围内
        if (!this->m_detectedEventIds.contains(event.GetEventId()) && IsWithinRange(position, event))
        {
            oss.str(""); // Clear the previous content
            oss.clear(); // Clear the error state flags
            oss << Simulator::Now() << ": Vehicle #" << GetNode()->GetId() << " find event #" << event.GetEventId();
            NS_LOG_DEBUG(oss.str());
            this->m_detectedEventIds.insert(event.GetEventId());
            // 发送给RSU
            this->SentMessageToRSU(position, event);
        }
    }
    this->ScheduleEvent();
}

bool VehicleApp::IsWithinRange(const Vector & position, const RandomEvent & randomEvent)
{
    const auto eventLocation = randomEvent.GetEventLocation();
    const double influenceRange = randomEvent.GetInfluenceRange();
    const double dx = position.x - eventLocation.x;
    const double dy = position.y - eventLocation.y;
    // constexpr std::less_equal<> less_x;
    // return less_x(dx * dx + dy * dy, influenceRange * influenceRange);
    // 不确定double用<=号会不会出问题
    return dx * dx + dy * dy <= influenceRange * influenceRange;
}

void VehicleApp::ScheduleEvent()
{
    this->m_eventId = Simulator::Schedule(this->m_delay, &VehicleApp::CheckNearbyEvents, this);
}

VehicleApp::VehicleApp(const uint16_t rsuServerPort, const NodeContainer& rsuNodes, const uint16_t m_serverPort, const Time& m_delay) :
    m_rsuServerPort(rsuServerPort), m_rsuNodes(rsuNodes),
    m_serverPort(m_serverPort),
      m_delay(m_delay)
{
}

void VehicleApp::SentMessageToRSU(const Vector & position, const RandomEvent& randomEvent) const
{
    Ptr<Socket> socket = Socket::CreateSocket(GetNode(), UdpSocketFactory::GetTypeId());
    auto rsu = this->GetTargetRSU();
    const Ipv4Address remoteAddress = GetTargetRSUAddress(rsu);
    const InetSocketAddress remote(remoteAddress, this->m_rsuServerPort);
    const EventMessage eventMessage(EventMessage::GenerateId(), position, Simulator::Now(), randomEvent);
    // 创建要发送的包
    Ptr<Packet> packet = Create<Packet>();
    // 把header添加进包里
    packet->AddHeader(eventMessage);
    // 发送给RSU
    socket->SendTo(packet, 0, remote);
    std::ostringstream oss;
    oss << Simulator::Now() << ": " << "Vehicle #" << GetNode()->GetId()
        << " from " << GetNode()->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal()
        << " sent package to RSU #" << rsu->GetId() << " at " << remoteAddress;
    NS_LOG_DEBUG(oss.str());
    socket->Close();
}

void VehicleApp::HandleRead(Ptr<Socket> socket){

}

Ptr<Node> VehicleApp::GetTargetRSU() const
{
    return this->m_rsuNodes.Get(0);
}

Ipv4Address VehicleApp::GetTargetRSUAddress(Ptr<Node> rsu) {
    // todo 可扩展RSU的选择逻辑，目前只返回一个固定的地址
    return rsu->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal();
}


