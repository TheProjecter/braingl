/*
 * sliderwithedit.h
 *
 *  Created on: 15.05.2012
 *      Author: Ralph
 */

#ifndef SLIDERWITHEDIT_H_
#define SLIDERWITHEDIT_H_

#include <QtGui/QWidget>

class QSlider;
class QLineEdit;
class QLabel;

class SliderWithEdit : public QWidget
{
    Q_OBJECT

public:
    SliderWithEdit( QString name, QWidget* parent = 0 );
    virtual ~SliderWithEdit();

    void setValue( float value );
    float getValue();

   void setMin( float min );
   void setMax( float max );

public slots:
    void sliderChanged( int value );
    void sliderMoved( int value );
    void editEdited();

signals:
    void valueChanged( float value );

private:
   QSlider*  m_slider;
   QLineEdit* m_edit;
   QLabel* m_label;
};

#endif /* SLIDERWITHEDIT_H_ */