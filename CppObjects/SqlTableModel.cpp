#include "CppObjects/SqlTableModel.h"
#include "defines.h"

#include <QSqlRecord>
#include <QQmlEngine>
#include <QDebug>
#include "Backend/database.h"


SqlTableModel::SqlTableModel(QObject* parent)
	: QSqlRelationalTableModel(parent, Database::connect())
{
	setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
}


auto SqlTableModel::setTableName(const QString& name) -> void
{
	submit();
	m_tableName = name;
	setTable(name);
	populateRoles();
	select();
	emit tableNameChanged(name);
}

auto SqlTableModel::getPattern() const -> QString
{
	return m_pattern.mid(1, m_pattern.length() - 2);
}

// replace all occurences of char. Use it for escaping
auto escapeAll(QString& str, QChar c) -> void
{
	int pos = str.indexOf(c);
	while (pos != -1)
	{
		str.replace(pos, '_');
		pos = str.indexOf(c, pos + 1);
	}
}

auto SqlTableModel::setPattern(const QString& arg) -> void
{
	m_pattern = arg;
	escapeAll(m_pattern, '\\');
	escapeAll(m_pattern, '%');
	escapeAll(m_pattern, '\'');
	m_pattern.push_front('%');
	m_pattern.push_back('%');
	emit patternChanged(arg);

	this->commitPattern("title LIKE '" + m_pattern + "'");
}
auto SqlTableModel::resetPattern() -> void
{
	m_pattern = "%";
	emit patternChanged("");
	this->commitPattern("");
}
auto SqlTableModel::commitPattern(const QString& pattern) -> void
{
	setFilter(pattern);
}


auto SqlTableModel::data(const QModelIndex& index, int role) const
	-> QVariant
{
	if (not index.isValid())  return QVariant();
	if (index.row() > rowCount())  return QVariant();
	if (role < Qt::UserRole)  return QSqlRelationalTableModel::data(index, role);

	return record(index.row()).value(role - Qt::UserRole);
}


auto SqlTableModel::setData(const QModelIndex& index
                           ,const QVariant& data
                           ,int role) -> bool
{
	if (not index.isValid())  return false;
	if (index.row() > rowCount()) return false;
	bool result;
	if (role < Qt::UserRole)
	{
		result = QSqlRelationalTableModel::setData(index, data, role);
	}
	else
	{
		auto&& tableIndex = this->index(index.row(), role - Qt::UserRole);
		result = QSqlRelationalTableModel::setData(tableIndex, data, Qt::EditRole);
	}
	submit();
	return result;
}


auto SqlTableModel::populateRoles() -> void
{
	m_roles.clear();
	m_role_codes.clear();
	int columns = this->columnCount();

	for (int i = 0; i < columns; ++i)
	{
		auto&& name = this->headerData(i, Qt::Horizontal).toByteArray();
		m_roles[Qt::UserRole + i] = name;
		m_role_codes.push_back(Qt::UserRole + i);
	}
}


auto SqlTableModel::regType(const char* uri) -> void
{
	qmlRegisterType<SqlTableModel>(uri, 1, 0, "SqlTableModel");
}


bool SqlTableModel::remove(int index)
{
	beginRemoveRows(QModelIndex(), index, index);
	let r = removeRows(index, 1);
	if (not r) qDebug() << "didn't delete";
	endRemoveRows();
	return r;
}


bool SqlTableModel::create(int index)
{
	return insertRows(index, 1);
}


Q_INVOKABLE bool SqlTableModel::clone(int index)
{
	let existing = record(index);
	if (existing.isEmpty()) return false;
	if (not create(index)) return false;
	submit();
	for (int i = 0; i < existing.count(); ++i)
	{
		let ind = this->index(index, i);
		setData(ind, existing.value(i), Qt::UserRole + i);
	}
	return true;
}
