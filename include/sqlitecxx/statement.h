#ifndef SQLITECXX_STATEMENT_H_
#define SQLITECXX_STATEMENT_H_

#include <string>
#include "common.h"

namespace sqlite {

namespace detail {

class statement {
 public:
    sqlite3_stmt* data() const noexcept { return stmt_; }

    void finalize() { sqlite3_finalize(stmt_); }
    void reset() { sqlite3_reset(stmt_); }
    void clear_bindings() { sqlite3_clear_bindings(stmt_); }

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

    template <class T>
    T get_column(size_t index) const = delete;
};

template <>
inline const void* query::get_column(size_t index) const {
    return sqlite3_column_blob(data(), index);
}

template <>
inline double query::get_column(size_t index) const {
    return sqlite3_column_double(data(), index);
}

template <>
inline sqlite_int64 query::get_column(size_t index) const {
    return sqlite3_column_int64(data(), index);
}

template <>
inline int query::get_column(size_t index) const {
    return sqlite3_column_int(data(), index);
}

template <>
inline const char* query::get_column(size_t index) const {
    return reinterpret_cast<const char*>(sqlite3_column_text(data(), index));
}

template <>
inline std::string query::get_column(size_t index) const {
    return reinterpret_cast<const char*>(sqlite3_column_text(data(), index));
}

template <>
inline sqlite3_value* query::get_column(size_t index) const {
    return sqlite3_column_value(data(), index);
}

} // namespace sqlite

#endif // SQLITECXX_STATEMENT_H_
