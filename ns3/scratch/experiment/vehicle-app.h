//
// Created by fade on 2024/12/7.
//

#ifndef VEHICLE_APP_H
#define VEHICLE_APP_H
#include "ns3/application.h"

namespace ns3
{
// 车辆应用
class VehicleApp : public Application
{
  // 车辆的检测范围
  double m_detectionRange;
  void StartApplication() override;
  void StopApplication() override;
public:
  // Register this type，包含一些元数据
  static TypeId GetTypeId();
};
}
#endif //VEHICLE_APP_H
