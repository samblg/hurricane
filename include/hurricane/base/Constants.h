/**
 * licensed to the apache software foundation (asf) under one
 * or more contributor license agreements.  see the notice file
 * distributed with this work for additional information
 * regarding copyright ownership.  the asf licenses this file
 * to you under the apache license, version 2.0 (the
 * "license"); you may not use this file except in compliance
 * with the license.  you may obtain a copy of the license at
 *
 * http://www.apache.org/licenses/license-2.0
 *
 * unless required by applicable law or agreed to in writing, software
 * distributed under the license is distributed on an "as is" basis,
 * without warranties or conditions of any kind, either express or implied.
 * see the license for the specific language governing permissions and
 * limitations under the license.
 */

#pragma once

#include <string>

namespace hurricane {

const std::string NODE_PRESIDENT = "president";
const std::string NODE_MANAGER = "manager";

const std::string CONF_KEY_PRESIDENT_HOST = "president.host";
const std::string CONF_KEY_PRESIDENT_PORT = "president.port";
const std::string CONF_KEY_MANAGER_COUNT = "president.manager.count";
const std::string CONF_KEY_MANAGER_NAME = "manager.name";
const std::string CONF_KEY_MANAGER_HOST = "manager.host";
const std::string CONF_KEY_MANAGER_PORT = "manager.port";
const std::string CONF_KEY_TOPOLOGY_NAME = "topology.name";
const std::string CONF_KEY_SPOUT_COUNT = "manager.spout.num";
const std::string CONF_KEY_BOLT_COUNT = "manager.bolt.num";
const std::string CONF_KEY_SPOUT_FLOW_PARAM = "spout.flow.param";
const std::string CONF_KEY_ENABLE_JAVA = "java.enable";
const std::string CONF_KEY_JAVA_CLASSPATH = "java.classpath";

}
