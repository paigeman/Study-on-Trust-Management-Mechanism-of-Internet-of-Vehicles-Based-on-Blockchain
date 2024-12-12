//
// Created by fade on 2024/12/7.
//
#include "rsu-app.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/simulator.h"
#include "ns3/ipv4.h"
#include "event-message.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("RsuApp");

TypeId RsuApp::GetTypeId()
{
    static TypeId tid = TypeId("ns3::RsuApp")
        .SetParent<Application>().SetGroupName("experiment");
    return tid;
}

void RsuApp::StartApplication()
{
    this->m_serverSocket = Socket::CreateSocket(GetNode(), UdpSocketFactory::GetTypeId());
    const auto local = InetSocketAddress(Ipv4Address::GetAny(), this->m_serverPort);
    if (this->m_serverSocket->Bind(local))
    {
        NS_FATAL_ERROR("Failed to bind socket");
    }
    this->m_serverSocket->SetRecvCallback(MakeCallback(&RsuApp::HandleRead, this));
}

void RsuApp::StopApplication()
{
    if (m_serverSocket)
    {
        this->m_serverSocket->Close();
    }
}

RsuApp::RsuApp(uint16_t serverPort) : m_serverPort(serverPort)
{

}

void RsuApp::HandleRead(Ptr<Socket> socket)
{
    Address from;
    Ptr<Packet> packet = socket->RecvFrom(from);
    if (packet)
    {
        std::ostringstream oss;
        oss << Simulator::Now() << ": RSU #" << GetNode()->GetId()
            << " at " << GetNode()->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal()
            << " received a package from Vehicle # at " << from;
        NS_LOG_DEBUG(oss.str());
    }
}
