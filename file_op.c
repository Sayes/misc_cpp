/*
 * Copyright (c) 2017-2018 SYZ
 *
 * gcc file_op.c -o release/file_op
 */

#include <stdio.h>
#include <string.h>

const int BUF_LEN = 1024;

void read_txt() {
  FILE* fp = fopen("test_xmlparse.xml", "r");
  if (fp == NULL) {
    return;
  }
  char tmpbuf[BUF_LEN];
  while (!feof(fp)) {
    memset(tmpbuf, 0, sizeof(tmpbuf));
    fgets(tmpbuf, BUF_LEN, fp);
    printf("%s", tmpbuf);
  }
}

void write_txt() {}

void read_bin() {}

void write_bin() {}

int main(int argc, char* argv[]) { read_txt(); }
