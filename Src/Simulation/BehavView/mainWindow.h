#ifndef mainWindow_H
#define mainWindow_H
#include "PythonFile.h"
#include "GraphView.h"
#include "FileDirectory.h"

#include <QMenu>
#include <QMenuBar>
#include <Python.h>
#include <string.h>
#include <qstring.h>

class mainWindow : public QMainWindow
{
	Q_OBJECT
public:
	mainWindow(QWidget *parent = 0);
	~mainWindow();
private:
	void saveFileList();
	void reloadOrder(int n);
	void readRecentFiles();
	void createWindow();
	void CreatConnect();
	void createActions();
	void createMenu();
	void createToolBar();
	void AddbehaviorMenu();
	void addRecentMenu();
	void runPython(std::string file);
	void modifySysPath();
	std::string getBaseName(std::string file);
	std::string getFolderDir(std::string file);
private slots:
	void openSingleFile();
	void OpenFiles();
	void LoadFile(QString str);
	void Initial_State();
	void Add_State();
	void ReDoAction();
	void UnDoAction();
	void LoadCurrentFile1();
	void LoadCurrentFile2();
	void LoadCurrentFile3();
	void LoadCurrentFile4();
	void LoadCurrentFile5();
	void LoadCurrentFile6();
	void SaveFile();
	void UpdateSvgView();
	void ProcessBehaMenu(QAction * action);
private:
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *behaviorMenu;
	QMenu *helpMenu;
	QMenu *recentMenu;

	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QToolBar *viewToolBar;

	QAction *openFile;
	QAction *redoing;
	QAction *undoing;
	QAction *addInitialState;
	QAction *addState;
	QAction *openDir;
	QAction *saveFile;
	QAction *updateSvg;
	QAction *BehaviorFiles;
	QSplitter *splitter;
	QSplitter *bottomSplitter;
	PythonFile *pythonFile;
	SvgWindow *svgWindow;

	QString  curFile;
	QString curDir;
	QStringList fileNameList;
	QStringList filelist;
	QList<FileDir>subDirList;
	QList<FileDir>recentDirList;
	QList<QAction*> behaviorAction;
	QList<QAction*> recentAction;
	QList<QMenu*>subMenu;

	PyObject *python_module;
	int module_ready;
	PyObject *python_instance;
	std::string globalPath;
	std::string folderDir;

};

#endif