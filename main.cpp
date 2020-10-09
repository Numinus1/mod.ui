#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include "backend.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    Backend ark;

    QQmlContext* ctx0 = engine.rootContext();
    ctx0->setContextProperty("ark", &ark);
    QQmlApplicationEngine::setObjectOwnership(ctx0, QQmlApplicationEngine::CppOwnership);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
