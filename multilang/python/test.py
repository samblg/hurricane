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

from ctypes import *
from PyHurricane import Spout, Bolt, Topology

class WordSpout(Spout):
    def clone(self):
        return WordSpout()

    def open(self):
        print 'open'

    def close(self):
        print 'close'

    def execute(self):
        print 'execute'
        self.emit([1, 2, 'hello'])

class WordSplitBolt(Bolt):
    def clone(self):
        return WordSplitBolt()

    def prepare(self):
        print 'prepare'

    def cleanup(self):
        print 'cleaup'

    def execute(self, values):
        print 'execute',
        print values

        self.emit(values[0].split(' '))

topology = Topology()
topology.setSpout('textSpout', WordSpout())
topology.setBolt('wordSplitBolt', WordSplitBolt(), 'textSpout')
topology.start()
