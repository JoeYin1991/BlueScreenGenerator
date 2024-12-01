#ifndef ONEKEYSEQUENCEEDIT_H
#define ONEKEYSEQUENCEEDIT_H

#include <QKeySequenceEdit>
#include <QObject>

class OneKeySequenceEdit : public QKeySequenceEdit
{
public:
    explicit OneKeySequenceEdit(QWidget *parent = 0);
    ~OneKeySequenceEdit();

protected:
    void keyPressEvent(QKeyEvent *pEvent);
};

#endif // ONEKEYSEQUENCEEDIT_H
