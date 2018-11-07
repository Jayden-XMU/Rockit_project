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
 
#ifndef __COMMON_H__
#define __COMMON_H__
#define RT_TAG_SIZE            32

#define RT_ABS(x)              ((x) < (0) ? -(x) : (x))

#define RT_MAX(a, b)           ((a) > (b) ? (a) : (b))
#define RT_MAX3(a, b, c)       RT_MAX(RT_MAX(a,b),c)
#define RT_MAX4(a, b, c, d)    RT_MAX((a), RT_MAX3((b), (c), (d)))

#define RT_MIN(a,b)            ((a) > (b) ? (b) : (a))
#define RT_MIN3(a,b,c)         RT_MIN(MPP_MIN(a,b),c)
#define RT_MIN4(a, b, c, d)    RT_MIN((a), RT_MIN3((b), (c), (d)))

#define RT_SWAP(type, a, b)    do {type SWAP_tmp = b; b = a; a = SWAP_tmp;} while(0)
#define RT_ARRAY_ELEMS(a)      (sizeof(a) / sizeof((a)[0]))
#define RT_ALIGN(x, a)         (((x)+(a)-1)&~((a)-1))
#define RT_VSWAP(a, b)         { a ^= b; b ^= a; a ^= b; }

#endif