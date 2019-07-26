#pragma once

#include <QSqlRelationalTableModel>

// Description: 

class SqlTableModel : public QSqlRelationalTableModel
{
	Q_OBJECT

	Q_PROPERTY(QString tableName MEMBER m_tableName WRITE setTableName
	           NOTIFY tableNameChanged)
	// search pattern for sqlite LIKE. Kept with % appended on both ends
	Q_PROPERTY(QString pattern READ getPattern WRITE setPattern
	           RESET resetPattern NOTIFY patternChanged)
private:
	QHash<int, QByteArray> m_roles;
	QVector<int> m_role_codes;
	QString m_tableName;
	QString m_pattern;

protected:
	// set pattern as filter. Children may override and do some pattern
	// preprocessing
	virtual auto commitPattern(const QString&) -> void;

signals:
	void tableNameChanged(const QString&);
	void patternChanged(const QString&);

public:
	SqlTableModel(QObject* parent = nullptr);

	auto setTableName(const QString&) -> void;
	auto getPattern() const -> QString;
	auto setPattern(const QString&) -> void;
	auto resetPattern() -> void;

	auto data(const QModelIndex&, int role = Qt::DisplayRole) const
		-> QVariant override;
	auto setData(const QModelIndex&, const QVariant&, int role = Qt::DisplayRole)
		-> bool override;
	auto populateRoles() -> void;
	auto roleNames() const -> QHash<int, QByteArray> override {return m_roles;}

	Q_INVOKABLE bool remove(int index);
	Q_INVOKABLE bool create(int index);
	Q_INVOKABLE bool clone(int index);

	static auto regType(const char* uri) -> void;
};
