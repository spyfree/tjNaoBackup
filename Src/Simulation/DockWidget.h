#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H
#include <QDockWidget>
#include <QAbstractItemModel>
#include <QTreeView>
#include <QModelIndex>
#include <QStringListModel>
#include <QVariant>
#include "MotionWindow.h"
class DockWidget :public QDockWidget
{
	Q_OBJECT
public:
	DockWidget(QWidget *parent=0);
	DockWidget(const QString & title, QWidget *parent=0);
};
//TreeItem is like a dir
class TreeItem
{
public:
	TreeItem(const QVector<QVariant> &data, TreeItem *parent = 0);
	~TreeItem();
	bool AddChildren(int position ,int rowcount,int columnscount );
	bool RemoveChildren(int posion,int rowcount);
	int RowCount();
	int columnCount() const;
	QVariant GetData(int columnscount);
	TreeItem *parent();
	TreeItem *children(int row);
	bool SetData(int columns,const QVariant &value);
	QList<TreeItem*>childItems;
private: 
	QVector<QVariant>itemData;
	
	TreeItem *parentItem;
};
class TreeModel:public QAbstractItemModel
{
	Q_OBJECT
public:
	TreeModel(const QStringList&header,const QString &data,
		QObject *parent = 0);
	~TreeModel();
	int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
	int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
	QVariant data(const QModelIndex&index,int role= Qt::DisplayRole)const;
private:
	bool setData(const QModelIndex &index, const QVariant &value,int role = Qt::EditRole);
	QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
	TreeItem* itemFromIndex(const QModelIndex &index)const ;

	 QModelIndex parent ( const QModelIndex & index ) const ;
	 TreeItem *rootItem;
	 	void setupModelData(const QStringList &lines, TreeItem *parent);
	QVariant headerData ( int section, Qt::Orientation orientation, 
		int role = Qt::DisplayRole ) const ;
	 Qt::ItemFlags flags(const QModelIndex &index) const;
	 bool insertRows(int position, int rows,
		 const QModelIndex &parent = QModelIndex());
};
class Tree:public DockWidget
{	
	Q_OBJECT
public:
	Tree(QWidget*parent=0);
	void SetModel(TreeModel &model);
signals:
	void MotionSeen(QString str );
	void ParentNodeStr(QString str);
private slots:
	void itemActivatedSlot(const QModelIndex& index);
private:
	void CreatConnect();
	QTreeView *treeView;
	TreeModel *treeModel;
	QStringListModel *model;
};
#endif