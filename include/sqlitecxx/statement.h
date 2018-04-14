#ifndef SQLITECXX_STATEMENT_H_
#define SQLITECXX_STATEMENT_H_

#include <string>
#include <memory>
#include <utility>

#include "common.h"

namespace sqlite {

namespace detail {

class statement_impl {
 public:
    statement_impl(const database& db, const std::string& query)
	: statement_impl(db, query.c_str()) {}

    statement_impl(const database& db, const char* query)
	: stmt_(nullptr) {
	sqlite3_prepare_v2(db.data(), query, -1, &stmt_, nullptr);
    }

    ~statement_impl() { finalize(); }

    sqlite3_stmt* data() const noexcept { return stmt_; }

    void finalize() { sqlite3_finalize(stmt_); }
    void reset() { sqlite3_reset(stmt_); }
    void clear_bindings() { sqlite3_clear_bindings(stmt_); }

    template <typename T>
    void bind_index(T&& param, std::size_t index) {
	sqlite3_bind_text(stmt_, index, param, -1, SQLITE_TRANSIENT);
    }

    template <typename ...Args>
    void bind(Args&&... args) {
	bind_indices(std::index_sequence_for<Args...>{}, std::forward<Args>(args)...);
    }

 private:
    sqlite3_stmt* stmt_;

    struct pass { template <typename ...T> pass(T...) {} };

    template <typename ...Args, std::size_t ...I>
    void bind_indices(std::index_sequence<I...>, Args&&... args) {
	pass{ (bind_index(args, I + 1), 1)... };
    }
};

class statement {
 public:
    sqlite3_stmt* data() const noexcept { return impl_->data(); }

    void finalize() { impl_->finalize(); }
    void reset() { impl_->reset(); }
    void clear_bindings() { impl_->clear_bindings(); }

    template <typename T>
    void bind_index(T&& param, std::size_t index) {
	impl_->bind_index(std::forward<T>(param), index);
    }

    template <typename ...Args>
    void bind(Args&&... args) { impl_->bind(std::forward<Args>(args)...); }

 protected:
    statement(const database& db, const std::string& query)
	: statement(db, query.c_str()) {}

    statement(const database& db, const char* query)
	: impl_(std::make_shared<statement_impl>(db, query)) {
	auto stmt = impl_->data();
	sqlite3_prepare_v2(db.data(), query, -1, &stmt, nullptr);
    }

 private:
    std::shared_ptr<statement_impl> impl_;
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

    template <typename T>
    T get_column(std::size_t) const { return T(); }
};

template <>
inline const void* query::get_column(std::size_t index) const {
    return sqlite3_column_blob(data(), index);
}

template <>
inline double query::get_column(std::size_t index) const {
    return sqlite3_column_double(data(), index);
}

template <>
inline sqlite_int64 query::get_column(std::size_t index) const {
    return sqlite3_column_int64(data(), index);
}

template <>
inline int query::get_column(std::size_t index) const {
    return sqlite3_column_int(data(), index);
}

template <>
inline const char* query::get_column(std::size_t index) const {
    return reinterpret_cast<const char*>(sqlite3_column_text(data(), index));
}

template <>
inline std::string query::get_column(std::size_t index) const {
    return reinterpret_cast<const char*>(sqlite3_column_text(data(), index));
}

template <>
inline sqlite3_value* query::get_column(std::size_t index) const {
    return sqlite3_column_value(data(), index);
}

} // namespace sqlite

#endif // SQLITECXX_STATEMENT_H_
