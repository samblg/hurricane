#pragma once

#include <string>

namespace hurricane {

class NodeType {
public:
    static std::string President;
    static std::string Manager;
};

class ConfigurationKey {
public:
    static std::string PresidentHost;
    static std::string PresidentPort;
    static std::string ManagerName;
    static std::string ManagerHost;
    static std::string ManagerPort;
    static std::string ManagerCount;
    static std::string TopologyName;
    static std::string SpoutCount;
    static std::string BoltCount;
    static std::string SpoutFlowParam;
};
}
