#include <sqlite3.h>
#include <stdio.h>
#include <string>

int sqlite3_callback_func(void *pHandle, int iRet, char **szSrc, char **szDst) {
  //...
  if (1 == iRet) {
    int iTableExist =
        atoi(*(szSrc)); //此处返回值为查询到同名表的个数，没有则为0，否则大于0
    if (pHandle != nullptr) {
      int *pRes = (int *)pHandle;
      *pRes = iTableExist;
    }
    // szDst 指向的内容为"count(*)"
  }

  return 0; //返回值一定要写，否则下次调用 sqlite3_exec(...) 时会返回
            //SQLITE_ABORT
}

bool IsTableExist(sqlite3 *db, const std::string &strTableName) {
  std::string strFindTable =
      "SELECT COUNT(*) FROM sqlite_master where type ='table' and name ='" +
      strTableName + "'";

  char *sErrMsg = nullptr;

  // void *pHandle = ***;
  int nTableNums = 0;
  if (sqlite3_exec(db, strFindTable.c_str(), &sqlite3_callback_func,
                   &nTableNums, &sErrMsg) != SQLITE_OK) {
    return false;
  }
  //回调函数无返回值，则此处第一次时返回SQLITE_OK， 第n次会返回SQLITE_ABORT

  return nTableNums > 0;
}

static int callback(void *data, int argc, char **argv, char **azColName) {
  int i;
  fprintf(stderr, "%s: ", (const char *)data);
  for (i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int main(int argc, char *argv[]) {
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char *sql;
  const char *data = "Callback function called";

  rc = sqlite3_open("test_sqlite.db", &db);

  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return 0;
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }

  /* Create SQL statement */
  sql = "CREATE TABLE COMPANY("
        "ID INT PRIMARY KEY     NOT NULL,"
        "NAME           TEXT    NOT NULL,"
        "AGE            INT     NOT NULL,"
        "ADDRESS        CHAR(50),"
        "SALARY         REAL );";

  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  } else {
    fprintf(stdout, "Table created successfully\n");
  }

  /* Create SQL statement */
  sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
        "VALUES (1, 'Paul', 32, 'California', 20000.00 ); "
        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
        "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "
        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
        "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );"
        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
        "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  } else {
    fprintf(stdout, "Records created successfully\n");
  }

  /* Create SQL statement */
  sql = "SELECT * from COMPANY";

  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  } else {
    fprintf(stdout, "Operation done successfully\n");
  }

  sqlite3_close(db);
  return 0;
}
