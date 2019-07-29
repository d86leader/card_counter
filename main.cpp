#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>
#include <QDebug>

#include "Backend/database.h"
#include "CppObjects/SqlTableModel.h"
#include "CppObjects/DeckBuildModel.h"
#include "CppObjects/GameModel.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    auto&& app = QGuiApplication(argc, argv);

	Database::init();

    // register qml cpp types
    auto cppTypesUri = "CppTypes";
	SqlTableModel::regType(cppTypesUri);
	DeckBuildModel::regType(cppTypesUri);
	GameModel::regType(cppTypesUri);

    auto&& engine = QQmlApplicationEngine();
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
