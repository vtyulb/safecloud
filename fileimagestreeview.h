#ifndef FILEIMAGESTREEVIEW_H
#define FILEIMAGESTREEVIEW_H

#include <QTreeView>
#include <fileimagestree.h>

class FileImagesTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit FileImagesTreeView(QObject *parent = 0);

signals:

public slots:

};

#endif // FILEIMAGESTREEVIEW_H
