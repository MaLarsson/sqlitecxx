#ifndef SQLITECXX_DATABASE_H_
#define SQLITECXX_DATABASE_H_

#include <string>
#include "common.h"

namespace sqlite {

class database {
 public:
    database(const std::string& filename)
	: database(filename.c_str()) {}

    database(const char* filename)
	: db_(nullptr) {
	sqlite3_open(filename, &db_);
    }

    ~database() { sqlite3_close(db_); }

    sqlite3* data() const noexcept { return db_; }

 private:
    sqlite3* db_;
};

} // namespace sqlite

#endif // SQLITECXX_DATABASE_H_
