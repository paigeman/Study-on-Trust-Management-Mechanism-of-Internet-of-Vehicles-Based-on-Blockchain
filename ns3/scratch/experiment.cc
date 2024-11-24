#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ns2-mobility-helper.h"
#include "filesystem"

#include <iostream>

using namespace ns3;

static void
CourseChange(std::ostream* os, std::string foo, Ptr<const MobilityModel> mobility)
{
    // 在输出要使用的信息之前，会有一批红色的信息输出，这些红色的信息是在ns2.Install()之后生成的
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

    }
}

// Example to use ns2 traces file in ns3
int
main(int argc, char* argv[])
{
    std::string traceFile;
    std::string configFile;

    int nodeNum;
    double duration;

    // Enable logging from the ns2 helper
    LogComponentEnable("Ns2MobilityHelper", LOG_LEVEL_DEBUG);

    // Parse command line attribute
    CommandLine cmd(__FILE__);
    cmd.AddValue("traceFile", "Ns2 movement trace file", traceFile);
    cmd.AddValue("nodeNum", "Number of nodes", nodeNum);
    cmd.AddValue("duration", "Duration of Simulation", duration);
    cmd.AddValue("configFile", "Configuration file", configFile);
    cmd.Parse(argc, argv);

    // Check command line arguments
    if (traceFile.empty() || nodeNum <= 0 || duration <= 0 || configFile.empty())
    {
        std::cout << "Usage of " << argv[0]
                  << " :\n\n"
                     "./ns3 run \"ns2-mobility-trace"
                     " --traceFile=src/mobility/examples/default.ns_movements"
                     " --configFile=/path/to/your/config"
                     " --nodeNum=2 --duration=100.0\" \n\n"
                     "NOTE: ns2-traces-file could be an absolute or relative path. You could use "
                     "the file default.ns_movements\n"
                     "      included in the same directory of this example file.\n\n"
                     "NOTE 1: Number of nodes present in the trace file must match with the "
                     "command line argument and must\n"
                     "        be a positive number. Note that you must know it before to be able "
                     "to load it.\n\n"
                     "NOTE 2: Duration must be a positive number. Note that you must know it "
                     "before to be able to load it.\n\n";

        return 0;
    }

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
    Ns2MobilityHelper ns2 = Ns2MobilityHelper(traceFile);

    // Must add the following two lines code
    // Create all nodes.
    NodeContainer stas;
    stas.Create(nodeNum);

    ns2.Install(); // configure movements for each node, while reading trace file

    // Configure callback for logging
    Config::Connect("/NodeList/*/$ns3::MobilityModel/CourseChange",
                    MakeBoundCallback(&CourseChange, &os));

    Simulator::Stop(Seconds(duration));
    Simulator::Run();
    Simulator::Destroy();

    os.close(); // close log file
}
