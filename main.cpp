#include <QApplication>
#include <QQmlApplicationEngine>
#include <uistream.h>
#include <QtQuick>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qRegisterMetaType<BaseModel>("BaseModel");
    qmlRegisterType<BaseModel>("WEOS", 1, 0, "WeosModel");
    qmlRegisterType<UIStream>("WEOS", 1, 0, "TStream");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
