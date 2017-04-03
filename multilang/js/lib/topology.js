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

'use strict'

let clib = require('./clib');

class Topology {
    constructor() {
        this.spoutDeclares = [];
        this.boltDeclares = [];
    }

    setSpout(spoutName, spout) {
        this.spoutDeclares.push({
            name: spoutName,
            spout: {
                onClone: spout.clone,
                onOpen: spout.open,
                onClose: spout.close,
                onExecute: bolt.rawExecute
            }
        });
    }

    setBolt(boltName, bolt, source) {
        this.boltDeclares.push({
            name: boltName,
            bolt: {
                onClone: bolt.clone,
                onPrepare: bolt.prepare,
                onCleanup: bolt.cleanup,
                onExecute: bolt.rawExecute
            },
            source: source
        });
    }

    start() {
        let topology = {
            spoutCount: this.spoutDeclares.length,
            cSpouts: this.spoutDeclares.map(declare => declare.spout),
            spoutNames: this.spoutDeclares.map(declare => declare.name),
            boltCount: this.boltDeclares.length,
            cBolts: this.boltDeclares.map(declare => declare.bolt),
            boltNames: this.boltDeclares.map(declare => declare.name),
            boltSources: this.boltDeclares.map(declare => declare.source)
        };

        clib.StartToplogy(topology);
    }
}

module.exports = {
    Topology
}
