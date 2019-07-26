#include "database.h"
#include "defines.h"
#include <string>
#include <QStandardPaths>
#include <QDebug>
#include <QDir>
#include <QSqlQuery>


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
// execute a query and throw if there was an error
inline auto tryExecute(QSqlQuery& query, const QString& text) -> QSqlQuery
{
	let success = query.exec(text);
	if (not success and query.lastError().isValid())
	{
		throw DatabaseException(query.lastError());
	}
	return query;
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
	qDebug() << dbPath;

	let dbName = dbPath + filename;
	qDebug() << "opening database" << dbName;

	auto db = QSqlDatabase::addDatabase("QSQLITE", SqliteConnectionName);
	db.setDatabaseName(dbName);

	throwDbErrWhen(not db.open(), db);

	return db;
}


auto initDbTables(QSqlDatabase& db) -> QSqlDatabase&
{
	let tables = db.tables();
	if (tables.contains("cards")
	    and tables.contains("games")
	    and tables.contains("card_in_game"))
	{
		return db;
	}

	auto q = QSqlQuery(db);
	tryExecute(q, "create table if not exists cards"
	              " (rowid integer primary key, title text)");
	tryExecute(q, "create table if not exists games"
	              " (rowid integer primary key, title text)");
	tryExecute(q,
		" create table if not exists card_in_game"
		" (card_id int, game_id int"
		" ,primary key (card_id, game_id)"
		" ,foreign key (card_id) references cards(rowid)"
		" ,foreign key (game_id) references games(rowid)"
		")"
	);
	db.commit();

	return db;
}


auto init(const QString& filename) -> QSqlDatabase
{
	auto&& db = createQtDatabase(filename);
	return initDbTables(db);
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
