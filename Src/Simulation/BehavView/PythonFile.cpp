#include "PythonFile.h"
#include <iostream>


PythonFile::PythonFile(QWidget *parent /* = 0 */):QTextEdit(parent)
{
	QFont font;
	font.setFamily("Courier");
	font.setFixedPitch(true);
	font.setPointSize(10);
	setFont(font);
}
void PythonFile::SetFrontColour(QString expr)
{
	QColor c=QColor(200,0,0);
	moveCursor (QTextCursor::Start);
	while(find(expr) )
	{setTextColor(c);};
}


void PythonFile::LoadPyFile(QString fileName_)
{
	fileName = fileName_;
	QFile file(fileName_);
	if (!file.open(QFile::ReadWrite| QFile::Text)) {
		QMessageBox::warning(this, tr("SDI"),
			tr("Cannot read file %1:\n%2.")
			.arg(fileName_)
			.arg(file.errorString()));
		return;
	}
	QTextStream in(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QString text = in.readAll();
	setPlainText(text);
	QString text1="def ";
	SetFrontColour(text1);
	QString text2="class ";
	SetFrontColour(text2);
	QString text3="import ";
	SetFrontColour(text3);
	QString text4="while ";
	SetFrontColour(text4);
	QString text5="if ";
	SetFrontColour(text5);
	QString text6="else ";
	SetFrontColour(text6);
	TabToSpace();
	setTabStopWidth(32);
	QApplication::restoreOverrideCursor();
	QAction *textchange;

	//connect(this,SIGNAL(textChanged()),this,SLOT(TabToSpace()));



}

void PythonFile::SavePyFile()
{
	TabToSpace();
	QFile file(fileName);
	if (!file.open(QFile::ReadWrite| QFile::Text|QIODevice::Truncate)) {
		QMessageBox::warning(this, tr("SDI"),
			tr("Cannot read file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return;

	}
	QTextStream out(&file);
	QString outStr=toPlainText();


	out<<(outStr);

}
void PythonFile::TabToSpace()
{
	moveCursor (QTextCursor::Start);
	QString text0='\t';
	QString space="    ";
	while(find(text0) )
	{insertPlainText(space);}
	moveCursor (QTextCursor::Start);


}
void PythonFile::ReDoAct()
{
	redo();
}
void PythonFile::UnDoAct()
{
	undo();
}
void PythonFile::InitialState()
{
	QString strstate = "##initial state";
	insertPlainText(strstate);
}
void PythonFile::AddState()
{
	QString strstate = "##state";
	insertPlainText(strstate);
}
void PythonFile::TidyFile()
{
	QFile file(fileName);
	if (!file.open(QFile::ReadWrite| QFile::Text|QIODevice::Truncate)) {
		QMessageBox::warning(this, tr("SDI"),
			tr("Cannot read file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return;
}

	QTextStream in(&file);
	QTextStream out(&file);

	const int TabSize = 4;
	int endlCount = 0;
	int spaceCount = 0;
	int column = 0;
	QChar ch;

	while (!in.atEnd()){
		in>>ch;
		if(ch=='\n'){
			++endlCount;
			spaceCount=0;
			column=0;
		}else if(ch=='\t'){
			int size = TabSize - (column % TabSize);
			spaceCount += size;
			column +=size;
			std::cout<<"find tab!!!!!!!!"<<std::endl;
		}else if (ch==' '){
			++spaceCount;
			++column;
		}else{
			while(endlCount>0){
				out<<endl;
				--endlCount;
				column=0;
			}
			while(spaceCount>0){
				out<<' ';
				--spaceCount;
				++column;
			}
			out<<ch;
			++column;
		}
	}
	out<<endl;

}