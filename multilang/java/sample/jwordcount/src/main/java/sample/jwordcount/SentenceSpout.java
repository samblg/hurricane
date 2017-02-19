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

package sample.jwordcount;

import hurricane.jni.ISpout;
import hurricane.jni.OutputCollector;

public class SentenceSpout implements ISpout {
	public SentenceSpout() {
		
	}
	
	public void prepare(OutputCollector collector) {
		this._collector = collector;
	}
	
	public void cleanup() {
	}
	
	public void nextTuple() {
		Object[] args = new Object[] {
				"hello world"
		};
		
		this._collector.emit(args);
	}
	
	public String[] declareFields() {
		return new String[]{ "sentence" };
	}
	
	private OutputCollector _collector; 
	
	static {
		System.load("/home/kinuxroot/Projects/DataEE/hurricane-github/target/linux/x64/Release/run/libjwordcount.so");
	}
}