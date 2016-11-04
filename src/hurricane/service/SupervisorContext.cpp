#include "hurricane/service/SupervisorContext.h"

namespace hurricane {
	namespace service {
		SupervisorContext::SupervisorContext() : _spoutCount(0), _boltCount(0) {
        }

        std::vector<hurricane::base::Variant> SupervisorContext::ToVariants() {
            std::vector<hurricane::base::Variant> variants;

            variants.push_back({ _id });
            variants.push_back({ _spoutCount });
            variants.push_back({ _boltCount });

            hurricane::base::Variants freeSpoutsVariants = hurricane::base::Variant::FromStdSet(_freeSpouts);
            variants.insert(variants.end(), freeSpoutsVariants.begin(), freeSpoutsVariants.end());

            hurricane::base::Variants freeBoltsVariants = hurricane::base::Variant::FromStdSet(_freeBolts);
            variants.insert(variants.end(), freeBoltsVariants.begin(), freeBoltsVariants.end());

            hurricane::base::Variants busySpoutsVariants = hurricane::base::Variant::FromStdSet(_busySpouts);
            variants.insert(variants.end(), busySpoutsVariants.begin(), busySpoutsVariants.end());

            hurricane::base::Variants busyBoltsVariants = hurricane::base::Variant::FromStdSet(_busyBolts);
            variants.insert(variants.end(), busyBoltsVariants.begin(), busyBoltsVariants.end());

            variants.push_back({ static_cast<int>(_taskInfos.size()) });
            for ( hurricane::task::TaskInfo& taskInfo : _taskInfos ) {
                hurricane::base::Variants taskVariants = taskInfo.ToVariants();
                variants.insert(variants.end(), taskVariants.begin(), taskVariants.end());
            }

            return variants;
        }

        void SupervisorContext::ParseVariants(const std::vector<hurricane::base::Variant>& variants) {
            ParseVariants(variants.cbegin());
        }

        std::vector<hurricane::base::Variant>::const_iterator
        SupervisorContext::ParseVariants(std::vector<hurricane::base::Variant>::const_iterator begin) {
            auto currentIterator = begin;

            _id = currentIterator->GetStringValue();
            currentIterator ++;
            _spoutCount = currentIterator->GetIntValue();
            currentIterator ++;
            _boltCount = currentIterator->GetIntValue();
            currentIterator ++;

            int32_t freeSpoutsSize = currentIterator->GetIntValue();
            currentIterator ++;
            _freeSpouts = hurricane::base::Variant::ToStdSet<int>(currentIterator, currentIterator + freeSpoutsSize);
            currentIterator += freeSpoutsSize;

            int32_t freeBoltsSize = currentIterator->GetIntValue();
            currentIterator ++;
            _freeBolts = hurricane::base::Variant::ToStdSet<int>(currentIterator, currentIterator + freeBoltsSize);
            currentIterator += freeBoltsSize;

            int32_t busySpoutsSize = currentIterator->GetIntValue();
            currentIterator ++;
            _busySpouts = hurricane::base::Variant::ToStdSet<int>(currentIterator, currentIterator + busySpoutsSize);
            currentIterator += busySpoutsSize;

            int32_t busyBoltsSize = currentIterator->GetIntValue();
            currentIterator ++;
            _busyBolts = hurricane::base::Variant::ToStdSet<int>(currentIterator, currentIterator + busyBoltsSize);
            currentIterator += busyBoltsSize;

            int taskInfoCount = currentIterator->GetIntValue();
            _taskInfos.resize(taskInfoCount);
            currentIterator ++;
            for ( hurricane::task::TaskInfo& taskInfo : _taskInfos ) {
                currentIterator = taskInfo.ParseVariant(currentIterator);
            }

            return currentIterator;
        }

        SupervisorContext SupervisorContext::FromVariants(std::vector<hurricane::base::Variant>::const_iterator begin) {
            SupervisorContext context;
            context.ParseVariants(begin);

            return context;
        }

        SupervisorContext SupervisorContext::FromVariants(const std::vector<hurricane::base::Variant>& variants) {
            SupervisorContext context;
            context.ParseVariants(variants);

            return context;
        }
	}
}
