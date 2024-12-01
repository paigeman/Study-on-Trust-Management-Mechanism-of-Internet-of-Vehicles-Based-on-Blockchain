#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ns2-mobility-helper.h"
#include "filesystem"

#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("experiment");

static void
CourseChange(std::ostream* os, std::string foo, Ptr<const MobilityModel> mobility)
{
    // 在输出要使用的信息之前，会有一批红色的信息输出，这些红色的信息是在ns2.Install()之后生成的
    // 注释掉LogComponentEnable("Ns2MobilityHelper", LOG_LEVEL_DEBUG);即可去除红色输出
    Vector pos = mobility->GetPosition(); // Get position
    Vector vel = mobility->GetVelocity(); // Get velocity

    // Prints position and velocities
    std::cout << Simulator::Now() << " POS: x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z
        << "; VEL:" << vel.x << ", y=" << vel.y << ", z=" << vel.z << std::endl;
    *os << Simulator::Now() << " POS: x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z
        << "; VEL:" << vel.x << ", y=" << vel.y << ", z=" << vel.z << std::endl;
}

// 解析配置文件
static void ParseConfigFile(const std::string &fileName, std::unordered_map<std::string, std::string> &configMap)
{
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        // 忽略注释
        size_t pos_sharp = line.find_first_of('#');
        if (pos_sharp != std::string::npos)
        {
            line = line.substr(0, pos_sharp);
        }
        // 去除行首和行尾的空白字符
        while (!line.empty() && isblank(line[0]))
        {
            line.erase(0, 1); // Removes blank spaces at the beginning of the line
        }
        while (!line.empty() && isblank(line[line.size() - 1]))
        {
            line.erase(line.size() - 1, 1); // Removes blank spaces from at end of line
        }
        std::istringstream iss(line);
        std::string keyword;
        std::string key;
        std::string value;
        iss >> keyword;
        // 如果不是set打头直接跳过
        if (keyword != "set") {
            continue;
        }
        iss >> key >> value;
        configMap[key] = value;
    }
    file.close();
}

// OutputRsuLocation 输出RSU位置
static void OutputRsuLocation(const NodeContainer & rsuNodes)
{
    for (uint32_t i = 0; i < rsuNodes.GetN(); ++i)
    {
        Ptr<MobilityModel> mobility = rsuNodes.Get(i)->GetObject<MobilityModel>();
        const Vector pos = mobility->GetPosition();
        std::ostringstream oss;
        oss << "RSU " << i << " position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
        NS_LOG_DEBUG(oss.str());
    }
}

int
main(int argc, char* argv[])
{
    std::string traceFile;
    std::string configFile;

    uint32_t nodeNum;
    int rsuNum;
    double duration;

    // Enable logging from the ns2 helper
    // LogComponentEnable("Ns2MobilityHelper", LOG_LEVEL_DEBUG);

    // Parse command line attribute
    CommandLine cmd(__FILE__);
    cmd.AddValue("traceFile", "Ns2 movement trace file", traceFile);
    cmd.AddValue("configFile", "Configuration file", configFile);
    cmd.AddValue("rsuNum", "Number of RSUs", rsuNum);
    cmd.Parse(argc, argv);

    // Check command line arguments
    if (traceFile.empty() || configFile.empty() || rsuNum <= 0)
    {
        std::cout << "Usage of " << argv[0]
                  << " :\n\n"
                     "./ns3 run \"ns2-mobility-trace"
                     " --traceFile=src/mobility/examples/default.ns_movements"
                     " --rsuNum=2"
                     " --configFile=/path/to/your/config\" \n\n"
                     "NOTE: ns2-traces-file could be an absolute or relative path. You could use "
                     "the file default.ns_movements\n"
                     "      included in the same directory of this example file.\n\n"
                     "NOTE 1: Number of RSUs must\n"
                     "        be a positive number.\n\n";
        return 0;
    }

    // 解析配置文件
    std::unordered_map<std::string, std::string> configMap;
    ParseConfigFile(configFile, configMap);
    // 设置节点数量
    nodeNum = std::stoi(configMap["opt(nn)"]);
    // 设置duration
    duration = std::stod(configMap["opt(stop)"]);

    // 生成日志名
    std::filesystem::path logFile = traceFile;
    logFile = logFile.filename();
    if (logFile.has_extension())
    {
        logFile.replace_extension(".log");
    }

    // open log file for output
    std::ofstream os;
    os.open(logFile);

    // Create Ns2MobilityHelper with the specified trace log file as parameter
    auto ns2 = Ns2MobilityHelper(traceFile);

    // Must add the following two lines code
    // Create all vehicle nodes.
    NodeContainer stas;
    stas.Create(nodeNum);

    // 配置RSU的移动模型
    double minX = std::stod(configMap["opt(min-x)"]);
    double minY = std::stod(configMap["opt(min-y)"]);
    double maxX = std::stod(configMap["opt(x)"]);
    double maxY = std::stod(configMap["opt(y)"]);
    double width = maxX - minX;
    double height = maxY - minY;

    // 这一块可以替换其它方案
    // ratio 可以使网格布局按区域比例调整
    double ratio = width / height;
    // gridWidth 有几列
    auto gridWidth = static_cast<uint32_t>(std::ceil(std::sqrt(rsuNum * ratio)));
    // gridHeight 有几行
    auto gridHeight = static_cast<uint32_t>(std::ceil(static_cast<double>(rsuNum) / gridWidth));
    // 根据用户提供的RSU数重新计算RSU数
    rsuNum = gridWidth * gridHeight;
    // 创建RSU节点
    NodeContainer rsuNodes;
    rsuNodes.Create(rsuNum);

    double deltaX = width / gridWidth;
    double deltaY = height / gridHeight;

    // 自定义位置分配器，确保每个 RSU 位于其小矩形中心，如此一辆车只要位于一个区域内，它只能与该区域内的RSU进行交互
    Ptr<ListPositionAllocator> positionAllocator = CreateObject<ListPositionAllocator>();
    for (size_t i = 0; i < gridHeight; ++i)
    {
        for (size_t j = 0; j < gridWidth; ++j)
        {
            double posX = minX + deltaX * (j + 0.5);
            double posY = minY + deltaY * (i + 0.5);
            positionAllocator->Add(Vector(posX, posY, 0.0));
        }
    }
    MobilityHelper mobility;
    mobility.SetPositionAllocator(positionAllocator);
    // RSU的mobility model配置为静态的
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(rsuNodes);
    OutputRsuLocation(rsuNodes);

    ns2.Install(); // configure movements for each node, while reading trace file

    // Configure callback for logging
    Config::Connect("/NodeList/*/$ns3::MobilityModel/CourseChange",
                    MakeBoundCallback(&CourseChange, &os));

    Simulator::Stop(Seconds(duration));
    Simulator::Run();
    Simulator::Destroy();

    os.close(); // close log file
}
