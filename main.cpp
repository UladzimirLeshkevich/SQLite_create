// https://www.tutorialspoint.com/sqlite/sqlite_using_autoincrement.htm
// https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
// https://habr.com/ru/post/149390/

#include "sqlite3.h"
#include <iostream>

//===== settings =====
static const char *SQLdropSettings = "DROP TABLE IF EXISTS SETTINGS;";
static const char *SQLcreateSettings =
    "CREATE TABLE IF NOT EXISTS SETTINGS("
    "ID                INTEGER PRIMARY KEY AUTOINCREMENT,"
    "NAME              TEXT,"
    "VALUE             INT);"
    "INSERT INTO SETTINGS (NAME,VALUE)"
    "VALUES ('N', 50);"
    "INSERT INTO SETTINGS (NAME,VALUE)"
    "VALUES ('N2', 60);";

//===== companies =====
static const char *SQLdropCompanies = "DROP TABLE IF EXISTS COMPANIES;";
static const char *SQLcreateCompanies =
    "CREATE TABLE IF NOT EXISTS COMPANIES("
    "COMPANY_ID        INTEGER PRIMARY KEY AUTOINCREMENT,"
    "NAME              TEXT);"
    "INSERT INTO COMPANIES (NAME)"
    "VALUES ('Company_1');"
    "INSERT INTO COMPANIES (NAME)"
    "VALUES ('Company_2');"
    "INSERT INTO COMPANIES (NAME)"
    "VALUES ('Company_3');";

//===== timetable =====
static const char *SQLdropTimetable = "DROP TABLE IF EXISTS TIMETABLE;";
static const char *SQLcreateTimetable =
    "CREATE TABLE IF NOT EXISTS TIMETABLE("
    "TIMETABLE_ID      INTEGER PRIMARY KEY AUTOINCREMENT,"
    "DAY               TEXT,"
    "START_TIME  NUMERIC,"
    "END_TIME    NUMERIC);"
    "INSERT INTO TIMETABLE (DAY,START_TIME,END_TIME)"
    "VALUES ('Monday', '13:30', '14:05');"
    "INSERT INTO TIMETABLE (DAY,START_TIME,END_TIME)"
    "VALUES ('Monday', '16:30', '17:00');"
    "INSERT INTO TIMETABLE (DAY,START_TIME,END_TIME)"
    "VALUES ('Tuesday', '15:25', '16:00');"
    "INSERT INTO TIMETABLE (DAY,START_TIME,END_TIME)"
    "VALUES ('Wednesday', '10:15', '11:00');"
    "INSERT INTO TIMETABLE (DAY,START_TIME,END_TIME)"
    "VALUES ('Thursday', '09:45', '10:05');"
    "INSERT INTO TIMETABLE (DAY,START_TIME,END_TIME)"
    "VALUES ('Thursday', '19:05', '20:05');"
    "INSERT INTO TIMETABLE (DAY,START_TIME,END_TIME)"
    "VALUES ('Friday', '11:35', '12:05');"
    "INSERT INTO TIMETABLE (DAY,START_TIME,END_TIME)"
    "VALUES ('Saturday', '14:20', '15:20');"
    "INSERT INTO TIMETABLE (DAY,START_TIME,END_TIME)"
    "VALUES ('Sunday', '00:00', '00:00');";

//===== numbers =====
static const char *SQLdropNumbers = "DROP TABLE IF EXISTS NUMBERS;";
static const char *SQLcreateNumbers =
    "CREATE TABLE IF NOT EXISTS NUMBERS("
    "NUMBER_ID         INTEGER PRIMARY KEY AUTOINCREMENT,"
    "ABONENT_NAME      TEXT,"
    "NUMBER            INT,"
    "COMPANY_FK        INT,"
    "FOREIGN KEY(COMPANY_FK) REFERENCES COMPANIES(COMPANY_ID));"
    "INSERT INTO NUMBERS (ABONENT_NAME,NUMBER,COMPANY_FK)"
    "VALUES ('Abonent_1', 375292597843,1);"
    "INSERT INTO NUMBERS (ABONENT_NAME,NUMBER,COMPANY_FK)"
    "VALUES ('Abonent_2', 375292597844,1);"
    "INSERT INTO NUMBERS (ABONENT_NAME,NUMBER,COMPANY_FK)"
    "VALUES ('Abonent_3', 375292597845,1);"
    "INSERT INTO NUMBERS (ABONENT_NAME,NUMBER,COMPANY_FK)"
    "VALUES ('Abonent_4', 375292597846,2);"
    "INSERT INTO NUMBERS (ABONENT_NAME,NUMBER,COMPANY_FK)"
    "VALUES ('Abonent_5', 375292597847,2);"
    "INSERT INTO NUMBERS (ABONENT_NAME,NUMBER,COMPANY_FK)"
    "VALUES ('Abonent_6', 375292597848,3);"
    "INSERT INTO NUMBERS (ABONENT_NAME,NUMBER,COMPANY_FK)"
    "VALUES ('Abonent_7', 375292597849,3);";

//===== operators =====
static const char *SQLdropOperators = "DROP TABLE IF EXISTS OPERATORS;";
static const char *SQLcreateOperators =
    "CREATE TABLE IF NOT EXISTS OPERATORS("
    "OPERATOR_ID       INTEGER PRIMARY KEY AUTOINCREMENT,"
    "NAME              TEXT,"
    "STATUS            INT);"
    "INSERT INTO OPERATORS (NAME,STATUS)"
    "VALUES ('Operator_1', 0);"
    "INSERT INTO OPERATORS (NAME,STATUS)"
    "VALUES ('Operator_2', 0);"
    "INSERT INTO OPERATORS (NAME,STATUS)"
    "VALUES ('Operator_3', 0);"
    "INSERT INTO OPERATORS (NAME,STATUS)"
    "VALUES ('Operator_4', 0);";

//===== COMPANY_TIMETABLE =====
static const char *SQLdropCT = "DROP TABLE IF EXISTS COMPANY_TIMETABLE;";
static const char *SQLcreateCT =
    "CREATE TABLE IF NOT EXISTS COMPANY_TIMETABLE("
    "COMPANY_FK          INT,"
    "TIMETABLE_FK        INT,"
    "FOREIGN KEY(COMPANY_FK) REFERENCES COMPANIES(COMPANY_ID),"
    "FOREIGN KEY(TIMETABLE_FK) REFERENCES TIMETABLE(TIMETABLE_ID));"
    "INSERT INTO COMPANY_TIMETABLE (COMPANY_FK,TIMETABLE_FK)"
    "VALUES (1, 1);"
    "INSERT INTO COMPANY_TIMETABLE (COMPANY_FK,TIMETABLE_FK)"
    "VALUES (1, 2);"
    "INSERT INTO COMPANY_TIMETABLE (COMPANY_FK,TIMETABLE_FK)"
    "VALUES (2, 1);";

//===== COMPANY_OPERATOR =====
static const char *SQLdropCO = "DROP TABLE IF EXISTS COMPANY_OPERATOR;";
static const char *SQLcreateCO =
    "CREATE TABLE IF NOT EXISTS COMPANY_OPERATOR("
    "COMPANY_FK          INT,"
    "OPERATOR_FK         INT,"
    "FOREIGN KEY(COMPANY_FK) REFERENCES COMPANIES(COMPANY_ID),"
    "FOREIGN KEY(OPERATOR_FK) REFERENCES OPERATORS(OPERATOR_ID));"
    "INSERT INTO COMPANY_OPERATOR (COMPANY_FK,OPERATOR_FK)"
    "VALUES (1, 1);"
    "INSERT INTO COMPANY_OPERATOR (COMPANY_FK,OPERATOR_FK)"
    "VALUES (1, 2);"
    "INSERT INTO COMPANY_OPERATOR (COMPANY_FK,OPERATOR_FK)"
    "VALUES (2, 3);";

//===============================================================================
int main(/*int argc, char **argv*/) {

  sqlite3 *db = nullptr;
  char *err = nullptr;

  //===== open DB =====
  if (sqlite3_open("test.db", &db)) {
    std::cout << stderr << " Can't open database:\n " << sqlite3_errmsg(db)
              << std::endl;
    return (0);
  } else {
    std::cout << stderr << " Opened database successfully\n";
  }

  //===== settings =====
  if (sqlite3_exec(db, SQLdropSettings, nullptr, nullptr, &err))
    std::cout << stderr << " SQL error:\n " << err << std::endl;
  if (sqlite3_exec(db, SQLcreateSettings, nullptr, nullptr, &err))
    std::cout << stderr << " SQL error:\n " << err << std::endl;

  //===== companies =====
  if (sqlite3_exec(db, SQLdropCompanies, nullptr, nullptr, &err))
    std::cout << stderr << " SQL error:\n " << err << std::endl;
  if (sqlite3_exec(db, SQLcreateCompanies, nullptr, nullptr, &err))
    std::cout << stderr << " SQL error:\n " << err << std::endl;

  //===== timetable =====
  if (sqlite3_exec(db, SQLdropTimetable, nullptr, nullptr, &err))
    std::cout << stderr << " SQL error:\n " << err << std::endl;
  if (sqlite3_exec(db, SQLcreateTimetable, nullptr, nullptr, &err))
    std::cout << stderr << " SQL error:\n " << err << std::endl;

  //===== numbers =====
  if (sqlite3_exec(db, SQLdropNumbers, nullptr, nullptr, &err))
    std::cout << stderr << " SQL error:\n " << err << std::endl;
  if (sqlite3_exec(db, SQLcreateNumbers, nullptr, nullptr, &err))
    std::cout << stderr << " SQL error:\n " << err << std::endl;

  //===== operators ====
  if (sqlite3_exec(db, SQLdropOperators, nullptr, nullptr, &err))
    std::cout << stderr << " SQL error:\n " << err << std::endl;
  if (sqlite3_exec(db, SQLcreateOperators, nullptr, nullptr, &err))
    std::cout << stderr << " SQL error:\n " << err << std::endl;

  //===== COMPANY_TIMETABLE =====
  if (sqlite3_exec(db, SQLdropCT, nullptr, nullptr, &err))
    std::cout << stderr << " SQL error:\n " << err << std::endl;
  if (sqlite3_exec(db, SQLcreateCT, nullptr, nullptr, &err))
    std::cout << stderr << " SQL error:\n " << err << std::endl;

  //===== COMPANY_OPERATOR =====
  if (sqlite3_exec(db, SQLdropCO, nullptr, nullptr, &err))
    std::cout << stderr << " SQL error:\n " << err << std::endl;
  if (sqlite3_exec(db, SQLcreateCO, nullptr, nullptr, &err))
    std::cout << stderr << " SQL error:\n " << err << std::endl;

  //===== close DB =====
  sqlite3_close(db);
  return 0;
}
