#include "CppObjects/AppControl.h"


AppControlAttached::AppControlAttached(QObject* parent)
	: QObject(parent)
	, m_name()
{
}


QPointer<AppControl> AppControl::singletonInstance;

AppControl::AppControl(QObject* parent)
	: QObject(parent)
{
	AppControl::singletonInstance = this;
}


auto AppControl::qmlAttachedProperties(QObject* parent) -> AppControlAttached*
{
	auto* attached = new AppControlAttached(parent);

	if (AppControl::singletonInstance)
	{
		auto that = AppControl::singletonInstance;
		connect(attached, SIGNAL(changePage(const QUrl&))
		       ,that, SIGNAL(changePage(const QUrl&)));
		connect(attached, SIGNAL(backPage())
		       ,that, SIGNAL(backPage()));
	}

	return attached;
}


auto AppControl::regType(const char* uri) -> void
{
	qmlRegisterType<AppControlAttached>();
	qmlRegisterUncreatableType<AppControl>(uri, 1, 0, "AppControl", "Uncreatable");
}
