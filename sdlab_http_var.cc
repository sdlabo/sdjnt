#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>/* struct hostent,gethostbyname‚Ì‚½‚ß */
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include "sdlab.h"

#define SDLAB_VAR(vartype, varname) {           \
    extern vartype varname;                                 \
    printf(#varname " is %d\n", varname);                   \
  }



#include "sdlab_http_cmd.h"

void init_sdlab_var()
{
  #include "sdlab_vars.h"
}

