

#include <QtCore/QTimer>
#include <QtGui/QContextMenuEvent>
#include <QtCore/QMetaProperty>
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtGui/QColor>
#include <QVector>

QVector<QVector<int>> array;
QVector<int> temp;

int main(){
for(int i=0; i<10; i++)
{
	temp.append(1);
	temp.append(2);
	temp.append(3);
	temp.append(4);
	array.append(temp);
}
qDebug()<<array.size()<<array[0].size();
qDebug()<<array[0];
qDebug()<<array[0][0];
return 0;
}
