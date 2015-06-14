#ifndef POSTHTTP_H
#define POSTHTTP_H

#include <string.h>

#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <const.h>
#include <QDebug>
#include <iostream>
#include <sstream>

using namespace  std;

class PostHTTP
{
public:
    PostHTTP();
    virtual ~PostHTTP(){}

    bool post_request(const char *field_email, const char *field_code, string (&data)[3]);
};

#endif // POSTHTTP_H
