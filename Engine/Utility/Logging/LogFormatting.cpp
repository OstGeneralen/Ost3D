#include "LogFormatting.h"
#include <chrono>

std::string ost::log::FormatLogTimeStamp(long long timeStamp)
{
    time_t asTimeT = static_cast<time_t>(timeStamp);
    std::chrono::system_clock::time_point t = std::chrono::system_clock::from_time_t(asTimeT); 
    return std::format("{:%H:%M:%S}", std::chrono::time_point_cast<std::chrono::seconds>(t));
}
