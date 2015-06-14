#include "posthttp.h"

PostHTTP::PostHTTP()
{
}

bool PostHTTP::post_request(const char *field_email, const char *field_code, string (&data)[3])
{

    char *host = "188.226.187.145";
    char *port = "80";
    char *page = "/rtc/tel/zigi_phone/verification.php";

    char DataType1[] = "email=";
    char DataType2[] = "&code=";


    char str_name[] = "sip_name_phone:";
    char str_server[] = "sip_server:";

    char buf[512];

    ssize_t n;
    stringstream sendline;

    struct addrinfo hint, *addr;

    int sock = -1;

    int ContentLength = strlen(field_email) +
        strlen(field_code) +
        strlen(DataType1) +
        strlen(DataType2);

    sendline << "POST " << page << " HTTP/1.1\n";
    sendline << "Content-Type: application/x-www-form-urlencoded\n";
    sendline << "Host: "<< host <<"\n";
    sendline << "Content-Length: " << ContentLength - '\0' << "\n\n";

    // actual content
    sendline << DataType1 << field_email;
    sendline << DataType2 << field_code;

    qDebug()<<sendline.str().c_str();

    memset(&hint, 0, sizeof(hint));

    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;

    getaddrinfo(host, port, &hint, &addr);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(connect(sock, addr->ai_addr, sizeof(struct sockaddr_in)) < 0)
    {
        qDebug()<<"Error connected";
        n = 0;
    } else
    {
        write(sock, sendline.str().c_str(), sendline.str().size());
        n = read(sock, buf, sizeof(buf));
        string str_buf = (const char*) buf;

        if( str_buf.find(str_name) != string::npos && str_buf.find(str_server) != string::npos)
        {
            data[0] = str_buf.substr(str_buf.find(str_name) + strlen(str_name), 4);
            data[1] = (const char*) field_code;
            data[2] = str_buf.substr(str_buf.find(str_server) + strlen(str_server), 15);
        }


        qDebug()<<data[0].c_str();
        qDebug()<<data[1].c_str();
        qDebug()<<data[2].c_str();
    }

    close(sock);

    if(strcmp(data[0].c_str(),"") && strcmp(data[1].c_str(),"") && strcmp(data[2].c_str(),""))
    {
        return true;
    }

    return false;
}
