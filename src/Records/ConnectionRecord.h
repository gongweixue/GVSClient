#ifndef __CONNECTION_RECORD_H__
#define __CONNECTION_RECORD_H__

#include <string>

typedef struct ConnectionRecord{
    std::string ip;
    std::string port;
} ConnectionRecord;

#endif