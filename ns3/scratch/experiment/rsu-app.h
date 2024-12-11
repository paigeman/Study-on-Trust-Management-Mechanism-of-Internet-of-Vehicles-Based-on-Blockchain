//
// Created by fade on 2024/12/7.
//

#ifndef RSU_APP_H
#define RSU_APP_H
#include "ns3/application.h"
#include "ns3/socket.h"

namespace ns3
{
// RSU应用
class RsuApp : public Application
{
  // 服务端socket
  Ptr<Socket> m_serverSocket;
  // 服务端port
  uint16_t m_serverPort;
  void StartApplication() override;
  void StopApplication() override;
  void HandleRead(Ptr<Socket> socket);
public:
  // Register this type，包含一些元数据
  static TypeId GetTypeId();
  RsuApp(uint16_t serverPort);
};
}
#endif //RSU_APP_H
