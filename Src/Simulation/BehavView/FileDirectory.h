#ifndef FileDirectory_H
#define FileDirectory_H

#include <QtGui>

class FileDir : public QDir
{
public:
	FileDir();
	FileDir(QString path,QWidget *parent = 0);
	void SetFileList(QStringList list);
	QStringList GetFileList();
private:
	QStringList fileList;

};

#endif