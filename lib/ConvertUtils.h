#pragma once

#include <quickfix/Message.h>

inline void convert_dds_string_to_fix(const std::string &string_in, const int FIX_TAG, FIX::FieldMap &fix_field_map_out)
{
    if (string_in.length() > 0)
    {
        FIX::StringField fixField(FIX_TAG, string_in);
        fix_field_map_out.setField(fixField);
    };
};

inline void convert_dds_timestamp_to_fix(unsigned long long timestamp_in, const int FIX_TAG, FIX::FieldMap &fix_field_map_out)
{
    if (timestamp_in > 0)
    {
        FIX::UtcTimeStamp utcTimeStamp((time_t)(timestamp_in / 1000000));
        utcTimeStamp.setMicrosecond(timestamp_in - ((timestamp_in / 1000000) * 1000000));
        FIX::UtcTimeStampField timestampField(FIX_TAG, utcTimeStamp, 6);
        fix_field_map_out.setField(timestampField);
    };
};