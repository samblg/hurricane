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

//
// Created by kinuxroot on 2016/3/5.
//

#include "utils/CommonUtils.h"

#include <cstdio>

namespace meshy {
#ifdef OS_LINUX
    int32_t SetNonBlocking(int32_t sockfd)
    {
        int32_t opts;

        opts = fcntl(sockfd, F_GETFL);
        if(opts < 0) {
            perror("fcntl(F_GETFL)\n");
            return -1;
        }
        opts = (opts | O_NONBLOCK);
        if(fcntl(sockfd, F_SETFL, opts) < 0) {
            perror("fcntl(F_SETFL)\n");
            return -1;
        }

        return 0;
    }
#endif
}
