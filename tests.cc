#include <iostream>
#include "sqlitecxx/sqlitecxx.h"

int main()
{
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

    // print all names in table
    sqlite::query map_data_query(db, "SELECT name FROM testTable");

    while (map_data_query.step()) {
	std::cout << map_data_query.get_column<std::string>(0) << '\n';
    }

    // test bind
    sqlite::query bind_query(db, "SELECT id, name FROM testTable WHERE testTable.name = ? OR testTable.name = ?");
    bind_query.bind_index("pelle", 1);
    bind_query.bind_index("sten", 2);

    while (bind_query.step()) {
	std::cout << "index for " << bind_query.get_column<std::string>(1) << " is: " <<
	    bind_query.get_column<int>(0) << '\n';
    }

    bind_query.reset();
    bind_query.clear_bindings();
    bind_query.bind("kalle", "sten");

    while (bind_query.step()) {
	std::cout << "index for " << bind_query.get_column<std::string>(1) << " is: " <<
	    bind_query.get_column<int>(0) << '\n';
    }

    return 0;
}
