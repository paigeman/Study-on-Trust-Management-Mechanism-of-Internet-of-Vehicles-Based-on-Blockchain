//
// Created by fade on 2024/12/7.
//
#include "vehicle-app.h"
using namespace ns3;

TypeId VehicleApp::GetTypeId()
{
    static TypeId tid = TypeId("ns3::VehicleApp")
        .SetParent<Application>().SetGroupName("experiment");
    return tid;
}

void VehicleApp::StartApplication()
{

}

void VehicleApp::StopApplication()
{

}
