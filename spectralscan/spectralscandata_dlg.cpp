#include "spectralscandata_dlg.h"


SpectralScanData_Dlg::SpectralScanData_Dlg(QWidget *parent) :
    QDialog(parent,Qt::Dialog|Qt::FramelessWindowHint)
{    
    setupUi(this);
    this->Btn_Quit->setFocus();
    this->dateEdit->setDate(QDate::currentDate());
    this->dateEdit->setCalendarPopup(true);
    if (!createConnection())
        QMessageBox::warning(this,tr("警告"),tr("read specscan.db failed"),tr("确定"));
    qTableModel = new QSqlTableModel(this);
    //display history data
    QString Strdate = this->dateEdit->date().toString("yyyy-MM-dd");
    mydelay(1000);
    display_QTable(Strdate);
    display_QTable(Strdate);
    display_QTable(Strdate);
}

SpectralScanData_Dlg::~SpectralScanData_Dlg()
{
    db_specscan.close();
    delete qTableModel;
}

bool SpectralScanData_Dlg::createConnection()
{
    db_specscan = QSqlDatabase::addDatabase("QSQLITE");
    db_specscan.setDatabaseName(SPECBASENAME);
    if (!db_specscan.open())
    {
        QMessageBox::warning(this,tr("警告"),tr("open database faile"),tr("确定"));
        return false;
    }
    return true;
}

void SpectralScanData_Dlg::display_QTable(QString Strdate)
{
    qTableModel->setTable("specscan");
    //    qTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    qTableModel->setSort(0,Qt::AscendingOrder);            //第0列，AscendingOrder升序排列,Qt::DescendingOrder为降序排序
    qTableModel->setFilter(QString("time like '%1'").arg(Strdate+"%"));   //QObject::tr("ftestitems = '%1'")
    qTableModel->select();
    qTableModel->setHeaderData(0, Qt::Horizontal, tr("检测序号"));
    qTableModel->setHeaderData(1, Qt::Horizontal, tr("检测通道数"));  //QString::fromLocal8Bit("卡号")
    qTableModel->setHeaderData(2, Qt::Horizontal, tr("检测模式"));
    qTableModel->setHeaderData(3, Qt::Horizontal, tr("起始波长"));
    qTableModel->setHeaderData(4, Qt::Horizontal, tr("终止波长"));
    qTableModel->setHeaderData(5, Qt::Horizontal, tr("Y轴最大值"));
    qTableModel->setHeaderData(6, Qt::Horizontal, tr("Y轴最小值"));
    qTableModel->setHeaderData(7, Qt::Horizontal, tr("CCD积分时间"));
    qTableModel->setHeaderData(8, Qt::Horizontal, tr("测试次数"));
    qTableModel->setHeaderData(9, Qt::Horizontal, tr("波峰波谷阈值"));
    qTableModel->setHeaderData(10, Qt::Horizontal, tr("测试时间"));
    while(qTableModel->canFetchMore())
        qTableModel->fetchMore();
    //set header size
    QHeaderView *header;
    header =this->tableView->horizontalHeader();
    header->resizeSection(0,80);
    header->resizeSection(1,80);
    header->resizeSection(2,80);
    header->resizeSection(3,80);
    header->resizeSection(4,80);
    header->resizeSection(5,80);
    header->resizeSection(6,80);
    header->resizeSection(7,100);
    header->resizeSection(8,80);
    header->resizeSection(9,100);
    header->resizeSection(10,150);
    this->tableView->hideColumn(qTableModel->columnCount()>10);
    this->tableView->setCurrentIndex(qTableModel->index(qTableModel->rowCount()-1,0));
    //set some authority
    if(Data::userType==commonUser)
    {
        this->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        this->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
    this->tableView->setShowGrid(true);
    this->tableView->setModel(qTableModel);
}

void SpectralScanData_Dlg::on_Btn_Display_clicked()
{
    QString Strdate = this->dateEdit->date().toString("yyyy-MM-dd");
    display_QTable(Strdate);
}

void SpectralScanData_Dlg::on_Btn_Del_clicked()
{
    QStringList splist;
    splist.clear();
    QSqlTableModel model;
    model.setTable("specscan");
    model.setSort(0,Qt::AscendingOrder);   //DescendingOrder
    model.select();
    while(model.canFetchMore())
        model.fetchMore();
    //del
    QItemSelectionModel *selections = tableView->selectionModel();
    QModelIndexList selecteds;
    selecteds = selections->selectedRows();
    int ok = QMessageBox::warning(this,tr("删除选中的行!"),tr("你确定删除当前选中的行吗?"),QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::Yes)
    {
        foreach (QModelIndex index, selecteds)
        {
            int curRow = index.row();
            splist << model.record(curRow-1).value("id").toString();
            qTableModel->removeRow(curRow);
        }
        qTableModel->submitAll();
    }
    else
    {
        qTableModel->revertAll();
    }
    //del result relation data
    QSqlTableModel mode2;
    mode2.setTable("result");
    for(int i=0; i<splist.size();i++)
    {
        mode2.setSort(0,Qt::AscendingOrder);   //DescendingOrder
        mode2.setFilter(QObject::tr("id = '%1'").arg(splist.at(i))); //QString("ftestitems=%1")
        mode2.select();
        while(mode2.canFetchMore())
            mode2.fetchMore();
        for(int j=0;j<mode2.rowCount();j++)
            mode2.removeRow(j);
        mode2.submitAll();
    }
}

void SpectralScanData_Dlg::on_Btn_Dis_clicked()
{
    int tmpID;
    int curRow = -1;
    curRow = this->tableView->currentIndex().row();
    if(curRow<0)
         QMessageBox::warning(this,tr("警告"),tr("请选中回放数据"),tr("确定"));

    else
    {
        int ok = QMessageBox::warning(this,tr("删除选中的行!"),tr("是要回放当前选中行的测试数据吗?"),QMessageBox::Yes,QMessageBox::No);
        if(ok == QMessageBox::Yes)
        {
            tmpID = qTableModel->record(curRow).value("id").toInt();
            hide();
            emit transIntToDlg(tmpID);
        }
    }
}

void SpectralScanData_Dlg::on_Btn_Quit_clicked()
{
    hide();
}

void SpectralScanData_Dlg::mydelay(int needtime)
{
    ////wait
    QTime nt;
    nt.start();
    while(nt.elapsed()<needtime)
        ;
    //        QCoreApplication::processEvents();
}
