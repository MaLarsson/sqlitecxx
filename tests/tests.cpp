#include <iostream>
#include <gtest/gtest.h>
#include <sqlite.hpp>


TEST(database, insert) {
    // open database
    sqlite::database db(":memory:");

    // create a table
    const char* create_table_str = "CREATE TABLE testTable("
	"id INTEGER PRIMARY KEY, name TEXT NOT NULL);";

    sqlite::command create_table_cmd(db, create_table_str);

    create_table_cmd.execute();

    // insert data into table
    const char* insert_data_str = "INSERT INTO testTable(name) VALUES "
	"('kalle'), ('pelle'), ('sten');";

    sqlite::command insert_data_cmd(db, insert_data_str);

    insert_data_cmd.execute();
}


TEST(database, create) {
    // open database
    sqlite::database db(":memory:");

    // create a table
    const char* create_table_str = "CREATE TABLE testTable("
	"id INTEGER PRIMARY KEY, name TEXT NOT NULL);";

    sqlite::command create_table_cmd(db, create_table_str);

    create_table_cmd.execute();
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
