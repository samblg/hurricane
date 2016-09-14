#pragma once

#include "hurricane/task/TaskInfo.h"
#include <string>
#include <set>
#include <vector>

namespace hurricane {
	namespace service {
		class SupervisorContext {
		public:
			SupervisorContext();

			const std::string& GetId() const {
				return _id;
			}

			void SetId(const std::string& id) {
				_id = id;
			}

			int GetSpoutCount() const {
				return _spoutCount;
			}

			void SetSpoutCount(int spoutCount) {
				_spoutCount = spoutCount;
			}

			int GetBoltCount() const {
				return _boltCount;
			}

			void SetBoltCount(int boltCount) {
				_boltCount = boltCount;
			}

			const std::set<int>& GetFreeSpouts() const {
				return _freeSpouts;
			}

            void SetFreeSpouts(const std::set<int>& freeSpouts) {
                _freeSpouts = freeSpouts;
            }

			const std::set<int>& GetBusySpouts() const {
				return _busySpouts;
			}

            void SetBusySpouts(const std::set<int>& busySpouts) {
                _busySpouts = busySpouts;
            }

			const std::set<int>& GetFreeBolts() const {
				return _freeBolts;
            }

            void SetFreeBolts(const std::set<int>& freeBolts) {
                _freeBolts = freeBolts;
            }

			const std::set<int>& GetBusyBolts() const {
				return _busyBolts;
            }

            void SetBusyBolts(const std::set<int>& busyBolts) {
                _busyBolts = busyBolts;
            }

			void useSpout(int spoutIndex) {
				_freeSpouts.erase(spoutIndex);
				_busySpouts.insert(spoutIndex);
			}

			void freeSpout(int spoutIndex) {
				_freeSpouts.insert(spoutIndex);
				_busySpouts.erase(spoutIndex);
			}

			void useBolt(int boltIndex) {
				_freeBolts.erase(boltIndex);
				_busyBolts.insert(boltIndex);
			}

			void freeBolt(int boltIndex) {
				_freeBolts.insert(boltIndex);
				_busyBolts.erase(boltIndex);
			}

			const std::vector<hurricane::task::TaskInfo>& GetTaskInfos() const {
				return _taskInfos;
			}

			void SetTaskInfos(const std::vector<hurricane::task::TaskInfo>& taskInfos) {
				_taskInfos = taskInfos;
			}

			const hurricane::task::TaskInfo& GetTaskInfo(int index) const {
				return _taskInfos[index];
			}

			void SetTaskInfo(int index, const hurricane::task::TaskInfo& info) {
				_taskInfos[index] = info;
			}

            std::vector<hurricane::base::Variant> ToVariants();
            void ParseVariants(const std::vector<hurricane::base::Variant>& variants);
            void ParseVariants(std::vector<hurricane::base::Variant>::const_iterator begin);
            static SupervisorContext FromVariants(const std::vector<hurricane::base::Variant>& variants);
            static SupervisorContext FromVariants(std::vector<hurricane::base::Variant>::const_iterator begin);

		private:
			std::string _id;
			int _spoutCount;
			int _boltCount;
			
			std::set<int> _freeSpouts;
			std::set<int> _freeBolts;

			std::set<int> _busySpouts;
			std::set<int> _busyBolts;

			std::vector<hurricane::task::TaskInfo> _taskInfos;
		};
	}
}