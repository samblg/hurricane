#pragma once

#include "hurricane/task/TaskInfo.h"

#include <string>
#include <set>
#include <vector>

namespace hurricane {
	namespace service {
        class SupervisorContext : base::Serializable {
        public:
            enum class ExecutorType {
                Spout,
                Bolt
            };

			SupervisorContext();

			const std::string& GetId() const {
				return _id;
			}

			void SetId(const std::string& id) {
				_id = id;
			}

            const hurricane::base::NetAddress& GetNetAddress() const {
                return _netAddress;
            }

            void SetNetAddress(const hurricane::base::NetAddress& netAddress) {
                _netAddress = netAddress;
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

            int useNextSpout() {
                if ( !_freeSpouts.size() ) {
                    return -1;
                }

                int spoutIndex = *(_freeSpouts.begin());
                useSpout(spoutIndex);

                return spoutIndex;
            }

			void freeSpout(int spoutIndex) {
				_freeSpouts.insert(spoutIndex);
				_busySpouts.erase(spoutIndex);
			}

			void useBolt(int boltIndex) {
				_freeBolts.erase(boltIndex);
				_busyBolts.insert(boltIndex);
			}

            int useNextBolt() {
                if ( !_freeBolts.size() ) {
                    return -1;
                }

                int boltIndex = *(_freeBolts.begin());
                useBolt(boltIndex);

                return boltIndex;
            }

			void freeBolt(int boltIndex) {
				_freeBolts.insert(boltIndex);
				_busyBolts.erase(boltIndex);
			}

            void PrepareTaskInfos() {
                _taskInfos.resize(_spoutCount + _boltCount);
            }

            std::vector<hurricane::task::TaskInfo>& GetTaskInfos() {
                return _taskInfos;
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

            const hurricane::task::TaskInfo& GetSpoutTaskInfo(int index) const {
                return _taskInfos[index];
            }

            const hurricane::task::TaskInfo& GetBoltTaskInfo(int index) const {
                return _taskInfos[_spoutCount + index];
            }

            hurricane::task::TaskInfo& GetTaskInfo(int index) {
                return _taskInfos[index];
            }

            hurricane::task::TaskInfo& GetSpoutTaskInfo(int index) {
                return _taskInfos[index];
            }

            hurricane::task::TaskInfo& GetBoltTaskInfo(int index) {
                return _taskInfos[_spoutCount + index];
            }

            void SetTaskInfo(int index, const hurricane::task::TaskInfo& info) {
				_taskInfos[index] = info;
			}

            int SetSpoutTaskInfo(int index, const hurricane::task::TaskInfo& info) {
                _taskInfos[index] = info;

                return index;
            }

            int SetBoltTaskInfo(int index, const hurricane::task::TaskInfo& info) {
                _taskInfos[_spoutCount + index] = info;

                return _spoutCount + index;
            }

            int GetExecutorIndex(ExecutorType type, int subIndex) {
                if ( type == ExecutorType::Spout ) {
                    return subIndex;
                }

                return _spoutCount + subIndex;
            }

            virtual void Serialize(hurricane::base::Variants& variants) const override;
            virtual void Deserialize(hurricane::base::Variants::const_iterator& it) override;

		private:
			std::string _id;
            hurricane::base::NetAddress _netAddress;

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
