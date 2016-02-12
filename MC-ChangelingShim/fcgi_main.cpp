//
// Created by chloridecull on 2016-01-19.
//

#include <string>
#include <fcgiapp.h>
#include <HiveProtocol.pb.h>

using namespace std;

int fcgi_main() {
    FCGX_Stream *in, *out, *err;
    FCGX_ParamArray envp;
    while(FCGX_Accept(&in, &out, &err, &envp) >= 0) {
        string authkey = string(FCGX_GetParam("KEY", envp));
    }
    return 0;
}