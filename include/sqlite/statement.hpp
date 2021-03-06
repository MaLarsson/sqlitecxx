#pragma once

#include <string>
#include <memory>
#include <utility>

#include "common.hpp"
#include "database.hpp"

namespace sqlite {
namespace detail {

class statement {
 public:
    sqlite3_stmt* data() const noexcept { return ptr_->data(); }
    void finalize() noexcept { ptr_->finalize(); }
    void reset() noexcept { sqlite3_reset(data()); }
    void clear_bindings() noexcept { sqlite3_clear_bindings(data()); }

    template <typename T>
    void bind_index(T&& param, std::size_t index) {
	sqlite3_bind_text(data(), index, param, -1, SQLITE_TRANSIENT);
    }

    template <typename ...Args>
    void bind(Args&&... args) {
	bind_indices(std::index_sequence_for<Args...>{}, std::forward<Args>(args)...);
    }

 protected:
    statement(const database& db, const std::string& query)
	: statement(db, query.c_str()) {}

    statement(const database& db, const char* query)
	: ptr_(std::make_shared<resource>(db, query)) {}

 private:
    class resource {
     public:
        resource(const database& db, const char* query)
	    : stmt_(nullptr) {
	    sqlite3_prepare_v2(db.data(), query, -1, &stmt_, nullptr);
	}

	~resource() { finalize(); }

	sqlite3_stmt* data() const noexcept { return stmt_; }
	void finalize() noexcept { sqlite3_finalize(stmt_); }

     private:
	sqlite3_stmt* stmt_;
    };

    std::shared_ptr<resource> ptr_;

    struct pass { template <typename ...T> pass(T...) {} };

    template <typename ...Args, std::size_t ...I>
    void bind_indices(std::index_sequence<I...>, Args&&... args) {
	pass{ (bind_index(args, I + 1), 1)... };
    }
};

} // namespace detail

class command : public detail::statement {
 public:
    command(const database& db, const std::string& query)
	: statement(db, query.c_str()) {}

    command(const database& db, const char* query)
	: statement(db, query) {}

    bool execute() noexcept { return sqlite3_step(data()) == sqlite::done; }
};

class query : public detail::statement {
 public:
    query(const database& db, const std::string& query)
	: statement(db, query.c_str()) {}

    query(const database& db, const char* query)
	: statement(db, query) {}

    bool step() noexcept { return sqlite3_step(data()) == sqlite::row; }

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
