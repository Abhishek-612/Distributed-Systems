#ifndef _REQUESTHANDLER_H_
#define _REQUESTHANDLER_H_

#include "common.h"

class RequestHandler {
public:
    string send_message(const string& ip, int port, const string& message);
};

#endif /* _REQUESTHANDLER_H_ */