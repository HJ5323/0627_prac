#include <stdlib.h>
#include <iostream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

using namespace std;

// for demonstration only. never save your password in the code!
const string server = "tcp://127.0.0.1:3306"; // 데이터베이스 주소
const string username = "user"; // 데이터베이스 사용자
const string password = "1234"; // 데이터베이스 접속 비밀번호

int main()
{
    // MySQL Connector/C++ 초기화
    sql::Driver* driver; // 추후 해제하지 않아도 Connector/C++가 자동으로 해제해 줌
    sql::Connection* con;
    sql::Statement* stmt;
    sql::PreparedStatement* pstmt;

    try
    {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }

    // please create database "cpp_db" ahead of time
    con->setSchema("cpp_db");
 
     // connector에서 한글 출력을 위한 셋팅 
     stmt = con->createStatement(); // 추가!!
     stmt->execute("set names euckr"); // 추가!!
     if (stmt) { delete stmt; stmt = nullptr; } // 추가!!

    // 데이터베이스 쿼리 실행
    stmt = con->createStatement();
    stmt->execute("DROP TABLE IF EXISTS inventory"); // Statement 객체를 사용하여 inventory 테이블 삭제
    cout << "Finished dropping table (if existed)" << endl;
    // inventory 테이블 생성 (구조 : id(기본키) ,name(문자열) ,quantity(정수))
    stmt->execute("CREATE TABLE inventory (id serial PRIMARY KEY, name VARCHAR(50), quantity INTEGER);"); 
    cout << "Finished creating table" << endl;
    delete stmt;

    // PreparedStatement 객체를 사용하여 inventory 테이블에 데이터 삽입
    // INSERT INTO table_name (column1, column2, ...) VALUES (value1, value2, ...);
    pstmt = con->prepareStatement("INSERT INTO inventory(name, quantity) VALUES(?,?)"); 
    pstmt->setString(1, "banana"); // 첫번째 데이터로 name에 banana 삽입
    pstmt->setInt(2, 150); // quantity에 150 삽입
    pstmt->execute();
    cout << "One row inserted." << endl;

    pstmt->setString(1, "오렌지");
    pstmt->setInt(2, 154);
    pstmt->execute();
    cout << "One row inserted." << endl;

    pstmt->setString(1, "apple");
    pstmt->setInt(2, 100);
    pstmt->execute();
    cout << "One row inserted." << endl;

    // 업데이트
    // UPDATE table_name SET column1 = value1, column2 = value2 WHERE condition;
    try {
        pstmt = con->prepareStatement("UPDATE inventory SET quantity = ? WHERE id = ?");
        pstmt->setInt(1, 50); // id가 1인 레코드의 quantity값을 50으로 업데이트
        pstmt->setInt(2, 1); // 업데이트할 행의 id 설정
        pstmt->executeUpdate();
        cout << "Update successful." << endl;
    }
    catch (sql::SQLException& e) {
        cout << "Update failed." << e.what() << endl;
    }

    // 삭제
    // DELETE FROM table_name WHERE condition;
    try {
        pstmt = con->prepareStatement("DELETE FROM inventory WHERE id = ?");
        pstmt->setInt(1, 2); // id가 2인 레코드 삭제
        pstmt->executeUpdate();
        cout << "Delete successful." << endl;
    }
    catch (sql::SQLException& e) {
        cout << "Delete failed." << e.what() << endl;
    }

    // MySQL Connector/C++ 정리
    delete pstmt;
    delete con;
    system("pause");
    return 0;
}

// INSERT INTO table_name (column1, column2, ...) VALUES (value1, value2, ...);
// SELECT column1, column2, ... FROM table_name WHERE condition;
// UPDATE table_name SET column1 = value1, column2 = value2 WHERE condition;
// DELETE FROM table_name WHERE condition;