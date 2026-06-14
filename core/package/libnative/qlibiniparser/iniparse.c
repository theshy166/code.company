#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "iniparser.h"

int main(int argc, char * argv[])
{
    dictionary * ini ;
    char       * ini_name ;

    if (argc<2) {
        ini_name = "conf.ini";
    } else {
        ini_name = argv[1] ;
    }

    ini = iniparser_load(ini_name);
    iniparser_dump(ini, stdout);
    dictionary_set(ini, "VirtualDev:NewKey0", "0");
    dictionary_set(ini, "VirtualDev:NewKey1", "1");
    dictionary_unset(ini, "VirtualDev:NewKey1");
    iniparser_dump(ini, stdout);
    {
        FILE *f_w=fopen(ini_name, "w");
        dictionary_dump(ini, f_w);
        fclose(f_w);
    }
    iniparser_freedict(ini);

    return 0 ;
}
