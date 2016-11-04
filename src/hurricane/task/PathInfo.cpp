#include "hurricane/task/TaskInfo.h"
#include "hurricane/base/Variant.h"

namespace hurricane {
namespace task {

std::vector<hurricane::base::Variant> PathInfo::ToVariants() const {
    std::vector<hurricane::base::Variant> variants;

    variants.push_back({ _groupMethod });
    variants.push_back({ _destinationSupervisor.GetHost() });
    variants.push_back({ _destinationSupervisor.GetPort() });
    variants.push_back({ _destinationExecutorIndex });

    return variants;
}

void PathInfo::ParseVariant(const std::vector<hurricane::base::Variant>& variants) {
    ParseVariant(variants.cbegin());
}

std::vector<hurricane::base::Variant>::const_iterator
    PathInfo::ParseVariant(std::vector<hurricane::base::Variant>::const_iterator begin)
{
    auto currentIterator = begin;

    _groupMethod = currentIterator->GetIntValue();
    ++ currentIterator;

    std::string host = currentIterator->GetStringValue();
    ++ currentIterator;

    int port = currentIterator->GetIntValue();
    ++ currentIterator;

    _destinationSupervisor.SetHost(host);
    _destinationSupervisor.SetPort(port);

    _destinationExecutorIndex = currentIterator->GetIntValue();
    ++ currentIterator;

    return currentIterator;
}

PathInfo PathInfo::FromVariants(const std::vector<hurricane::base::Variant>& variants) {
    PathInfo pathInfo;
    pathInfo.ParseVariant(variants);

    return pathInfo;
}

PathInfo PathInfo::FromVariants(std::vector<hurricane::base::Variant>::const_iterator begin)
{
    PathInfo pathInfo;
    pathInfo.ParseVariant(begin);

    return pathInfo;
}

}
}
