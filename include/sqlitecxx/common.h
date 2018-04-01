#ifndef SQLITECXX_COMMON_H_
#define SQLITECXX_COMMON_H_

#include <sqlite3.h>

namespace sqlite {

constexpr int ok = SQLITE_OK;
constexpr int error = SQLITE_ERROR;
constexpr int internal = SQLITE_INTERNAL;
constexpr int perm = SQLITE_PERM;
constexpr int abort = SQLITE_ABORT;
constexpr int busy = SQLITE_BUSY;
constexpr int locked = SQLITE_LOCKED;
constexpr int no_mem = SQLITE_NOMEM;
constexpr int read_only = SQLITE_READONLY;
constexpr int interrupt = SQLITE_INTERRUPT;
constexpr int io_err = SQLITE_IOERR;
constexpr int corrupt = SQLITE_CORRUPT;
constexpr int not_found = SQLITE_NOTFOUND;
constexpr int full = SQLITE_FULL;
constexpr int cant_open = SQLITE_CANTOPEN;
constexpr int protocol = SQLITE_PROTOCOL;
constexpr int empty = SQLITE_EMPTY;
constexpr int schema = SQLITE_SCHEMA;
constexpr int too_big = SQLITE_TOOBIG;
constexpr int constraint = SQLITE_CONSTRAINT;
constexpr int mismatch = SQLITE_MISMATCH;
constexpr int misuse = SQLITE_MISUSE;
constexpr int no_lfs = SQLITE_NOLFS;
constexpr int auth = SQLITE_AUTH;
constexpr int format = SQLITE_FORMAT;
constexpr int range = SQLITE_RANGE;
constexpr int not_a_db = SQLITE_NOTADB;
constexpr int notice = SQLITE_NOTICE;
constexpr int warning = SQLITE_WARNING;
constexpr int row = SQLITE_ROW;
constexpr int done = SQLITE_DONE;

} // namespace sqlite

#endif // SQLITECXX_COMMON_H_
