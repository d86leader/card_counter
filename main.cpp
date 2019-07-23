#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>
#include <QDebug>

#include "CppObjects/AppControl.h"
#include "CppObjects/SqlTableModel.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    auto&& app = QGuiApplication(argc, argv);

    // register qml cpp types
    auto cppTypesUri = "CppTypes";
	AppControl::regType(cppTypesUri);
	SqlTableModel::regType(cppTypesUri);

    auto&& engine = QQmlApplicationEngine();
#ifdef QT_DEBUG
    qDebug() << "Running with disk file";
    engine.load(QUrl(QStringLiteral("Qml/main.qml")));
#else
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
#endif

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
