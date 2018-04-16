#ifndef SQLITECXX_DATABASE_H_
#define SQLITECXX_DATABASE_H_

#include <string>
#include <memory>

#include "common.h"

namespace sqlite {

class database {
 public:
    database(const std::string& filename)
	: database(filename.c_str()) {}

    database(const char* filename)
	: ptr_(std::make_shared<ptr>(filename)) {}

    sqlite3* data() const noexcept { return ptr_->data(); }

 private:
    class ptr {
     public:
        ptr(const char* filename)
	    : db_(nullptr) {
	    sqlite3_open(filename, &db_);
	}

	~ptr() { sqlite3_close(db_); }

	sqlite3* data() const noexcept { return db_; }

     private:
	sqlite3* db_;
    };

    std::shared_ptr<ptr> ptr_;
};

} // namespace sqlite

#endif // SQLITECXX_DATABASE_H_
