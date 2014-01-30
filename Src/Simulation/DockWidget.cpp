#include "DockWidget.h"
#include <Platform/GetSetting.h>
#include <QFile>
DockWidget::DockWidget(QWidget *parent/* =0 */):QDockWidget(parent)
{
	setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
}
DockWidget::DockWidget(const QString & title, QWidget *parent):QDockWidget(title, parent)
{
	//setAllowedAreas(Qt::TopDockWidgetArea| Qt::BottomDockWidgetArea);
}
TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent /* = 0 */)
{
	itemData=data;
	parentItem=parent;
}


TreeItem::~TreeItem()
{
	qDeleteAll(childItems);
}

bool TreeItem::AddChildren(int position ,int rowcount,int columnscount )
{
	if(position<0||position>childItems.size())
		return false;
	for(int row=0;row<rowcount;row++)
	{
		QVector<QVariant> data(columnscount);
		TreeItem *item=new TreeItem(data,this);
		childItems.insert(position,item);
	}
	return true;
}

bool TreeItem::RemoveChildren(int posion,int rowcount)
{
	if(posion<0||(posion+rowcount)>childItems.size())
		return false;
	childItems.move(posion,posion+rowcount);
	return true;

}
int TreeItem::RowCount()
{
	return childItems.size();
}
int TreeItem::columnCount() const
{
	return itemData.count();
}
TreeItem *TreeItem::parent()
{
	return parentItem;

}
TreeItem *TreeItem::children(int row)
{
	return childItems.value(row);
}
bool TreeItem::SetData(int columns, const QVariant &value)
{
	if (columns < 0 || columns >= itemData.count())
		return false;
	itemData[columns]=value;
		return true;

}
QVariant TreeItem::GetData(int columnscount)
{
	if(columnscount<0||columnscount>itemData.size())
	return false;
	else
	return itemData.value(columnscount);
}



TreeModel::TreeModel(const QStringList&header,const QString &data, QObject *parent /* = 0 */)
	:QAbstractItemModel(parent)
{
	QVector<QVariant> rootData;
	foreach(QString head,header)
		rootData<<head;
	rootItem=new TreeItem(rootData);
	setupModelData(data.split(QString("\n")),rootItem);

}
TreeModel:: ~TreeModel()
{
delete rootItem;
}
QModelIndex TreeModel::parent( const QModelIndex & index )const
{
	if(!index.isValid())
		return QModelIndex();
	TreeItem *childItem = itemFromIndex(index);
	TreeItem *parentItem = childItem->parent();
	TreeItem *grandparentItem=parentItem->parent();
	if (parentItem == rootItem)
		return QModelIndex();
	int row=grandparentItem->childItems.indexOf(parentItem);
	return createIndex(row, 0, parentItem);
}
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (parent.isValid() && parent.column() != 0)
		return QModelIndex();
	TreeItem *parentItem = itemFromIndex(parent);

	TreeItem *childItem = parentItem->children(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value,
						int role)
{
	if (role != Qt::EditRole)
		return false;

	TreeItem *item = itemFromIndex(index);
	bool result = item->SetData(index.column(), value);

	if (result)
		emit dataChanged(index, index);

	return result;
}

TreeItem *TreeModel::itemFromIndex(const QModelIndex &index)const
{
	if (index.isValid()) {
		TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
		if (item) return item;
	}
	return rootItem;
}
int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem = itemFromIndex(parent);

	return parentItem->RowCount();
}
int TreeModel::columnCount(const QModelIndex & parent )const
{
	return 1;
}
QVariant TreeModel::data(const QModelIndex&index,int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	TreeItem *item = itemFromIndex(index);

	return item->GetData(index.column());
}

QVariant TreeModel::headerData( int section, Qt::Orientation orientation, int role )const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->GetData(section);

	return QVariant();
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem *parentItem = itemFromIndex(parent);
	bool success;

	beginInsertRows(parent, position, position + rows - 1);
	success = parentItem->AddChildren(position, rows, rootItem->columnCount());
	endInsertRows();

	return success;
}
void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
	QList<TreeItem*> parents;
	QList<int> indentations;
	parents << parent;
	indentations << 0;
	int number = 0;

	while (number < lines.count()) {
		int position = 0;
		while (position < lines[number].length()) {
			if (lines[number].mid(position, 1) != " ")
				break;
			position++;
		}

		QString lineData = lines[number].mid(position).trimmed();

		if (!lineData.isEmpty()) {
			// Read the column data from the rest of the line.
			QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
			QVector<QVariant> columnData;
			for (int column = 0; column < columnStrings.count(); ++column)
				columnData << columnStrings[column];

			if (position > indentations.last()) {
				// The last child of the current parent is now the new parent
				// unless the current parent has no children.

				if (parents.last()->RowCount() > 0) {
					parents << parents.last()->children(parents.last()->RowCount()-1);
					indentations << position;
				}
			} else {
				while (position < indentations.last() && parents.count() > 0) {
					parents.pop_back();
					indentations.pop_back();
				}
			}

			// Append a new item to the current parent's list of children.
			TreeItem *parent = parents.last();
			parent->AddChildren(parent->RowCount(), 1, rootItem->columnCount());
			for (int column = 0; column < columnData.size(); ++column)
				parent->children(parent->RowCount() - 1)->SetData(column, columnData[column]);
		}

		number++;
	}
}

Qt::ItemFlags TreeModel:: flags(const QModelIndex& index) const
{
	if(!index.isValid())
		return 0;
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

Tree::Tree(QWidget*parent/* =0 */):DockWidget(parent)
{
	treeView=new QTreeView;
	QStringList header;
	header << tr("Tree") ;
	QFile file(QString(GetSetting::GetConfigDir())+"/TreeConfig"+"/default.txt");
	file.open(QIODevice::ReadOnly);
	treeModel= new TreeModel(header, file.readAll());
	file.close();
	treeView->setModel(treeModel);
	setWidget(treeView);
	CreatConnect();
}
void Tree::CreatConnect()
{
connect(treeView, SIGNAL(doubleClicked( const QModelIndex &)), this, SLOT(itemActivatedSlot(const QModelIndex&)));
}

void Tree::itemActivatedSlot(const QModelIndex& index)
{
	QString str=treeModel->data(index).toString();
	emit MotionSeen(str);

}
void Tree::SetModel(TreeModel &model)
{
	treeModel=&model;
}
