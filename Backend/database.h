#pragma once

#include <QtSql>
#include <stdexcept>

constexpr char DbFileName[] = "card_counting.sqlite3";

namespace Database
{

// creates a connection and ensures tables have correct structure. Returns
// handle to it. THROWS on error
auto init(const QString& filename = DbFileName) -> QSqlDatabase;

// returns a connection to counter db
auto connect() -> QSqlDatabase;


// Exception when working with database
struct DatabaseException : public std::runtime_error
{
	inline DatabaseException() = default;
	inline DatabaseException(const DatabaseException&) = default;
	inline DatabaseException(DatabaseException&&) = default;
	DatabaseException(const QSqlError& e);
};

} // namespace Database
