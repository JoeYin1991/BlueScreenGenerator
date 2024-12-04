#include "onekeysequenceedit.h"

OneKeySequenceEdit::OneKeySequenceEdit(QWidget *parent)
    : QKeySequenceEdit(parent)
{

}

OneKeySequenceEdit::~OneKeySequenceEdit()
{

}

void OneKeySequenceEdit::keyPressEvent(QKeyEvent *pEvent)
{
    QKeySequenceEdit::keyPressEvent(pEvent);

    QKeySequence seq(QKeySequence::fromString(keySequence().toString().split(", ").first()));
    if (seq[0] == Qt::Key_Delete || seq[0] == Qt::Key_Backspace) {
        seq = 0;
    }
    setKeySequence(seq);
}
