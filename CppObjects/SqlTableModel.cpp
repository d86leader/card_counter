#include "CppObjects/SqlTableModel.h"

#include <QSqlRecord>
#include <QQmlEngine>


inline auto dbFromName(const QString& name) -> QSqlDatabase
{
	auto db = QSqlDatabase::addDatabase("QSQLITE", "TestDb");
	db.setDatabaseName(name);
	return db;
}


SqlTableModel::SqlTableModel(QObject* parent)
	: QSqlRelationalTableModel(parent, dbFromName("test.db"))
{
}


auto SqlTableModel::setTableName(const QString& name) -> void
{
	submit();
	m_tableName = name;
	emit tableNameChanged(name);
	populateRoles();
	select();
}


auto SqlTableModel::data(const QModelIndex& index, int role) const
	-> QVariant
{
	if (not index.isValid())  return QVariant();
	if (index.row() > rowCount()) return QVariant();
	if (role < Qt::UserRole)  return QSqlQueryModel::data(index, role);

	return record(index.row()).value(role - Qt::UserRole);
}


auto SqlTableModel::setData(const QModelIndex& index
                           ,const QVariant& data
                           ,int role) -> bool
{
	if (not index.isValid())  return false;
	if (index.row() > rowCount()) return false;
	if (role < Qt::UserRole)  return QSqlQueryModel::setData(index, data, role);

	auto&& tableIndex = this->index(index.row(), role - Qt::UserRole);
	return QSqlQueryModel::setData(tableIndex, data);
}


auto SqlTableModel::populateRoles() -> void
{
	m_roles.clear();
	int columns = this->columnCount();

	for (int i = 0; i < columns; ++i)
	{
		auto&& name = this->headerData(i, Qt::Horizontal).toByteArray();
		m_roles[Qt::UserRole + i] = name;
	}
}


auto SqlTableModel::regType(const char* uri) -> void
{
	qmlRegisterType<SqlTableModel>(uri, 1, 0, "SqlTableModel");
}
