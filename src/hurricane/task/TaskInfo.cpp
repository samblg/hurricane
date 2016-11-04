#include "hurricane/task/TaskInfo.h"

namespace hurricane {
namespace task {

std::vector<base::Variant> TaskInfo::ToVariants() const
{
    std::vector<hurricane::base::Variant> variants;

    variants.push_back({ _topologyName });
    variants.push_back({ _taskName });
    variants.push_back({ _executorIndex });

    variants.push_back({ static_cast<int>(_paths.size()) });
    for ( const PathInfo& pathInfo : _paths ) {
        std::vector<hurricane::base::Variant> pathVariants = pathInfo.ToVariants();

        variants.insert(variants.end(), pathVariants.begin(), pathVariants.end());
    }

    return variants;
}

void TaskInfo::ParseVariant(const std::vector<base::Variant>& variants)
{
    ParseVariant(variants.cbegin());
}

std::vector<base::Variant>::const_iterator
    TaskInfo::ParseVariant(std::vector<base::Variant>::const_iterator begin)
{
    auto currentIterator = begin;

    _topologyName = currentIterator->GetStringValue();
    ++ currentIterator;

    _taskName = currentIterator->GetStringValue();
    ++ currentIterator;

    _executorIndex = currentIterator->GetIntValue();
    ++ currentIterator;

    int pathCount = currentIterator->GetIntValue();
    ++ currentIterator;

    _paths.clear();
    for ( int pathIndex = 0; pathIndex != pathCount; ++ pathIndex ) {
        PathInfo pathInfo;

        currentIterator = pathInfo.ParseVariant(currentIterator);
        _paths.push_back(pathInfo);
    }

    return currentIterator;
}

TaskInfo TaskInfo::FromVariants(const std::vector<base::Variant>& variants)
{
    return FromVariants(variants.cbegin());
}

TaskInfo TaskInfo::FromVariants(std::vector<base::Variant>::const_iterator begin)
{
    TaskInfo taskInfo;
    taskInfo.ParseVariant(begin);

    return taskInfo;
}

}
}
