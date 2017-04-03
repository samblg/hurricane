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

public class Topology {
    public native void start();
    
    public String[] getSpoutNames() {
        return _spoutNames;
    }
    
    public void setSpoutNames(String[] spoutNames) {
        this._spoutNames = spoutNames;
    }
    
    public Spout[] getSpouts() {
        return _spouts;
    }
    
    public void setSpouts(Spout[] spouts) {
        this._spouts = spouts;
    }
    
    public String[] getBoltNames() {
        return _boltNames;
    }
    
    public void setBoltNames(String[] boltNames) {
        this._boltNames = boltNames;
    }
    
    public String[] getBoltSources() {
        return _boltSources;
    }
    
    public void setBoltSources(String[] boltSources) {
        this._boltSources = boltSources;
    }
    
    public Bolt[] getBolts() {
        return _bolts;
    }
    
    public void setBolts(Bolt[] bolts) {
        this._bolts = bolts;
    }

    private String[] _spoutNames;
    private Spout[] _spouts;
    private String[] _boltNames;
    private String[] _boltSources;
    private Bolt[] _bolts;
}
