#include <QGuiApplication>
#include "tcpserver.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	TcpServer tcpserver;

	return app.exec();
}
