
#ifndef XOLOXIMAGEWRITER_H
#define XOLOXIMAGEWRITER_H

#include "../abstractimagewriter.h"

class XoloxImageWriter : public AbstractImageWriter
{
Q_OBJECT
public:
    XoloxImageWriter(QString ext,QObject *parent = 0);

    virtual bool Export(QFile& file);
private:
signals:

public slots:

};
#endif // XOLOXIMAGEWRITER_H
