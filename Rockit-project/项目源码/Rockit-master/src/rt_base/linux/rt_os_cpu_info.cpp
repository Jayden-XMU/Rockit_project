/*
 * Copyright 2018 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * author: martin.cheng
 *   date: 20180801
 */

#include "../include/rt_cpu_info.h"

#ifdef OS_LINUX

#include <sysconf.h>
#include <unistd.h>
UINT32 rt_cpu_count(void) {
    UINT32 count = sysconf(_SC_NPROCESSORS_CONF);
    return count;
}

#endif // OS_LINUX
