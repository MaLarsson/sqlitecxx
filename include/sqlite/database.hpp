#pragma once

#include <string>
#include <memory>

#include "common.hpp"

namespace sqlite {

class database {
 public:
    database(const std::string& filename)
	: database(filename.c_str()) {}

    database(const char* filename)
	: ptr_(std::make_shared<resource>(filename)) {}

    sqlite3* data() const noexcept { return ptr_->data(); }

 private:
    class resource {
     public:
        resource(const char* filename)
	    : db_(nullptr) {
	    sqlite3_open(filename, &db_);
	}

	~resource() { sqlite3_close(db_); }

	sqlite3* data() const noexcept { return db_; }

     private:
	sqlite3* db_;
    };

    std::shared_ptr<resource> ptr_;
};

} // namespace sqlite
