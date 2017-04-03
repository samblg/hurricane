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

public class TopologyBuilder {
    public void setSpout(String name, Spout spout) {
        _spoutNames.add(name);
        _spouts.add(spout);
    }
    
    public void setBolt(String name, Bolt bolt, String source) {
        _boltNames.add(name);
        _bolts.add(bolt);
        _boltSources.add(source);
    }
    
    public Topology buildTopology() {
        Topology topology = new Topology();
        String[] stringArray = {};
        Spout[] spoutArray = {};
        Bolt[] boltArray= {};
        
        topology.setSpoutNames(_spoutNames.toArray(stringArray));
        topology.setSpouts(_spouts.toArray(spoutArray));
        topology.setBoltNames(_boltNames.toArray(stringArray));
        topology.setBoltSources(_boltSources.toArray(stringArray));
        topology.setBolts(_bolts.toArray(boltArray));
        
        return topology;
    }
    
    public List<String> getSpoutNames() {
        return _spoutNames;
    }
    
    public void setSpoutNames(List<String> spoutNames) {
        this._spoutNames = spoutNames;
    }
    
    public List<Spout> getSpouts() {
        return _spouts;
    }
    
    public void setSpouts(List<Spout> spouts) {
        this._spouts = spouts;
    }
    
    public List<String> getBoltNames() {
        return _boltNames;
    }
    
    public void setBoltNames(List<String> boltNames) {
        this._boltNames = boltNames;
    }
    
    public List<String> getBoltSources() {
        return _boltSources;
    }
    
    public void setBoltSources(List<String> boltSources) {
        this._boltSources = boltSources;
    }
    
    public List<Bolt> getBolts() {
        return _bolts;
    }
    
    public void setBolts(List<Bolt> bolts) {
        this._bolts = bolts;
    }

    private List<String> _spoutNames = new ArrayList<String>();
    private List<Spout> _spouts = new ArrayList<Spout>();
    private List<String> _boltNames = new ArrayList<String>();
    private List<String> _boltSources = new ArrayList<String>();
    private List<Bolt> _bolts = new ArrayList<Bolt>();
}
