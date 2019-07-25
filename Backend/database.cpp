#include "database.h"
#include "defines.h"
#include <string>
#include <QStandardPaths>
#include <QDebug>
#include <QDir>


constexpr char SqliteConnectionName[] = "CounterDb";


namespace Database
{

// helper function to avoid writing multiple ifs and throws
inline auto throwDbErrWhen(bool condition, const QSqlDatabase& db) -> void
{
	if (condition)
	{
		throw DatabaseException(db.lastError());
	}
}

// creates an internal qt connection and returns it. THROWS on error
auto createQtDatabase(const QString& filename) -> QSqlDatabase
{
	let dbPath =
		QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
		"/";

	if (not QDir(dbPath).exists())
	{
		QDir().mkdir(dbPath);
	}

//	let dbName = dbPath + filename;
	let dbName = QString("test.db");
	qDebug() << "opening database" << dbName;

	auto db = QSqlDatabase::addDatabase("QSQLITE", SqliteConnectionName);
	db.setDatabaseName(dbName);

	throwDbErrWhen(not db.open(), db);

	return db;
}


auto init(const QString& filename) -> QSqlDatabase
{
	return createQtDatabase(filename);
}

// returns a connection to counter db
auto connect() -> QSqlDatabase
{
	let db = QSqlDatabase::database(SqliteConnectionName, true);

	throwDbErrWhen(not db.isOpen(), db);

	return db;
}


auto stringFromError(const QSqlError& err) -> std::string
{
	std::string str;
	for (auto&& c : err.text())
	{
		str.push_back(c.toLatin1());
	}
	return str;
}
DatabaseException::DatabaseException(const QSqlError& err)
	: std::runtime_error(stringFromError(err))
{
}

} // namespace Database
