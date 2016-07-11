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

#include "hurricane/multilang/c/HCTopology.h"
#include "hurricane/multilang/java/org_hurricane_hurricane_Topology.h"

JNIEXPORT void JNICALL Java_org_hurricane_hurricane_Topology_start
  (JNIEnv * env, jobject obj) {
      jclass clazzTopology = env->GetObjectClass(obj);

      jmethodID getSpoutNames = env->GetMethodID(clazzTopology, "getSpoutNames");
      jmethodID getSpouts = env->GetMethodID(clazzTopology, "getSpouts");
      jmethodID getBoltNames = env->GetMethodID(clazzTopology, "getBoltNames");
      jmethodID getBolts = env->GetMethodID(clazzTopology, "getBolts");
      jmethodID getBoltSources = env->GetMethodID(clazzTopology, "getBoltSources");

      jarray jSpoutNames = env->CallArrayMethod(obj, getSpoutNames);
      jint jSpoutCount = env->GetIntField(jspoutNames, "length");
      jarray jSpouts = env->CallArrayMethod(obj, getSpouts);
      jarray jBoltNames = env->CallArrayMethod(obj, getBoltNames);
      jarray jBolts = env->CallArrayMethod(obj, getBolts);
      jint jBoltCount = env->GetIntField(jBolts, "length");
      jarray jBoltSources = env->CallArrayMethod(obj, getBoltSources);

      CTopology cTopology;
      topology.spoutCount = jSpoutCount;
      topology.cSpoutNames = new char*[jSpoutCount];
      topology.cSpouts = new CSpout[jSpoutCount];
      topology.boltCount = jBoltCount;
      topology.cBoltNames = new char*[jBoltCount];
      topology.cBolts = new CSpout[jBoltCount];
      topology.cBoltSources = new char*[jBoltCount];

      CTopologyWrapper topologyWrapper(&cTopology);
      topologyWrapper.Start();
}
