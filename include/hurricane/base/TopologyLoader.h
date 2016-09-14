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

#pragma once

#ifdef OS_WIN32
#include <Windows.h>
#elif ( defined(OS_LINUX) )
#include <dlfcn.h>
#endif

#include "hurricane/topology/ITopology.h"

using hurricane::topology::ITopology;

namespace hurricane {
    namespace base {
#ifdef OS_WIN32
        ITopology* LoadTopolgoy(const std::string& fileName) {
            HINSTANCE hInstance = LoadLibrary(fileName.c_str());

            typedef ITopology* (TopologyGetter)();
            
            TopologyGetter GetTopology = (TopologyGetter)GetProcAddress(
                hInstance, "GetTopology");

            ITopology* topology = GetTopology();

            return topology;
        }

#elif ( defined(OS_LINUX) )
        ITopology* LoadTopolgoy(const std::string& fileName) {
            void * libm_handle = NULL;
            typedef ITopology* (TopologyGetter)();
            char *errorInfo;
            float result;

            // dlopen ���������Զ������������е������������������������һ�������������������Ķ��������ͻ��Զ��������ǡ�
            // ��������һ���������þ������ں����� API ����
            libm_handle = dlopen(fileName.c_str(), RTLD_LAZY);
            // �������� NULL ��������ʾ�޷��ҵ������ļ������̽����������Ļ��������õ�������һ�����������Խ�һ��ѯ�ʶ���
            if ( !libm_handle ) {
                // �������� NULL ����,ͨ��dlerror��������ȡ���޷����ʶ�����ԭ��
                printf("Open Error:%s.\n", dlerror());
                return 0;
            }

            // ʹ�� dlsym ���������Խ����´򿪵Ķ����ļ��еķ��š��������õ�һ����Ч��ָ���÷��ŵ�ָ�룬�����ǵõ�һ�� NULL ������һ������
            TopologyGetter GetTopology = dlsym(libm_handle, "GetTopology");
            errorInfo = dlerror();// ����dlerror���������ش�����Ϣ��ͬʱ���ڴ��еĴ�����Ϣ������
            if ( errorInfo != NULL ) {
                printf("Dlsym Error:%s.\n", errorInfo);
                return 0;
            }

            // ִ��GetTopology����
            ITopology* topology = GetTopology();

            // ���� ELF �����е�Ŀ�꺯������ͨ������ dlclose ���رն����ķ���
            dlclose(libm_handle);

            return topology;

        }
#endif
    }
}