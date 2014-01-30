
#include "ConnectDialog.h"

#define  DEFAULT_PORT

ConnectDialog::ConnectDialog(QMdiArea* mdiArea):SubWindow(mdiArea,"Add New Robot")
{
	//setAttribute(Qt::WA_DeleteOnClose);
	setFixedSize(200,200);
	nameEdit=new QLineEdit;
	ipEdit=new QLineEdit;
	portEdit=new QLineEdit;
	cancelButton=new QPushButton(tr("&Cancel"),this);
	okButton=new QPushButton(tr("&Ok"),this);
   
	layout=new QGridLayout;
	QVBoxLayout *mainlayout=new QVBoxLayout();
	QHBoxLayout *buttonlayout=new QHBoxLayout;
	buttonlayout->addWidget(okButton);
	buttonlayout->addWidget(cancelButton);
	activeRobot=new ConnectInfo;
	layout->addWidget(new QLabel(tr("Name"),this),0,0);
	nameEdit->setText(activeRobot->nameString);
	layout->addWidget(nameEdit,0,1);
	layout->addWidget(new QLabel(tr("Ip"),this),1,0);
	ipEdit->setText(activeRobot->ipstring);
	layout->addWidget(ipEdit,1,1);
	layout->addWidget(new QLabel(tr("Port"),this),2,0);
	portEdit->setValidator(new QIntValidator(1, 65535, this));
	portEdit->setText(activeRobot->port);
	
#ifdef DEFAULT_PORT
	portEdit->setReadOnly(true);
#endif
	layout->addWidget(portEdit,2,1);
	layout->setColumnStretch(1, 1);
	mainlayout->addLayout(layout);
	mainlayout->addLayout(buttonlayout);
	widget=new QWidget(this);
	widget->setLayout(mainlayout);
	setWidget(widget);
	connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
	connect(okButton,SIGNAL(clicked()),this,SLOT(SaveValues()));

}

void ConnectDialog::SaveValues()
{
	activeRobot->nameString=nameEdit->text();
	activeRobot->ipstring=ipEdit->text();
	activeRobot->port=portEdit->text();
	setVisible(false);
	emit SendValues(*activeRobot);
	
}
void ConnectDialog::Show()
{
	if(!isVisible())
	{	
		setVisible(!isVisible());
		widget->show();
	}
}
void ConnectDialog::NewItem()
{
	activeRobot->SetInit();
	UpdateValues();
	Show();
	
}
void ConnectDialog::EditItem(ConnectInfo const robotinfo)
{
	activeRobot->Copy(robotinfo);
	UpdateValues();
	Show();
}
void ConnectDialog::UpdateValues()
{
	nameEdit->setText(activeRobot->nameString);
	ipEdit->setText(activeRobot->ipstring);
	portEdit->setText(activeRobot->port);
}

ConnectWindow ::ConnectWindow(QMdiArea *mdiArea/* =0 */):SubWindow(mdiArea,"Connect")
{	setFixedSize(200,150);
	addButton=new QPushButton(QIcon(":/icons/add.png"),"",this);
	delButton=new QPushButton(QIcon(":/icons/del.png"),"",this);
	editButton=new QPushButton(QIcon(":/icons/edit.png"),"",this);
	editButton->setFixedSize(20,20);
	delButton->setFixedSize(20,20);
	addButton->setFixedSize(20,20);
	robotList=new QComboBox;
	okButton=new QPushButton(tr("Connect"),this);
	cancelButton=new QPushButton(tr("Cancel"),this);
	infoLayout=new QHBoxLayout;
	infoLayout->addWidget(robotList);
	infoLayout->addWidget(editButton);
	infoLayout->addWidget(addButton);
	infoLayout->addWidget(delButton);	
	buttonLayout=new QHBoxLayout;
	buttonLayout->addWidget(okButton);
	buttonLayout->addWidget(cancelButton);
	mainlayout=new QVBoxLayout;
	mainlayout->addLayout(infoLayout);
	mainlayout->addLayout(buttonLayout);
	widget=new QWidget;
	widget->setLayout(mainlayout);
	setWidget(widget);
	connectRobot=new ConnectInfo;
	robotInfoList=new QVector<ConnectInfo>;
	connectDialog=new ConnectDialog(mdiArea); 
	connectDialog->setVisible(false);
	connect(connectDialog,SIGNAL(SendValues(ConnectInfo)),
		this,SLOT(ReceiveSetting(ConnectInfo)));
	connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
	connect(addButton,SIGNAL(clicked()),this,SLOT(AddItem()));
	connect(delButton,SIGNAL(clicked()),this,SLOT(DeleteItem()));
	connect(editButton,SIGNAL(clicked()),this,SLOT(EditItem()));
	connect(okButton,SIGNAL(clicked()),this,SLOT(SendInfo()));
	connect(robotList,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeIndex(int)));
		readInfo();
}
ConnectWindow::~ConnectWindow()
{
	
	delete robotInfoList;
}
void ConnectWindow::Show()
{
	if(!isVisible())
	{	
	setVisible(!isVisible());
	widget->show();
	}
	
}
void ConnectWindow::ReceiveSetting(ConnectInfo const robotinfo)
{
	int index=robotList->findText(robotinfo.nameString);
	if(index==-1)
	{
	connectRobot=robotinfo;
	robotInfoList->append(connectRobot);
	robotList->addItem(connectRobot.nameString);
	robotList->setCurrentIndex(robotList->count()-1);
	}
	else 
	{
		connectRobot=robotinfo;
		robotInfoList->replace(index,robotinfo);
		robotList->setCurrentIndex(index);
	}
	
}
void ConnectWindow::DeleteItem()
{
	int index=robotList->currentIndex();
	int counter=robotList->count();
	
	if(counter>0)
	{
	robotInfoList->remove(robotList->currentIndex());
	robotList->removeItem(index);
	}
}
void ConnectWindow::EditItem()
{
	int index=robotList->currentIndex();
	connectDialog->EditItem(robotInfoList->value(index));
}
void ConnectWindow::AddItem()
{
	connectDialog->NewItem();
}
int ConnectWindow::IntFromString(QString str)
{

	return 0;
}
void ConnectWindow::SendInfo()
{	
	setVisible(false);

	emit ConfigInfo(connectRobot);
	MenuBar::getInstance()->connectAction->setToolTip(connectRobot.ipstring);
	writingInfo(robotInfoList);
}
void ConnectWindow::writingInfo(QVector<ConnectInfo> * list)
{
	/*QFile file("test.txt");
	file.open(QIODevice::ReadWrite);
	for(int i=0;i<list->size();i++)
	{
	file.write(list->value(i).nameString,sizeof(QString));
	file.write(list->value(i).ipstring,sizeof(QString));
	file.write(list->value(i).port,sizeof(QString));
	}
	file.close();*/
	std::string path=std::string(GetSetting::GetConfigDir())+"/ipAddress.cfg";
	QFile file(path.c_str());
	file.open(QIODevice::ReadWrite|QIODevice::Truncate);
	QTextStream stream(&file);
	int m=robotList->currentIndex();

	for(int i=0;i<list->size();i++)
	{
		if(i!=m)
		{
	stream<<list->value(i).nameString;
	stream<<"\t";
	stream<<list->value(i).ipstring;
	stream<<"\t";
	stream<<list->value(i).port;
	stream<<"\n";
		}
	}
	stream<<list->value(m).nameString;
	stream<<"\t";
	stream<<list->value(m).ipstring;
	stream<<"\t";
	stream<<list->value(m).port;
	stream<<"\n";
	file.close();

}
void ConnectWindow::readInfo()
{
	std::string path=std::string(GetSetting::GetConfigDir())+"/ipAddress.cfg";
	QFile file(path.c_str());
	file.open(QIODevice::ReadWrite);
	QTextStream stream(&file);
	while(!stream.atEnd())
	{
		ConnectInfo info;
		stream>>info.nameString;
		stream>>info.ipstring;
		stream>>info.port;
		if(info.nameString!=NULL)
		robotInfoList->append(info);

	}
	for(int i=0;i<robotInfoList->size();i++)
	{
		connectRobot=robotInfoList->value(i);
		robotList->addItem(connectRobot.nameString);
		robotList->setCurrentIndex(i);
		if (i==robotInfoList->size()-1)
		{MenuBar::getInstance()->connectAction->setToolTip(connectRobot.ipstring);
		}
	}
	file.close();
	
}
void ConnectWindow::ChangeIndex(int index)
{
	connectRobot=robotInfoList->value(index);
}
ConnectInfo ConnectWindow::GetCurrentInfo()
{
	return connectRobot;
}