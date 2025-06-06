#include "LogMessage.h"
#include "LogFormatting.h"

ost::log::LogMessage::LogMessage(ELevel msgLvl, const LogInstance* lInstPtr, long long ts, const std::string& msg)
    : MsgLevel{msgLvl}
    , LoggingInstancePtr{lInstPtr}
    , TimeStamp{ts}
    , Message{msg}
{
}

std::string ost::log::LogMessage::GetTimeStampString() const
{
    return FormatLogTimeStamp(TimeStamp);
}
