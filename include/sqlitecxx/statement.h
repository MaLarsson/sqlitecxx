#ifndef SQLITECXX_STATEMENT_H_
#define SQLITECXX_STATEMENT_H_

#include <string>
#include "common.h"

namespace sqlite {

inline namespace v1 {

namespace detail {

class statement {
 public:
    sqlite3_stmt* data() const noexcept { return stmt_; }

    bool finalize() { return sqlite3_finalize(stmt_) == sqlite::ok; }

 protected:
    statement(const database& db, const std::string& query)
	: statement(db, query.c_str()) {}

    statement(const database& db, const char* query)
	: stmt_(nullptr) {
	sqlite3_prepare_v2(db.data(), query, -1, &stmt_, nullptr);
    }

    ~statement() { finalize(); }

 private:
    sqlite3_stmt* stmt_;
};

} // namespace detail

class command : public detail::statement {
 public:
    command(const database& db, const std::string& query)
	: statement(db, query.c_str()) {}

    command(const database& db, const char* query)
	: statement(db, query) {}

    bool execute() { return sqlite3_step(data()) == sqlite::done; }
};

class query : public detail::statement {
 public:
    query(const database& db, const std::string& query)
	: statement(db, query.c_str()) {}

    query(const database& db, const char* query)
	: statement(db, query) {}

    bool step() { return sqlite3_step(data()) == sqlite::row; }
};

} // inline namespace v1

} // namespace sqlite

#endif // SQLITECXX_STATEMENT_H_
