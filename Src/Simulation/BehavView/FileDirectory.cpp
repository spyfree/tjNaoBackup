#include "FileDirectory.h"

FileDir::FileDir()
{

}

FileDir::FileDir(QString path,QWidget *parent /* = 0 */)
{
	setPath(path);
}

void FileDir::SetFileList(QStringList list)
{
	fileList = list;
}

QStringList FileDir::GetFileList()
{
	QStringList list;
	list << "*.py";
	fileList = entryList(list,QDir::Files);
	return fileList;
}