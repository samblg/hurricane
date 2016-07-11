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

'use strict';

let hurricane = require('./index.js');
let Spout = hurricane.spout.Spout;
let Bolt = hurricane.spout.Bolt;
let Topology = hurricane.topology.Topology;

class TextSpout extends Spout {
    clone() {
        return new TextSpout();
    }

    open() {
    }

    close() {
    }

    execute() {
        this.emit([
                'hello world'
        ]);
    }

    getFields() {
        return [ 'text' ];
    }
}

class WordSplitBolt extends Bolt {
    clone() {
        return new TextSpout();
    }

    prepare() {
    }

    cleanup() {
    }

    execute(values) {
        let self = this;
        let text = values[0];
        let words = text.split(' ');

        words.forEach(word => {
            self.emit(word);
        });
    }

    getFields() {
        return [ 'word' ];
    }
}

let topology = new Topology();
topology.setSpout('textSpout', new TextSpout());
topology.setBolt('wordSplitBolt', new WordSplitBolt());
topology.start();
