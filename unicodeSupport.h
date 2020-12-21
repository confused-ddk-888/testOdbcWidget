#ifndef UNICODESUPPORT_H
#define UNICODESUPPORT_H

#include <QString>

#define QT_ODBC_UTFC_(x) QString::fromLocal8Bit(x)

#endif // UNICODESUPPORT_H
