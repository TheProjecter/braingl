/*
 * sharpqballthread.h
 *
 *  Created on: 27.12.2012
 *      Author: Ralph
 */

#ifndef SHARPQBALLTHREAD_H_
#define SHARPQBALLTHREAD_H_

#include <QtCore/QDebug>
#include <QThread>
#include <QtCore/QVector>

#include "../thirdparty/newmat10/newmat.h"

class DatasetDWI;

class SharpQBallThread : public QThread
{
public:
    SharpQBallThread( DatasetDWI* ds, int order, int id );
    virtual ~SharpQBallThread();

    QVector<ColumnVector>* getQBallVector();

private:
    void run();

    void regularize_sqball( const double par_1, const double par_2, ColumnVector& data );

    DatasetDWI* m_ds;
    int m_order;
    int m_id;

    QVector<ColumnVector>* m_qBallVector;
};

#endif /* SHARPQBALLTHREAD_H_ */