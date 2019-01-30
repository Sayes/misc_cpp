/*
 * Copyright (c) 2017-2018 S.Y.Z
 *
 * gcc mongodb_conn.c -I/usr/local/include/libbson-1.0/ -lmongoc-1.0 -lbson-1.0
 * -g -o release/mongodb_conn_c
 *
 */

#include <bson.h>
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
    pmc = mongoc_client_new("mongodb://192.168.1.41:32017");
    pdb = mongoc_client_get_database(pmc, "local");
    if (mongoc_database_has_collection(pdb, "startup_log", &error)) {
      pcoll = mongoc_database_get_collection(pdb, "startup_log");
      if (pcoll) {
        bson_t* pfilter = BCON_NEW(NULL);
        bson_t* pminmaxfilter = BCON_NEW(NULL);
        bson_t* popts = BCON_NEW(NULL);
        bson_t* psort = BCON_NEW(NULL);

        bson_init(pfilter);
        bson_init(pminmaxfilter);
        bson_init(popts);
        bson_init(psort);

        BSON_APPEND_UTF8(pfilter, "hostname", "mongodb-c4db6d45f-kgjdf");
        BSON_APPEND_INT64(pfilter, "pid", 1);

        BSON_APPEND_INT64(popts, "limit", 1);
        BSON_APPEND_INT64(psort, "_id", 1);
        BSON_APPEND_DOCUMENT(popts, "sort", psort);

        pcursor = mongoc_collection_find_with_opts(pcoll, pfilter, popts, NULL);
        const bson_t* ps = mongoc_cursor_current(pcursor);
        while (mongoc_cursor_next(pcursor, &ps)) {
          size_t len;
          char* json = bson_as_canonical_extended_json(ps, &len);
          printf("%s\n", json);
          bson_free(json);
        }  // while

        bson_destroy(psort);
        bson_destroy(popts);
        bson_destroy(pminmaxfilter);
        bson_destroy(pfilter);
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
