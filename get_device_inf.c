/*
 * Copyright (c) 2017-2018 SYZ
 *
 */

#include <errno.h>
#include <fcntl.h>
#include <linux/hdreg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

int get_hd_sn(const char* szDevName, char* szSN, size_t nLimit) {
  struct hd_driveid id;
  int nRtn = -1;
  int fd = open(szDevName, O_RDONLY | O_NONBLOCK);
  while (1) {
    if (fd < 0) {
      perror(szDevName);
      break;
    }
    if (!ioctl(fd, HDIO_GET_IDENTITY, &id)) {
      printf("serial_no = %s\n", id.serial_no);
      strncpy(szSN, id.serial_no, nLimit);
      printf("Model Number=%s\n", id.model);
      nRtn = 0;
    }
    break;
  }
  return nRtn;
}

int main(int argc, char* argv[]) {
  char serialNum[128];
  memset(serialNum, 0, sizeof(serialNum));
  get_hd_sn(argv[1], serialNum, sizeof(serialNum));
  printf("serial = %s\n", serialNum);
  return 0;
}
