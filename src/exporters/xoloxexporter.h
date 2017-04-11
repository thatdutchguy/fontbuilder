#ifndef XOLOXEXPORTER_H
#define XOLOXEXPORTER_H

#include "../abstractexporter.h"

class XoloxExporter : public AbstractExporter
{
Q_OBJECT
public:
    explicit XoloxExporter(QObject *parent = 0);
protected:
    virtual bool Export(QByteArray& out);
signals:

public slots:

};

#endif // XOLOXEXPORTER_H
