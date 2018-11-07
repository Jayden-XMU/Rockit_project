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
 *   date: 20180704
 */

#ifndef __RT_ERROR_H__
#define __RT_ERROR_H__

typedef enum _RT_ERR{
    RT_OK                      = 0,

    RT_ERR_BAD                 = -1,
    RT_ERR_UNKNOWN             = -2,
    RT_ERR_NULL_PTR            = -3,
    RT_ERR_MALLOC              = -4,
    RT_ERR_OPEN_FILE           = -5,
    RT_ERR_VALUE               = -6,
    RT_ERR_READ_BIT            = -7,
    RT_ERR_TIMEOUT             = -8,
    RT_ERR_UNIMPLIMENTED       = -9,

    RT_ERR_BASE                = -64,

    /* The error in stream processing */
    RT_ERR_INIT                = RT_ERR_BASE - 1,
    RT_ERR_FATAL_THREAD        = RT_ERR_BASE - 2,
    RT_ERR_NOMEM               = RT_ERR_BASE - 3,
    RT_ERR_OUTOF_RANGE         = RT_ERR_BASE - 4,
} RT_ERR;

#endif // __RT_ERROR_H__
