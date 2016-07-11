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

package org.hurricane.hurricane;

import java.util.ArrayList;
import java.util.List;

public class Sample {
    public static class TextSpout extends Spout {

        @Override
        public void open() {
        }

        @Override
        public void close() {
        }

        @Override
        public void execute() {
            List<Object> values = new ArrayList<Object>();
            values.add("Hello World");
            this.emit(values);
        }

        @Override
        public Spout clone() {
            return new TextSpout();
        }

        @Override
        public List<String> getFields() {
            List<String> fields = new ArrayList<String>();
            fields.add("text");
            
            return fields;
        }
        
    }
    
    public static class WordSplitBolt extends Bolt {

        @Override
        public void prepare() {
        }

        @Override
        public void cleanup() {
        }

        @Override
        public void execute(List<Object> values) {
            String text = (String)values.get(0);
            String[] words = text.split(" ");
            
            for ( String word : words ) {
                List<Object> wordValues = new ArrayList<Object>();
                wordValues.add(word);
                this.emit(values);
            }
        }

        @Override
        public Bolt clone() {
            return new WordSplitBolt();
        }

        @Override
        public List<String> getFields() {
            List<String> fields = new ArrayList<String>();
            fields.add("word");
            
            return fields;
        }
        
    }
    
    public static void main(String[] args) {
        TopologyBuilder builder = new TopologyBuilder();
        builder.setSpout("textSpout", new TextSpout());
        builder.setBolt("wordSplitBolt", new WordSplitBolt(), "textSpout");
        
        Topology topology = builder.buildTopology();
        topology.start();
    }
}
