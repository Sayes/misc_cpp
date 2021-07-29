/*
 * Copyright (c) 2017-2018 SYZ
 */

/*

#include <objbase.h>
#include <string>
#include "stdafx.h"

void getuuid(std::string& strguid)
{
        char buffer[64] = { 0 };
        GUID guid;
        strguid = "";
        if (CoCreateGuid(&guid) == S_OK) {
                sprintf_s(buffer
                        , sizeof(buffer)
                        , "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"
                        , guid.Data1, guid.Data2, guid.Data3
                        , guid.Data4[0], guid.Data4[1], guid.Data4[2],
guid.Data4[3] , guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
                strguid = buffer;
        }
}

Windows£ºCoCreateGuid(GUID)
#include<objbase.h>
#include<stdio.h>

#defineGUID_LEN64

intmain(intargc,char*argv[])
{
        charbuffer[GUID_LEN]={0};
        GUIDguid;

        if(CoCreateGuid(&guid))
        {
                fprintf(stderr,"createguiderror\n");
                return-1;
        }
        _snprintf(buffer,sizeof(buffer),
                "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
                guid.Data1,guid.Data2,guid.Data3,
                guid.Data4[0],guid.Data4[1],guid.Data4[2],
                guid.Data4[3],guid.Data4[4],guid.Data4[5],
                guid.Data4[6],guid.Data4[7]);
                printf("guid:%s\n",buffer);

        return0;
}

*/

// g++ uuid_gen.cc -std=c++11 -luuid -o release/uuid_gen
//
// Linux: uuid_generate(UUID)
// cmd: gccatemp.c-oatemp-luuid
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <uuid/uuid.h>
#include <string>

std::string uuid_gen(const std::string prefix) {
  std::string str = "";
  int fd = open("/proc/sys/kernel/random/uuid", O_RDONLY);

  if (fd < 0) {
    return str;
  }

  char buf[100];
  memset(buf, 0, sizeof(buf));
  int nret = read(fd, buf, sizeof(buf) - 1);

  if (nret > 0) {
    int len = strlen(buf);
    if (buf[len - 1] == '\n') {
      buf[len - 1] = 0;
    }
    str = prefix + std::string(buf);
  }

  close(fd);
  return str;
}

int main(int argc, char* argv[]) {
  uuid_t uu;
  int i;
  uuid_generate(uu);

  for (i = 0; i < 16; ++i) {
    printf("%02X-", uu[i]);
  }
  printf("\n");

  printf("%s\n", uuid_gen("").c_str());

  return 0;
}
