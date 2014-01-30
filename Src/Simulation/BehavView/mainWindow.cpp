#include "mainWindow.h"
#include "Platform/GetSetting.h"
#include <iostream>
#include <QtGui>

mainWindow::mainWindow(QWidget *parent /* = 0 */)
{
	setWindowTitle(tr("BehavView"));
	setWindowIcon(QIcon(":/icons/behavwindow.png"));
	setGeometry (0, 30, 1800, 800);
	createWindow();
	QList<int>list;
	list<<3000<<2000;
	QList<int>vList;
	vList<<3000<<1000;
	pythonFile = new PythonFile;
	svgWindow = new SvgWindow;
	bottomSplitter=new QSplitter;
	splitter=new QSplitter;
	bottomSplitter->setOrientation(Qt::Vertical);
	bottomSplitter->addWidget(svgWindow);
	bottomSplitter->setSizes(vList);
	pythonFile->setTabStopWidth(16);
	splitter->addWidget(pythonFile);
	splitter->addWidget(bottomSplitter);
	splitter->setSizes(list);
	setCentralWidget(splitter);
	CreatConnect();
	std::string globalStr1 = "/";
	int last = (std::string(GetSetting::GetConfigDir())).find_last_of(globalStr1);
	globalPath = (std::string(GetSetting::GetConfigDir())).substr(0,last);
}

mainWindow::~mainWindow()
{

}
void mainWindow::readRecentFiles()
{
	QFile recentFiles("c:\\recentFiles.txt");
	if (!recentFiles.open(QFile::ReadWrite| QFile::Text)) 
	{std::cout<<"read recentFiles failed!"<<std::endl;
	return ;}
	QTextStream in(&recentFiles);
	while(!in.atEnd()){
		QString fileline = in.readLine();
		filelist.append(fileline);}
}

void mainWindow::createWindow()
{
	createActions();
	createMenu();
	createToolBar();
	readRecentFiles();
	addRecentMenu();
	module_ready=0;
}

void mainWindow::createActions()
{
	openFile = new QAction(QIcon(":/icons/opensingle.png"),tr("Open Single File"),this);
	openDir = new QAction(QIcon(":/icons/openfiles.png"),tr("Open Files"),this);
	saveFile = new QAction(QIcon(":/icons/save.png"),tr("Save"),this);
	updateSvg = new QAction(QIcon(":/icons/refresh.png"),tr("Update"),this);
	BehaviorFiles = new QAction(tr("(Load)"),this);
	addInitialState = new QAction(tr("InitialState"),this);
	addState = new QAction(tr("State"),this);
	redoing = new QAction(QIcon(":/icons/redo.png"),tr("redo"),this);
	undoing = new QAction(QIcon(":/icons/undo.png"),tr("undo"),this);

}

void mainWindow::createMenu()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	editMenu = menuBar()->addMenu(tr("&Edit"));
	viewMenu = menuBar()->addMenu(tr("&View"));
	recentMenu = menuBar()->addMenu(tr("&recentFile"));
	helpMenu = menuBar()->addMenu(tr("&Help"));

	fileMenu->addAction(openFile);
	fileMenu->addAction(openDir);
	fileMenu->addAction(saveFile);
	viewMenu->addAction(updateSvg);

}

void mainWindow::createToolBar()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(openFile);
	fileToolBar->addAction(openDir);
	fileToolBar->addAction(saveFile);
	fileToolBar->addAction(undoing);
	fileToolBar->addAction(redoing);
	viewToolBar = addToolBar(tr("View"));
	viewToolBar->addAction(updateSvg);
	viewToolBar->addSeparator();
	//viewToolBar->addAction(BehaviorFiles);
	viewToolBar->addAction(addInitialState);
	viewToolBar->addAction(addState);
}


void mainWindow::CreatConnect()
{
	connect(openFile,SIGNAL(triggered()),this,SLOT(openSingleFile()));
	connect(openDir,SIGNAL(triggered()),this,SLOT(OpenFiles()));
	connect(saveFile,SIGNAL(triggered()),this,SLOT(SaveFile()));
	connect(updateSvg,SIGNAL(triggered()),this,SLOT(UpdateSvgView()));
	connect(addInitialState,SIGNAL(triggered()),this,SLOT(Initial_State()));
	connect(addState,SIGNAL(triggered()),this,SLOT(Add_State()));
	connect(redoing,SIGNAL(triggered()),this,SLOT(ReDoAction()));
	connect(undoing,SIGNAL(triggered()),this,SLOT(UnDoAction()));
}
void mainWindow::ReDoAction()
{
	pythonFile->ReDoAct();
}
void mainWindow::UnDoAction()
{
	pythonFile->UnDoAct();
}
void mainWindow::Initial_State()
{
	pythonFile->InitialState();
	
}
void mainWindow::Add_State()
{
	pythonFile->AddState();
}

void mainWindow::openSingleFile()
{	
	if (QString::fromStdString(folderDir)==NULL)
	{
		if (filelist.size()>0)
		{folderDir=filelist[0].toStdString();}
	}
	QString str=QFileDialog::getOpenFileName(this,"open",QString::fromStdString(folderDir),"*.py");
	folderDir = getFolderDir(str.toStdString());

	
	int existance = 1;
	foreach (QString str2, filelist) { 
		if (str==str2)
		{existance=0;}
	}
	QFile recentFiles("c:\\recentFiles.txt");
	if (!recentFiles.open(QFile::ReadWrite| QFile::Text)) 
	{std::cout<<"read recentFiles failed!"<<std::endl;
	return ;}
	if (existance==1)
	{
		filelist.append(str);


	/*
			if (filelist.size()==0)
					{filelist.append(str);}
				else
				{
					QString str1=filelist.value(filelist.size()-1);
					filelist.append(str1);
					int n = filelist.size()-1;
					for(n;n>0;n--)
					{
					filelist.replaceInStrings(filelist.value(n),filelist.value(n-1));
					}
		
					filelist.replaceInStrings(filelist.value(0),str);
					std::cout<<"filelist.value(0)"<<filelist.value(0).toStdString()<<std::endl;
				}*/
		

	}
	saveFileList();
	if(str!=NULL)
	{curFile=str;
	pythonFile->LoadPyFile(curFile);
	//嵌入python解释器，执行python文件
	runPython(curFile.toStdString());
	UpdateSvgView();}
}
void mainWindow::runPython(std::string file)
{
	//初始化python解释器
	Py_Initialize();
	if(!Py_IsInitialized())
	{
		std::cout<<"initialize python failed!"<<std::endl;
	}
	//将python文件路径放到path路径下
	modifySysPath();
	if (!module_ready)
	{module_ready=1;}
	//导入相应的python文件
	python_module = PyImport_ImportModule("DrawGraph");

	if(!python_module)
	{
		std::cout<<"import Python File failed!!!"<<std::endl;
		return ;
	}
	PyObject *dict = PyModule_GetDict(python_module);
	if (dict == NULL)
		std::cout<<"get dict error,stop!!!"<<std::endl;
	PyObject *python_class = PyDict_GetItemString(dict, "Graph");
	//创建python中类的参数列表，且该参数列表形式为一个元组
	PyObject *pArgs = PyTuple_New(1); //构造参数 
	PyTuple_SetItem(pArgs, 0, PyString_FromString(file.c_str())); 
	if (PyCallable_Check(python_class))
	{
		//实例化python类
		python_instance = PyObject_CallObject(python_class, pArgs);
	}
	else
		std::cout<<"instant class failed!"<<std::endl;

	if (python_instance == NULL)
	{
		if (PyErr_Occurred())
			PyErr_Print();
		else
			std::cout<<"  No error available"<<std::endl;
	}
	if (python_instance != NULL)
	{
		//调用类下的DoGraph函数
		PyObject *func = PyObject_CallMethod(python_instance, "DoGraph", NULL);
		if (func == NULL)
		{
			// report error
			std::cout<<"Error occurred in Behavior.Behavior.run() method"<<std::endl;
			if (PyErr_Occurred())
			{
				PyErr_Print();
			}	
			else
			{
				std::cout<<"  No Python exception information available"<<std::endl;
			}
		}
		else
		{
			std::cout<<"begin to run behavior codes!!!"<<std::endl;
			Py_DECREF(func);
		}
	}
	//关闭python解释器
	Py_Finalize();  
}

std::string mainWindow::getBaseName(std::string file)
{
	std::string baseName;
	int first = 0;
	int last = 0;
	std::string str1 = ".";
	std::string str2 = "/";
	last = file.find_first_of(str1);
	first = file.find_last_of(str2)+1;
	baseName = file.substr(first,last-first);
	
	return baseName;

}
std::string mainWindow::getFolderDir(std::string file)
{
	std::string folderDir;
	int first = 0;
	std::string str2 = "/";
	first = file.find_last_of(str2);
	folderDir = file.substr(0,first);
	return folderDir;

}
void mainWindow::modifySysPath()
{	
	
	PyRun_SimpleString("import os");
	PyRun_SimpleString("import sys");
	if (module_ready==0)
	{
	PyRun_SimpleString("os.pardir");
	PyRun_SimpleString("os.chdir(os.pardir)");/*get the superior directory*/
	PyRun_SimpleString("os.pardir");
	PyRun_SimpleString("os.chdir(os.pardir)");
	PyRun_SimpleString("os.pardir");
	PyRun_SimpleString("os.chdir(os.pardir)");
	}
	PyRun_SimpleString("Pyaddr1 = os.path.join(os.getcwd(),'Src\\Module\\Behavior\\python')");
	PyRun_SimpleString("Pyaddr2 = os.path.join(os.getcwd(),'Src\\Module\\Behavior\\python\\Player')");
	PyRun_SimpleString("Pyaddr3 = os.path.join(os.getcwd(),'Src\\Module\\Behavior\\python\\Skills')");
	PyRun_SimpleString("Pyaddr4 = os.path.join(os.getcwd(),'Src\\Module\\Behavior\\python\\Tools')");
	//PyRun_SimpleString("print Pyaddr1");
	PyRun_SimpleString("sys.path.append(Pyaddr1)");
	PyRun_SimpleString("sys.path.append(Pyaddr2)");
	PyRun_SimpleString("sys.path.append(Pyaddr3)");
	PyRun_SimpleString("sys.path.append(Pyaddr4)");
}

void mainWindow::LoadFile(QString str)
{
	curFile=curDir+"/"+str;
	pythonFile->LoadPyFile(curFile);
	runPython(curFile.toStdString());
	UpdateSvgView();
}


void mainWindow::SaveFile()
{
	pythonFile->SavePyFile();
}
void mainWindow::OpenFiles()
{
	curDir=QFileDialog::getExistingDirectory(this,"openDir");
	QDir dir(curDir);
	QStringList list;
	list<<"*.py";
	fileNameList=dir.entryList(list,QDir::Files);
	QFileInfoList dirlist=dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
	QString str=dirlist.value(0).filePath();
	for(int i=0;i<dirlist.size();i++)
	{
		FileDir dir1(dirlist.value(i).filePath());
		subDirList.append(dir1);
	}

	AddbehaviorMenu();
}
void mainWindow::UpdateSvgView()
{
	//pythonFile->SavePyFile();
	QFile file(curFile);
	QFileInfo fileinfo(file);
	QString basename=fileinfo.baseName();
	//所有生成的svg文件都放在SvgGraph文件夹下
	QString str= QString::fromStdString(globalPath);
	basename.append(".svg");
	svgWindow->openFile(str+"/Src/Module/Behavior/python/SvgGraph/"+basename);
}


void mainWindow::AddbehaviorMenu()
{
	for(int i=0;i<fileNameList.size();i++)
	{
		QAction *action=new QAction(fileNameList.value(i),this);	
		behaviorAction.append(action);
		behaviorMenu->addAction(behaviorAction.value(i));
		connect(behaviorMenu/*action*/,SIGNAL(triggered(QAction *)),this,SLOT(ProcessBehaMenu(QAction *)));
	}
	for(int i=0;i<subDirList.size();i++)
	{
		QString str=subDirList.value(i).dirName();
		QMenu *menu=new QMenu(subDirList.value(i).dirName());

		for(int j=0;j<subDirList.value(i).GetFileList().size();j++)
		{
			QAction *action=new QAction(subDirList.value(i).GetFileList().value(j),this);

			connect(menu,SIGNAL(triggered(QAction *)),this,SLOT(ProcessBehaMenu(QAction *)));
			menu->addAction(action);
		}
		if(subDirList.value(i).GetFileList().size()!=0)
		{
			behaviorMenu->addMenu(menu);
			subMenu.append(menu);
		}
	}
}
void mainWindow::ProcessBehaMenu(QAction * action )
{
	QString str=action->text();
	QList<QWidget*> menustr=action->associatedWidgets();
	QString menuName=(static_cast<QMenu*>(menustr.value(0)))->title();
	if(menuName!=tr("behaviorFile"))
		LoadFile(menuName+"/"+str);
	else 
		LoadFile(str);
}
void mainWindow::saveFileList()
{
	int i;
	QFile::remove("C:\\recentFiles.txt");
	QFile recentFiles("c:\\recentFiles.txt");
	if (!recentFiles.open(QFile::ReadWrite| QFile::Text)) 
	{std::cout<<"read recentFiles failed!"<<std::endl;
	return ;}
	QTextStream out(&recentFiles);
	for (i=0;i<filelist.size();i++)
	{QString outStr=filelist.value(i);
	out<<(outStr)<<endl;}
}
void mainWindow::reloadOrder(int n)
{
	QString fileName;
	fileName=filelist.value(n);
	for(n;n>0;n--)
	{filelist.value(n)=filelist.value(n-1);}
	filelist.value(0)=fileName;
}
void mainWindow::LoadCurrentFile1()
{

	if (filelist.size()>0)
	{
		curFile=filelist.value(0);
		pythonFile->LoadPyFile(curFile);
		runPython(curFile.toStdString());
		UpdateSvgView();
	} 
	else
	{return;}
}
void mainWindow::LoadCurrentFile2()
{
	if (filelist.size()>1)
	{
		curFile=filelist.value(1);
		pythonFile->LoadPyFile(curFile);
		runPython(curFile.toStdString());
		UpdateSvgView();
		reloadOrder(1);
		saveFileList();
	} 
	else
	{return;}
}
void mainWindow::LoadCurrentFile3()
{
	if (filelist.size()>2)
	{
		curFile=filelist.value(2);
		pythonFile->LoadPyFile(curFile);
		runPython(curFile.toStdString());
		UpdateSvgView();
		reloadOrder(2);
		saveFileList();
	} 
	else
	{return;}
}
void mainWindow::LoadCurrentFile4()
{
	if (filelist.size()>3)
	{
		curFile=filelist.value(3);
		pythonFile->LoadPyFile(curFile);
		runPython(curFile.toStdString());
		UpdateSvgView();
		reloadOrder(3);
		saveFileList();
	} 
	else
	{return;}
}
void mainWindow::LoadCurrentFile5()
{
	if (filelist.size()>4)
	{
		curFile=filelist.value(4);
		pythonFile->LoadPyFile(curFile);
		runPython(curFile.toStdString());
		UpdateSvgView();
		reloadOrder(4);
		saveFileList();
	} 
	else
	{return;}
}
void mainWindow::LoadCurrentFile6()
{
	if (filelist.size()>5)
	{
		curFile=filelist.value(5);
		pythonFile->LoadPyFile(curFile);
		runPython(curFile.toStdString());
		UpdateSvgView();
		reloadOrder(5);
		saveFileList();
	} 
	else
	{return;}
}
void mainWindow::addRecentMenu()
{	
	if (filelist.size()>0)
	{
		QAction *action1= new QAction(QIcon(":/icons/opensingle.png"),QString::fromStdString(getBaseName(filelist.value(0).toStdString())),this);
		recentMenu->addAction(action1);
		connect(action1,SIGNAL(triggered()),this,SLOT(LoadCurrentFile1()));
	}
	if (filelist.size()>1)
	{
		QAction *action1= new QAction(QIcon(":/icons/opensingle.png"),QString::fromStdString(getBaseName(filelist.value(1).toStdString())),this);
		recentMenu->addAction(action1);
		connect(action1,SIGNAL(triggered()),this,SLOT(LoadCurrentFile2()));
	}
	if (filelist.size()>2)
	{
		QAction *action1= new QAction(QIcon(":/icons/opensingle.png"),QString::fromStdString(getBaseName(filelist.value(2).toStdString())),this);
		recentMenu->addAction(action1);
		connect(action1,SIGNAL(triggered()),this,SLOT(LoadCurrentFile3()));
	}
	if (filelist.size()>3)
	{
		QAction *action1= new QAction(QIcon(":/icons/opensingle.png"),QString::fromStdString(getBaseName(filelist.value(3).toStdString())),this);
		recentMenu->addAction(action1);
		connect(action1,SIGNAL(triggered()),this,SLOT(LoadCurrentFile4()));
	}
	if (filelist.size()>4)
	{
		QAction *action1= new QAction(QIcon(":/icons/opensingle.png"),QString::fromStdString(getBaseName(filelist.value(4).toStdString())),this);
		recentMenu->addAction(action1);
		connect(action1,SIGNAL(triggered()),this,SLOT(LoadCurrentFile5()));
	}
	if (filelist.size()>5)
	{
		QAction *action1= new QAction(QIcon(":/icons/opensingle.png"),QString::fromStdString(getBaseName(filelist.value(5).toStdString())),this);
		recentMenu->addAction(action1);
		connect(action1,SIGNAL(triggered()),this,SLOT(LoadCurrentFile6()));
	}



	//for(int i=0;i<filelist.size();i++)
	//{

	//fileMenu->addAction(openFile);
	//openFile = new QAction(QIcon(":/icons/opensingle.png"),tr("Open Single File"),this);
	//	QAction *action=new QAction(filelist.value(i),this);	
	//	recentAction.append(action);
	//	recentMenu->addAction(recentAction.value(i));
	//	connect(recentAction.value(i),SIGNAL(triggered()),this,SLOT(LoadCurrentFile()));
	//connect(updateSvg,SIGNAL(triggered()),this,SLOT(UpdateSvgView()));
	//UpdateSvgView()
	//}

}