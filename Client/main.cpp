#include <QApplication>
#include "LoginWin.h"

int                 main(int argc, char **argv)
{
    QApplication    app(argc, argv);
    LoginWin        *client = new LoginWin();

    client->show();
    return app.exec();
}
