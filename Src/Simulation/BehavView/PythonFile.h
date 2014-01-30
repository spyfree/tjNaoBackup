#ifndef PythonFile_H
#define PythonFile_H

#include <QtGui>

class PythonFile :public QTextEdit
{
	Q_OBJECT
public:
	PythonFile(QWidget *parent = 0);
	void LoadPyFile(QString fileName);
	void SavePyFile();
	void SetFrontColour(QString expr);
	void InitialState();
	void AddState();
	void ReDoAct();
	void UnDoAct();
private slots:
	void TidyFile();
	void TabToSpace();

private:
	QString fileName;

};
#endif