/*
 * property.h
 *
 *  Created on: Jan 17, 2013
 *      Author: schurade
 */
#ifndef PROPERTY_H_
#define PROPERTY_H_

#include "../enums.h"

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QObject>

#include <limits>

class QWidget;

class Property : public QObject
{
public:
    Property( Fn::Property name );
    virtual ~Property();

    Fn::Property getName();
    QString getLabel();

    virtual QWidget* getWidget() = 0;
    virtual QVariant getValue() = 0;
    virtual void setValue( QVariant value ) = 0;

protected:
    Fn::Property m_name; // used for access
    QString m_label; // used for label in the dataset property view

private:

};

#endif /* PROPERTY_H_ */
