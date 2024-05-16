#include <QApplication>
#include <QQmlApplicationEngine>
#include "backend.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qmlRegisterType<Backend>("filter.designer.qmlcomponents", 1, 0, "Backend");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
