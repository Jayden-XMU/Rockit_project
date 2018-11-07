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
 *   date: 2018/07/05
 */

#ifndef __RT_TIME_H__
#define __RT_TIME_H__

#include "rt_header.h"
#include "rt_log.h"

class RtTime {
public:
    struct DateTime {
        UINT16 mYear;          //!< e.g. 2005
        UINT8  mMonth;         //!< 1..12
        UINT8  mDayOfWeek;     //!< 0..6, 0==Sunday
        UINT8  mDay;           //!< 1..31
        UINT8  mHour;          //!< 0..23
        UINT8  mMinute;        //!< 0..59
        UINT8  mSecond;        //!< 0..59
    };

    static void   getDateTime(DateTime*);
    static UINT64 getNowTimeUs();
    static UINT64 getNowTimeMs();
    static void sleepMs(UINT64 time);
    static void sleepUs(UINT64 time);
};

class RtAutoTimeoutLog {
public:
    // The label is not deep-copied, so its address must remain valid for the
    // lifetime of this object
    inline RtAutoTimeoutLog(const char* label = RT_NULL, UINT64 timeout_ms = 0) : mLabel(label)
    {
        mNow     = RtTime::getNowTimeMs();
        mTimeOut = timeout_ms;
    }
    inline ~RtAutoTimeoutLog()
    {
        UINT64 duration = RtTime::getNowTimeMs() - mNow;
        if (duration >= mTimeOut) {
            RT_LOGE("%s [perf:%lld ms]\n", mLabel ? mLabel : "", duration);
        }
    }
private:
    const char* mLabel;
    UINT64      mNow;
    UINT64      mTimeOut;
};

#endif
