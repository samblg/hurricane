#include "hurricane/base/Constants.h"

namespace hurricane {

std::string NodeType::President = "president";
std::string NodeType::Manager = "manager";

std::string ConfigurationKey::PresidentHost = "president.host";
std::string ConfigurationKey::PresidentPort = "president.port";
std::string ConfigurationKey::ManagerCount = "president.manager.count";
std::string ConfigurationKey::ManagerName = "manager.name";
std::string ConfigurationKey::ManagerHost = "manager.host";
std::string ConfigurationKey::ManagerPort = "manager.port";
std::string ConfigurationKey::TopologyName = "topology.name";
std::string ConfigurationKey::SpoutCount = "manager.spout.num";
std::string ConfigurationKey::BoltCount = "manager.bolt.num";
std::string ConfigurationKey::SpoutFlowParam = "spout.flow.param";
}
