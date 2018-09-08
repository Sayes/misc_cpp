/*
 * Copyright (c) 2017-2018 S.Y.Z
 *
 * gcc mongodb_conn.c -I/usr/local/include/libbson-1.0/ -lmongoc-1.0 -lbson-1.0
 * -g -o release/mongodb_conn_c
 *
 */

#include <libbson-1.0/bson.h>
#include <libmongoc-1.0/mongoc.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  int result = -1;
  bson_error_t error;
  mongoc_client_t* pmc = NULL;
  mongoc_database_t* pdb = NULL;
  mongoc_collection_t* pcoll = NULL;
  mongoc_cursor_t* pcursor = NULL;
  mongoc_init();
  do {
    pmc = mongoc_client_new("mongodb://192.168.1.32:27017");
    pdb = mongoc_client_get_database(pmc, "local");
    if (mongoc_database_has_collection(pdb, "startup_log", &error)) {
      pcoll = mongoc_database_get_collection(pdb, "startup_log");
      if (pcoll) {
        bson_t* pdata = BCON_NEW(NULL);
        pcursor = mongoc_collection_find_with_opts(pcoll, pdata, NULL, NULL);
        const bson_t* ps = mongoc_cursor_current(pcursor);
        while (mongoc_cursor_next(pcursor, &ps)) {
          size_t len;
          char* json = bson_as_canonical_extended_json(ps, &len);
          printf("%s\n", json);
          bson_free(json);
        }  // while
        bson_destroy(pdata);
      }
    }
  } while (0);
  mongoc_cursor_destroy(pcursor);
  mongoc_collection_destroy(pcoll);
  mongoc_database_destroy(pdb);
  mongoc_client_destroy(pmc);
  mongoc_cleanup();
  return result;
}
