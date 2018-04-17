/*
 * https://github.com/mysql/mysql-connector-cpp.git
 *
 * g++ mysql_conn.cc -std=c++11 -I$MYSQLCPPCONN_HOME/include
 * -L$MYSQLCPPCONN_HOME/lib -lmysqlcppconn -o release/mysql_conn
 */

#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>

int main(int argc, char* argv[]) {
  std::string host = "127.0.0.1:3306";
  std::string user = "root";
  std::string pswd = "123456";
  std::string schema = "sys";
  std::string sqlstmt = "show databases";

  sql::Driver* pdriver_ = nullptr;
  sql::Connection* pconnection_ = nullptr;
  sql::Statement* pstmt_ = nullptr;
  sql::ResultSet* pres_ = nullptr;

  do {
    pdriver_ = get_driver_instance();
    if (pdriver_ == nullptr) break;
    try {
      pconnection_ =
          pdriver_->connect(host.c_str(), user.c_str(), pswd.c_str());
      if (pconnection_ == nullptr) break;

      pconnection_->setSchema(schema.c_str());
      pstmt_ = pconnection_->createStatement();
      if (pstmt_ == nullptr) break;

      pres_ = pstmt_->executeQuery(sqlstmt.c_str());
      if (pres_ == nullptr) break;

      while (pres_->next()) {
        std::cout << pres_->getString("Database") << std::endl;
      }
    } catch (sql::SQLException& e) {
      std::cout << e.what() << std::endl;
    }

  } while (0);
  if (pconnection_) {
    delete pconnection_;
    pconnection_ = nullptr;
  }

  return 0;
}
