#pragma once

#include <QSqlRelationalTableModel>

// Description: 

class SqlTableModel : public QSqlRelationalTableModel
{
	Q_OBJECT

	Q_PROPERTY(QString tableName MEMBER m_tableName WRITE setTableName
	           NOTIFY tableNameChanged)
private:
	QHash<int, QByteArray> m_roles;
	QVector<int> m_role_codes;
	QString m_tableName;

signals:
	void tableNameChanged(const QString&);

public:
	SqlTableModel(QObject* parent = nullptr);

	auto setTableName(const QString&) -> void;

	auto data(const QModelIndex&, int role = Qt::DisplayRole) const
		-> QVariant override;
	auto setData(const QModelIndex&, const QVariant&, int role = Qt::DisplayRole)
		-> bool override;
	auto populateRoles() -> void;
	auto roleNames() const -> QHash<int, QByteArray> override {return m_roles;}

	Q_INVOKABLE bool remove(int index);

	static auto regType(const char* uri) -> void;
};
