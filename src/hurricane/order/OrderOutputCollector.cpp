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

#include "hurricane/order/OrderOutputCollector.h"
#include "hurricane/collector/OutputCollector.h"
#include "hurricane/service/Manager.h"
#include "hurricane/message/CommandClient.h"
#include "hurricane/util/NetConnector.h"

namespace hurricane {
namespace order {

OrderOutputCollector::OrderOutputCollector(std::shared_ptr<collector::OutputCollector> collector) :
        _collector(collector){
}

void OrderOutputCollector::Emit(const OrderTuple& tuple)
{
    service::Manager* manager = service::Manager::GetGlobalInstance();

    message::CommandClient* commandClient = manager->CreatePresidentClient();

    if ( tuple.GetOrderId() == 0 ) {
        commandClient->GetConnector()->Connect([this, manager, commandClient, tuple]
        (const util::SocketError& socketError) {
            if ( socketError.GetType() != util::SocketError::Type::NoError ) {
                LOG(LOG_ERROR) << socketError.what();
                return;
            }

            hurricane::message::Command command(hurricane::message::Command::Type::OrderId);
            command.AddArgument({ manager->GetTopologyName() });

            commandClient->SendCommand(command,
                [this, manager, tuple](const hurricane::message::Response& response,
                        const message::CommandError& error) -> void {
                if ( error.GetType() != message::CommandError::Type::NoError ) {
                    LOG(LOG_ERROR) << error.what();
                    return;
                }

                if ( response.GetStatus() == hurricane::message::Response::Status::Successful ) {
                    int64_t orderId = response.GetArgument(0).GetInt64Value();
                    OrderTuple newTuple = tuple;
                    newTuple.SetOrderId(orderId);
                    _collector->Emit(newTuple.ToBaseTuple());
                }
                else {
                    LOG(LOG_ERROR) << "Get new order id of " << manager->GetTopologyName() << " failed.";
                }
            });
        });
    }
    else {
        _collector->Emit(tuple.ToBaseTuple());
    }

}


}
}
