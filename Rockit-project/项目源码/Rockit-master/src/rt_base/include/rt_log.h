/*
 * Copyright 2018 The Rockit Open Source Project
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
 *   date: 20180704
 */

#ifndef __RT_LOG_H__
#define __RT_LOG_H__

#include "rt_header.h"
#include <string>

#ifndef LOG_TAG
#define LOG_TAG NULL
#endif

//! super macro.
#define RT_LOGT( format, ... ) LOG_TRACE( format, ##__VA_ARGS__ )
#define RT_LOGE( format, ... ) LOG_ERROR( format, ##__VA_ARGS__ )

//! super macro.
#define LOG_TRACE(fmt, ...) rt_log(LOG_TAG, fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) rt_err(LOG_TAG, fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

void rt_set_log_flag(UINT32 flag);
void rt_log(const char *tag, const char *fmt, const char *fname,
                            const UINT16 row, ...);
void rt_err(const char *tag, const char *fmt, const char *fname,
                            const UINT16 row, ...);

#ifdef __cplusplus
}
#endif
#endif // RBASELOG_H
