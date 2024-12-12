//
// Created by fade on 2024/12/7.
//

#ifndef VEHICLE_APP_H
#define VEHICLE_APP_H
#include "ns3/node-container.h"
#include "random-event.h"
#include "unordered_set"

#include "ns3/application.h"
#include "ns3/socket.h"

namespace ns3
{
// 车辆应用
class VehicleApp : public Application
{
  // RSU 服务端port
  uint16_t m_rsuServerPort;
  // RSU节点
  NodeContainer m_rsuNodes;
  // 服务端port
  uint16_t m_serverPort;
  // 服务端socket
  Ptr<Socket> m_serverSocket;
  // 发送的socket
  // Ptr<Socket> m_clientSocket;
  // 所有感知到的事件id
  std::unordered_set<uint32_t> m_detectedEventIds;
  // 调度器事件的id
  EventId m_eventId;
  // 调度事件过期的延迟
  Time m_delay;
  // 车辆的检测范围 todo
  // double m_detectionRange;
  void StartApplication() override;
  void StopApplication() override;
  void CheckNearbyEvents();
  static bool IsWithinRange(const Vector & position, const RandomEvent & randomEvent) ;
  void ScheduleEvent();
  // 发送消息给RSU
  void SentMessageToRSU(const Vector & position, const RandomEvent& randomEvent) const;
  // 处理来自RSU的消息
  void HandleRead(Ptr<Socket> socket);
  // 获取目标RSU
  Ptr<Node> GetTargetRSU() const;
  // 获取目标RSU的网络地址
  static Ipv4Address GetTargetRSUAddress(Ptr<Node> rsu) ;
public:
  // Register this type，包含一些元数据
  static TypeId GetTypeId();
  VehicleApp(uint16_t rsuServerPort, const NodeContainer& rsuNodes, uint16_t m_serverPort, const Time& m_delay);
};
}
#endif //VEHICLE_APP_H
