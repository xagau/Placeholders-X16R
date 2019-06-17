



#ifndef SLIDERSGROUP_H
#define SLIDERSGROUP_H

#include <QGroupBox>

class QDial;
class QScrollBar;
class QSlider;

namespace Ui {
    class SlidersGroup;
}

class SlidersGroup : public QGroupBox
{
    Q_OBJECT

public:
    explicit SlidersGroup(Qt::Orientation orientation, const QString &title, QWidget *parent = 0);
	~SlidersGroup();

public Q_SLOTS:
    void setValue(int value);
    void setMinimum(int value);
    void setMaximum(int value);
    void invertAppearance(bool invert);
    void invertKeyBindings(bool invert);

private:
    QSlider *slider;
    QScrollBar *scrollBar;
    QDial *dial;
	
	Q_SIGNALS:
    void valueChanged(int value);
	void message(const QString &title, const QString &message, unsigned int style);
};

#endif