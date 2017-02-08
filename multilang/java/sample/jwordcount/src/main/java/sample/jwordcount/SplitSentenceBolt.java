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

import hurricane.jni.IBolt;
import hurricane.jni.OutputCollector;

public class SplitSentenceBolt implements IBolt {
	public void prepare(OutputCollector collector) {
		this.collector = collector;
	}

	public void cleanup() {
	}

	public void execute(Object[] tuple) {
		String sentence = (String)tuple[0];
		String[] words = sentence.split(" ");
		
		for ( String word : words ) {
			collector.emit(new Object[] {
					word
			});
		}
	}

	public String[] declareFields() {
		return new String[] {
				"word"
		};
	}

	private OutputCollector collector;
}
