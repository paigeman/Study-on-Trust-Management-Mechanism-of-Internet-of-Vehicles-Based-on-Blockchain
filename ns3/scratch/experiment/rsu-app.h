//
// Created by fade on 2024/12/7.
//

#ifndef RSU_APP_H
#define RSU_APP_H
#include "ns3/application.h"

namespace ns3
{
// RSU应用
class RsuApp : public Application
{
  void StartApplication() override;
  void StopApplication() override;
public:
  // Register this type，包含一些元数据
  static TypeId GetTypeId();
};
}
#endif //RSU_APP_H
