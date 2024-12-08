//
// Created by fade on 2024/12/7.
//
#include "rsu-app.h"
using namespace ns3;

TypeId RsuApp::GetTypeId()
{
    static TypeId tid = TypeId("ns3::RsuApp")
        .SetParent<Application>().SetGroupName("experiment");
    return tid;
}

void RsuApp::StartApplication()
{

}

void RsuApp::StopApplication()
{

}
