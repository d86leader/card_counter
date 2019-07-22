#pragma once

#include <QObject>
#include <QString>
#include <QUrl>
#include <QPointer>
#include <QQmlEngine>

// Description: provides attached properties to control the app flow, like
// switching pages, and having top buttons


class AppControlAttached : public QObject
{
	Q_OBJECT

	QString m_name;
	Q_PROPERTY(QString pageName MEMBER m_name NOTIFY nameChanged)

signals:
	void nameChanged(const QString&);
	void changePage(const QUrl&);
	void backPage();

public:
	AppControlAttached(QObject* parent = nullptr);
};


class AppControl : public QObject
{
	Q_OBJECT

private:

signals:
	void changePage(const QUrl&);
	void backPage();

public:
	AppControl(QObject* parent = nullptr);

	static auto qmlAttachedProperties(QObject*) -> AppControlAttached*;
	static auto regType(const char* uri) -> void;

	static QPointer<AppControl> singletonInstance;
};

QML_DECLARE_TYPEINFO(AppControl, QML_HAS_ATTACHED_PROPERTIES)
