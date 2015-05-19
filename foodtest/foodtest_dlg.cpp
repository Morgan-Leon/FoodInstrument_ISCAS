#include "foodtest_dlg.h"

FoodTest_Dlg::FoodTest_Dlg(QWidget *parent) :
        QDialog(parent,Qt::Dialog|Qt::FramelessWindowHint)
{    

    setupUi(this);
    this->Btn_Quit->setFocus();
    connect(this->Btn_Quit,SIGNAL(clicked()),this,SLOT(close()));
    if(Data::testType==serialTest)
        if(!serialCOMinit())
            hide();
    init();    
}

FoodTest_Dlg::~FoodTest_Dlg()
{
    db_food.close();
//    if(Data::testType==serialTest)
//    {
//        delete p_serialCOM2;
//	   }
//        delete p_sample_infoinput;
//    }
//    delete qTableModel;
}
void FoodTest_Dlg::GetList_FTestItems()             //��������Ŀ�б�
{
    QStringList fdlist;
    QSqlTableModel model;
    model.setTable("foodtestlist");
    model.setSort(0,Qt::AscendingOrder);
    model.select();
    while(model.canFetchMore())
        model.fetchMore();
    for(int row=0;row <model.rowCount();++row)
    {
        fdlist << model.record(row).value("name").toString();
    }
    this->comboBox_TestItems_CH1->clear();
    this->comboBox_TestItems_CH2->clear();
    this->comboBox_TestItems_CH3->clear();
    this->comboBox_TestItems_CH1->addItems(fdlist);
    this->comboBox_TestItems_CH2->addItems(fdlist);
    this->comboBox_TestItems_CH3->addItems(fdlist);
    this->comboBox_TestItems_CH1->setCurrentIndex(0);
    this->comboBox_TestItems_CH2->setCurrentIndex(0);
    this->comboBox_TestItems_CH3->setCurrentIndex(0);
}
void FoodTest_Dlg::GetList_SampleTypes(int CH_Num) //���뵱ǰ�����Ŀ����Ʒ����
{    
    QStringList splist;
    QSqlTableModel model;
    model.setTable("ftinfor");
    model.setSort(0,Qt::AscendingOrder);
    switch(CH_Num)
    {
    case 0:
        Num_TestItems[0]=this->comboBox_TestItems_CH1->currentText().trimmed();
        break;
    case 1:
        Num_TestItems[1]=this->comboBox_TestItems_CH2->currentText().trimmed();
        break;
    case 2:
        Num_TestItems[2]=this->comboBox_TestItems_CH3->currentText().trimmed();
        break;
    default:
        qDebug()<<"CH_Num error!";
        break;
    }
    model.setFilter(QObject::tr("ftestitems = '%1'").arg(Num_TestItems[CH_Num])); //QString("ftestitems=%1")
    model.select();
    while(model.canFetchMore())
        model.fetchMore();
    for(int row=0;row <model.rowCount();++row)
    {
        splist << model.record(row).value("fsample").toString();
    }
    switch(CH_Num)
    {
    case 0:
        this->comboBox_SampleTypes_CH1->clear();
        this->comboBox_SampleTypes_CH1->addItems(splist);
        this->comboBox_SampleTypes_CH1->setCurrentIndex(0);
        break;
    case 1:
        this->comboBox_SampleTypes_CH2->clear();
        this->comboBox_SampleTypes_CH2->addItems(splist);
        this->comboBox_SampleTypes_CH2->setCurrentIndex(0);
        break;
    case 2:
        this->comboBox_SampleTypes_CH3->clear();
        this->comboBox_SampleTypes_CH3->addItems(splist);
        this->comboBox_SampleTypes_CH3->setCurrentIndex(0);
        break;
    default:
        qDebug()<<"CH_Num error!";
        break;
    }
}

bool FoodTest_Dlg::serialCOMinit()
{
    bool ok=true;
    bool b_tmp;
    p_serialCOM2 =new serialCOM;
    p_serialPrint2 =new serialPrint;
    PrintInit();//////��ӡ����ʼ��
    ////connect
    b_tmp=p_serialCOM2->MultiCMD(online,CHEN123);
    ok &=b_tmp;
    if(!b_tmp)
    {
        QMessageBox::warning(this,tr("����"),tr("on line failed"),tr("ȷ��"));
        return ok;
    }
    b_tmp=p_serialCOM2->MultiCMD(readwave,CHEN123);
    ok &=b_tmp;
    if(!b_tmp)
    {
        QMessageBox::warning(this,tr("����"),tr("read wave list failed"),tr("ȷ��"));
        return ok;
    }
    return ok;
}

void FoodTest_Dlg::init()
{
    //init
    Num_ExtraDilute[0] =1;
    Num_ExtraDilute[1]=1;
    Num_ExtraDilute[2]=1;
    this->dateEdit->setDate(QDate::currentDate());
    this->dateEdit->setCalendarPopup(true);    
    db_food = QSqlDatabase::addDatabase("QSQLITE");
    db_food.setDatabaseName(DATABASENAME);
    if (!db_food.open())
    {
        QMessageBox::warning(this,tr("����"),tr("open database faile"),tr("ȷ��"));
        return;
    }
	this->labelDD1->hide();
    this->labelDD2->hide();
    this->labelDD3->hide();
    qTableModel = new QSqlTableModel(this);
    //-----¼�������ĿĿ¼������һ������Ŀ�µ���Ʒ����Ŀ¼
    GetList_FTestItems();
    //enable some thing,like buttons
    this->checkBox_CH1->setChecked(false);
    this->checkBox_CH2->setChecked(false);
    this->checkBox_CH3->setChecked(false);
 	this->Btn_IsZero->setEnabled(false);
    this->Btn_Measure->setEnabled(false);
    //test type
    if(Data::testType==noserialTest)
        bsetEnable(false);
    p_sample_infoinput = new sample_infoinput;
    connect(this->p_sample_infoinput,SIGNAL(transStrToDlg(struct Info_Sample)),this,SLOT(do_transStrToDlg(struct Info_Sample)));
    //display history data
    QString Strdate = this->dateEdit->date().toString("yyyy-MM-dd");
    display_QTable(Strdate);
    display_QTable(Strdate);    
}

void FoodTest_Dlg::display_QTable(QString Strdate)
{
    qTableModel->setTable("result");
    //    qTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    qTableModel->setSort(0,Qt::AscendingOrder);            //��0�У�AscendingOrder��������,Qt::DescendingOrderΪ��������
    qTableModel->setFilter(QString("time like '%1'").arg(Strdate+"%"));   //QObject::tr("ftestitems = '%1'")
    qTableModel->select();
    qTableModel->setHeaderData(0, Qt::Horizontal, tr("������"));
    qTableModel->setHeaderData(1, Qt::Horizontal, tr("�����Ŀ����"));  //QString::fromLocal8Bit("����")
    qTableModel->setHeaderData(2, Qt::Horizontal, tr("��Ʒ����"));
    qTableModel->setHeaderData(3, Qt::Horizontal, tr("����ͨ��"));
    qTableModel->setHeaderData(4, Qt::Horizontal, tr("�ж����"));
    qTableModel->setHeaderData(5, Qt::Horizontal, tr("�������"));
    qTableModel->setHeaderData(6, Qt::Horizontal, tr("�����λ"));
    qTableModel->setHeaderData(7, Qt::Horizontal, tr("�ο���׼"));
    qTableModel->setHeaderData(8, Qt::Horizontal, tr("����Ƚ��"));
    qTableModel->setHeaderData(9, Qt::Horizontal, tr("��Ʒ���"));
    qTableModel->setHeaderData(10, Qt::Horizontal, tr("��Ʒ����"));
    qTableModel->setHeaderData(11, Qt::Horizontal, tr("����"));
    qTableModel->setHeaderData(12, Qt::Horizontal, tr("�ͼ쵥λ"));
    qTableModel->setHeaderData(13, Qt::Horizontal, tr("������Ա"));
    qTableModel->setHeaderData(14, Qt::Horizontal, tr("���Ե�λ"));
    qTableModel->setHeaderData(15, Qt::Horizontal, tr("����ʱ��"));
    while(qTableModel->canFetchMore())
        qTableModel->fetchMore();
    //set header size
    QHeaderView *header;
    header =this->tableView->horizontalHeader();
    header->resizeSection(0,50);
    header->resizeSection(1,120);
    header->resizeSection(2,120);
    header->resizeSection(3,80);
    header->resizeSection(4,80);
    header->resizeSection(5,80);
    header->resizeSection(6,80);
    header->resizeSection(7,120);
    header->resizeSection(8,80);
    header->resizeSection(9,120);
    header->resizeSection(10,150);
    header->resizeSection(11,150);
    header->resizeSection(12,150);
    header->resizeSection(13,80);
    header->resizeSection(14,150);
    header->resizeSection(15,150);
    this->tableView->hideColumn(qTableModel->columnCount()<0);
    this->tableView->setCurrentIndex(qTableModel->index(qTableModel->rowCount()-1,5));
    //set some authority
    if(Data::userType==commonUser)
    {
        this->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        this->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
    this->tableView->setShowGrid(true);
    this->tableView->setModel(qTableModel);
}

void FoodTest_Dlg::do_transStrToDlg(struct Info_Sample FOODD)
{
    FSampleInfo = FOODD;
    FSample_Num[SampleInfo_CHNum] = FSampleInfo.Sample_Num;         //��Ʒ���
    FSample_Name[SampleInfo_CHNum]= FSampleInfo.Sample_Name;        //��Ʒ��λ
    FSample_From[SampleInfo_CHNum]= FSampleInfo.Sample_From;        //����
    FSample_Unit[SampleInfo_CHNum]= FSampleInfo.Sample_Unit;        //�ͼ쵥λ
    FTest_Name[SampleInfo_CHNum]= FSampleInfo.Test_Name;          //������
    FTest_Unit[SampleInfo_CHNum]= FSampleInfo.Sample_Unit;           //���Ե�λ
}
void FoodTest_Dlg::GetFoodInfo(int CH_Num)             //�õ�������Ϣ
{
    QSqlRecord record;
    QSqlTableModel model;
    model.setTable("ftinfor");
    model.setSort(0,Qt::AscendingOrder);
    switch(CH_Num)
    {
    case 0:
        Num_SampleTypes[0]=this->comboBox_SampleTypes_CH1->currentText().trimmed();
        break;
    case 1:
        Num_SampleTypes[1]=this->comboBox_SampleTypes_CH2->currentText().trimmed();
        break;
    case 2:
        Num_SampleTypes[2]=this->comboBox_SampleTypes_CH3->currentText().trimmed();
        break;
    default:
        qDebug()<<"CH_Num error!";
        break;
    }
    model.setFilter(QObject::tr("ftestitems = '%1' and fsample = '%2'").arg(Num_TestItems[CH_Num]).arg(Num_SampleTypes[CH_Num]));
    model.select();
    while(model.canFetchMore())
        model.fetchMore();
    if(model.rowCount()==1)
    {
        record =model.record(0);
    }
    else
    {        
        QMessageBox::warning(this,tr("����"),tr("Sample data has some problems"),tr("ȷ��"));
        return;
    }
    Data::Sign_DoubleWave[CH_Num] = record.value("wavemode").toInt();
    Data::WaveLengthMajor[CH_Num] = record.value("wavemajor").toFloat();
    Data::WaveLengthMinor[CH_Num] = record.value("waveminor").toFloat();
    Num_FormulaC[CH_Num] = record.value("formulaC").toFloat();
    Num_FormulaB[CH_Num] = record.value("formulaB").toFloat();
    Num_FormulaA[CH_Num] = record.value("formulaA").toFloat();
    Num_InterDilute[CH_Num] = record.value("interdilute").toFloat();
    Uint_TestItems[CH_Num] = record.value("unit").toString();
    Standards_TestItems[CH_Num] = record.value("standards").toString();
    FComparemode[CH_Num] = record.value("comparemode").toInt();
    FCompareMin[CH_Num] = record.value("comparemin").toFloat();
    FCompareMax[CH_Num] = record.value("comparemax").toFloat();
    FTestRangMax[CH_Num] = record.value("testrangemax").toFloat();
    FTestRangMin[CH_Num] = record.value("testrangemin").toFloat();
}

void FoodTest_Dlg::bsetEnable(bool bset)
{
    this->checkBox_CH1->setEnabled(false);
    this->checkBox_CH2->setEnabled(false);
    this->checkBox_CH3->setEnabled(false);
}
void FoodTest_Dlg::on_Btn_IsZero_clicked()
{
    srand((unsigned)time(0));
    int j;
 	QTime nntt;
    bool ok=true;
    bool b_tmp;
    int tmpFlag=Data::sign_TestCHNum;  //^Data::errorFlag;
    this->Btn_Measure->setEnabled(false);
   	this->Btn_IsZero->setEnabled(false);
        QMessageBox *messageBox=new QMessageBox(QMessageBox::Information,tr("�ȴ�"),tr("У���У����Ժ�"),QMessageBox::Ok,this);
    messageBox->show();
    if(Data::testType!=serialTest)
    {
        nntt.start();
        while(nntt.elapsed()<2000)
            QCoreApplication::processEvents();
    }
    if(Data::testType==serialTest)       ////serial test mode
    {
        Data::SetTime = 5;                   ////???????????????????????????
        b_tmp=p_serialCOM2->MultiCMD(settime,tmpFlag);
        ok &=b_tmp;
        if(!b_tmp)
        {
            QMessageBox::warning(this,tr("����"),tr("set time failed"),tr("ȷ��"));
            return;
        }
        ///��������
        b_tmp = p_serialCOM2->MultiCMD(readinit,CHEN123);
        if(!b_tmp)
            QMessageBox::warning(this,tr("����"),tr("read init failed"),tr("ȷ��"));
        for(j=0;j<3;j++)
        {
            if(tmpFlag&0x01)
            {
                if(Data::Sign_DoubleWave[j] != 3)
            	{
                Data::GetWaveLength = Data::WaveLengthMajor[j];
                b_tmp = p_serialCOM2->MultiCMD(readsigwave, j+1);///////////////////////////////////
                ok &=b_tmp;
                if(!b_tmp)
                {
                    QMessageBox::warning(this,tr("����"),tr("read BaseMajor failed"),tr("ȷ��"));
                    return;
                }
                Data::BaseMajor[j] = Data::GetADvalue;
                if(Data::Sign_DoubleWave[j] == 1)
                {
                    Data::GetWaveLength = Data::WaveLengthMinor[j];
                    b_tmp = p_serialCOM2->MultiCMD(readsigwave, j+1);///////////////////////////////////
                    ok &=b_tmp;
                    if(!b_tmp)
                    {
                        QMessageBox::warning(this,tr("����"),tr("read BaseMinor failed"),tr("ȷ��"));
                        return;
                    }
                    Data::BaseMinor[j] = Data::GetADvalue;
                }
            }
			}
            tmpFlag=tmpFlag>>1;
        }
        b_tmp = p_serialCOM2->MultiCMD(lightSW,lightoff);
        ok &=b_tmp;
        if(!b_tmp)
        {
            QMessageBox::warning(this,tr("����"),tr("light off failed"),tr("ȷ��"));
            return;
        }
        ///��������
        b_tmp = p_serialCOM2->MultiCMD(readinit, CHEN123);
        if(!b_tmp)
            QMessageBox::warning(this,tr("����"),tr("read init failed"),tr("ȷ��"));
        tmpFlag=Data::sign_TestCHNum;//^Data::errorFlag;
        for(j=0;j<3;j++)
        {
            if(tmpFlag&0x01)
			{
                if(Data::Sign_DoubleWave[j] != 3)
            	{
                Data::GetWaveLength = Data::WaveLengthMajor[j];
                b_tmp = p_serialCOM2->MultiCMD(readsigwave, j+1);///////////////////////////////////
                ok &=b_tmp;
                if(!b_tmp)
                {
                    QMessageBox::warning(this,tr("����"),tr("read DarkMajor failed"),tr("ȷ��"));
                    return;
                }
                Data::DarkMajor[j] = Data::GetADvalue;
                if(Data::Sign_DoubleWave[j] == 1)
                {
                    Data::GetWaveLength = Data::WaveLengthMinor[j];
                    b_tmp = p_serialCOM2->MultiCMD(readsigwave, j+1);///////////////////////////////////
                    ok &=b_tmp;
                    if(!b_tmp)
                    {
                        QMessageBox::warning(this,tr("����"),tr("read DarkMinor failed"),tr("ȷ��"));
                        return;
                    }
                    Data::DarkMinor[j] = Data::GetADvalue;
					}
                }
            }
            tmpFlag=tmpFlag>>1;
        }
        b_tmp = p_serialCOM2->MultiCMD(lightSW,lighton);
        ok &=b_tmp;
        if(!b_tmp)
        {
            QMessageBox::warning(this,tr("����"),tr("light on failed"),tr("ȷ��"));
            return;
        }
    }
    else if(Data::testType==showType)       ////show mode
    {
        for(j=0;j<3;j++)
        {
            if(tmpFlag&0x01)
            {
                Data::BaseMajor[j] = (rand()%(2048-800))+800;            //???????????????????????
                Data::DarkMajor[j] = (rand()%(300-100))+100;            //???????????????????????
                if(Data::Sign_DoubleWave[j] == 1)
                {
                    Data::BaseMinor[j] = (rand()%(2048-1000))+1000;            //???????????????????????
                    Data::DarkMinor[j] = (rand()%(300-100))+100;            //???????????????????????
                }
            }
            tmpFlag=tmpFlag>>1;
        }
    }
    this->Btn_Measure->setEnabled(ok);
    this->Btn_IsZero->setEnabled(ok);
    if(ok)
        this->Btn_Measure->setFocus();
	messageBox->close();
    delete messageBox;
}
void FoodTest_Dlg::on_Btn_Measure_clicked()
{
    srand((unsigned)time(0));
    QSqlQuery query;
    QString Strdate;
	QTime nntt;
    bool ok=true;
    bool b_tmp;
    int SELsum;//ѡ������
    int tmpFlag=Data::sign_TestCHNum;//^Data::errorFlag;
    QMessageBox *messageBox=new QMessageBox(QMessageBox::Information,tr("�ȴ�"),tr("�����У����Ժ�"),QMessageBox::Ok,this);
    messageBox->show();
    this->Btn_Measure->setEnabled(false);
    this->Btn_IsZero->setEnabled(false);
    if(Data::testType!=serialTest)
    {
        nntt.start();
        while(nntt.elapsed()<1000)
            QCoreApplication::processEvents();
    }		        
    ///��������
    if(Data::testType==serialTest)       ////serial test mode
    {
        b_tmp = p_serialCOM2->MultiCMD(readinit, CHEN123);
        if(!b_tmp)
            QMessageBox::warning(this,tr("����"),tr("read init failed"),tr("ȷ��"));
    }
    for(int j=0;j<3;j++)
    {
        if(tmpFlag&0x01)
        {
            if(Data::testType==serialTest)       ////serial test mode
            {
                if(Data::Sign_DoubleWave[j] != 3)
            {
                Data::GetWaveLength = Data::WaveLengthMajor[j];
                b_tmp = p_serialCOM2->MultiCMD(readsigwave, j+1);///////////////////////////////////
                ok &=b_tmp;
                if(!b_tmp)
                {
                    QMessageBox::warning(this,tr("����"),tr("read SampleMajor failed"),tr("ȷ��"));
                    return;
                }
                Data::SampleMajor[j] = Data::GetADvalue;
				}
            }
            else if(Data::testType==showType)      ////show mode
            {
                Data::SampleMajor[j]=(rand()%(1800-800))+800;             //???????????????????????
            }
            AbsMajor[j] =  -log10((float)(Data::SampleMajor[j] - Data::DarkMajor[j])/(float)(Data::BaseMajor[j] - Data::DarkMajor[j]));
            AbsSampleTest[j] = AbsMajor[j];
            if(Data::Sign_DoubleWave[j] == 1)
            {
                if(Data::testType==serialTest)        ////serial test mode
                {
                    Data::GetWaveLength = Data::WaveLengthMinor[j];
                    b_tmp = p_serialCOM2->MultiCMD(readsigwave, j+1);///////////////////////////////////
                    ok &=b_tmp;
                    if(!b_tmp)
                    {
                        QMessageBox::warning(this,tr("����"),tr("read SampleMinor failed"),tr("ȷ��"));
                        return;
                    }
                    Data::SampleMinor[j] = Data::GetADvalue;
                }
                else if(Data::testType==showType)       ////show mode
                {
                    Data::SampleMinor[j] = (rand()%(2000-800))+800;             //???????????????????????
                }
                AbsMinor[j] =  -log10((float)(Data::SampleMinor[j] - Data::DarkMinor[j])/(float)(Data::BaseMinor[j] - Data::DarkMinor[j]));
                AbsSampleTest[j] = AbsMajor[j] - AbsMinor[j];
            }
            if(AbsSampleTest[j]>10)
            {
                AbsSampleTest[j] = 10;
            }
            else if(AbsSampleTest[j]<-10)
            {
                AbsSampleTest[j] = -10;
            }
			if(Data::Sign_DoubleWave[j] != 3)
            {
            	Num_content[j] = Num_ExtraDilute[j]*Num_InterDilute[j]*(AbsSampleTest[j]*AbsSampleTest[j]*Num_FormulaC[j] + AbsSampleTest[j]*Num_FormulaB[j] + Num_FormulaA[j]);       //������
			}
            else
            {
                AbsSampleTest[j] = Num_ExtraDilute[j];
                Num_content[j] = Num_InterDilute[j]*(AbsSampleTest[j]*AbsSampleTest[j]*Num_FormulaC[j] + AbsSampleTest[j]*Num_FormulaB[j] + Num_FormulaA[j]);//�ζ�����
            }
            if(Num_content[j] < 0)
            {
                Num_content[j] = 0;
            }
            if(Num_content[j] < FTestRangMin[j])
            {
                FTestResult[j] = tr("Ũ�ȹ���");
            }
            else if(Num_content[j] > FTestRangMax[j])
            {
                FTestResult[j] = tr("Ũ�ȹ���");
            }
            else
            {
                if(FComparemode[j] == 0)
                {
                    FTestResult[j] = tr("�ޱ�׼");
                }
                else if(FComparemode[j] == 1)
                {
                    if(Num_content[j] < FCompareMin[j])
                    {
                        FTestResult[j] = tr("�ϸ�");
                    }
                    else
                    {
                        FTestResult[j] = tr("���ϸ�");
                    }
                }
                else if(FComparemode[j] == 2)
                {
                    if(Num_content[j] > FCompareMax[j])
                    {
                        FTestResult[j] = tr("�ϸ�");
                    }
                    else
                    {
                        FTestResult[j] = tr("���ϸ�");
                    }
                }
                else if(FComparemode[j] == 3)
                {
                    if((Num_content[j] > FCompareMin[j])&&(Num_content[j] < FCompareMax[j]))
                    {
                        FTestResult[j] = tr("�ϸ�");
                    }
                    else
                    {
                        FTestResult[j] = tr("���ϸ�");
                    }
                }
                else if(FComparemode[j] == 4)
                {
                    if((Num_content[j] < FCompareMin[j])||(Num_content[j] > FCompareMax[j]))
                    {
                        FTestResult[j] = tr("�ϸ�");
                    }
                    else
                    {
                        FTestResult[j] = tr("���ϸ�");
                    }
                }
            }
            //insert one data
            //            int rowNum=qTableModel->rowCount();
            //            qTableModel->insertRow(rowNum);
            //            qTableModel->setData(qTableModel->index(rowNum,1),Num_TestItems[j]);
            //            qTableModel->setData(qTableModel->index(rowNum,2),Num_SampleTypes[j]);
            //            qTableModel->setData(qTableModel->index(rowNum,3),QString::number(j+1));
            //            qTableModel->setData(qTableModel->index(rowNum,4),FTestResult[j]);
            //            qTableModel->setData(qTableModel->index(rowNum,5),QString::number(Num_content[j],'f',1));
            //            qTableModel->setData(qTableModel->index(rowNum,6),Uint_TestItems[j]);
            //            qTableModel->setData(qTableModel->index(rowNum,7),Standards_TestItems[j]);
            //            qTableModel->setData(qTableModel->index(rowNum,8),QString::number(AbsSampleTest[j],'f',3));
            //            qTableModel->setData(qTableModel->index(rowNum,9),FSample_Num[j]);
            //            qTableModel->setData(qTableModel->index(rowNum,10),FSample_Name[j]);
            //            qTableModel->setData(qTableModel->index(rowNum,11),FSample_From[j]);
            //            qTableModel->setData(qTableModel->index(rowNum,12),FSample_Unit[j]);
            //            qTableModel->setData(qTableModel->index(rowNum,13),FTest_Name[j]);
            //            qTableModel->setData(qTableModel->index(rowNum,14),FTest_Unit[j]);
            //            qTableModel->setData(qTableModel->index(rowNum,15),(QDateTime::currentDateTime()).toString("yyyy-MM-dd hh:mm:ss"));
            //            qTableModel->submitAll();

            query.exec("INSERT INTO result VALUES (NULL,'"+Num_TestItems[j]+"','"+Num_SampleTypes[j]+"', "
                       " '"+QString::number(j+1)+"', "
                       " '"+FTestResult[j]+"', "
                       " '"+QString::number(Num_content[j],'f',1)+"', "
                       " '"+Uint_TestItems[j]+"', "
                       " '"+Standards_TestItems[j]+"', "
                       " '"+QString::number(AbsSampleTest[j],'f',3)+"', "
                       " '"+FSample_Num[j]+"', "
                       " '"+FSample_Name[j]+"', "
                       " '"+FSample_From[j]+"', "
                       " '"+FSample_Unit[j]+"', "
                       " '"+FTest_Name[j]+"', "
                       " '"+FTest_Unit[j]+"', "
                       " '"+(QDateTime::currentDateTime()).toString("yyyy-MM-dd hh:mm:ss")+"')");
        }
        tmpFlag=tmpFlag>>1;
        Strdate = this->dateEdit->date().toString("yyyy-MM-dd");
        display_QTable(Strdate);


    }
    messageBox->close();
    delete messageBox;
    this->Btn_Measure->setEnabled(ok);
    this->Btn_IsZero->setEnabled(ok);
}
void FoodTest_Dlg::on_comboBox_TestItems_CH1_currentIndexChanged(QString )
{
    //¼��ü����Ŀ�µ���Ʒ����Ŀ¼
    if (this->comboBox_TestItems_CH1->currentText().trimmed()!="")
    {
        GetList_SampleTypes(0);
        this->Btn_Measure->setEnabled(false);
    }
    if(Data::Sign_DoubleWave[0] == 3)
    {
        this->labelDD1->show();
    }
    else
    {
        this->labelDD1->hide();
        Num_ExtraDilute[0] = 1;
    }
}

void FoodTest_Dlg::on_comboBox_TestItems_CH2_currentIndexChanged(QString )
{
    //¼��ü����Ŀ�µ���Ʒ����Ŀ¼
    if (this->comboBox_TestItems_CH2->currentText().trimmed()!="")
    {
        GetList_SampleTypes(1);
        this->Btn_Measure->setEnabled(false);
    }
	 if(Data::Sign_DoubleWave[1] == 3)
    {
        this->labelDD2->show();
    }
    else
    {
        this->labelDD2->hide();
        Num_ExtraDilute[1] = 1;
    }
}
void FoodTest_Dlg::on_comboBox_TestItems_CH3_currentIndexChanged(QString )
{
    //¼��ü����Ŀ�µ���Ʒ����Ŀ¼
    if (this->comboBox_TestItems_CH3->currentText().trimmed()!="")
    {
        GetList_SampleTypes(2);
        this->Btn_Measure->setEnabled(false);
    }
    if(Data::Sign_DoubleWave[2] == 3)
    {
        this->labelDD3->show();
    }
    else
    {
        this->labelDD3->hide();
        Num_ExtraDilute[2] = 1;
    }
}

void FoodTest_Dlg::on_comboBox_SampleTypes_CH1_currentIndexChanged(QString )
{
    if (this->comboBox_SampleTypes_CH1->currentText().trimmed()!="")
        GetFoodInfo(0);
}

void FoodTest_Dlg::on_comboBox_SampleTypes_CH2_currentIndexChanged(QString )
{
    if (this->comboBox_SampleTypes_CH2->currentText().trimmed()!="")
        GetFoodInfo(1);
}

void FoodTest_Dlg::on_comboBox_SampleTypes_CH3_currentIndexChanged(QString )
{
    if (this->comboBox_SampleTypes_CH3->currentText().trimmed()!="")
        GetFoodInfo(2);
}


void FoodTest_Dlg::on_checkBox_CH1_stateChanged(int )
{
    if(this->checkBox_CH1->isChecked())
        Data::sign_TestCHNum |=0x01;
    else
        Data::sign_TestCHNum &=0x06;
    this->comboBox_TestItems_CH1->setEnabled(this->checkBox_CH1->isChecked());
    this->comboBox_SampleTypes_CH1->setEnabled(this->checkBox_CH1->isChecked());
    this->DiluteNum_CH1->setEnabled(this->checkBox_CH1->isChecked());
    this->Btn_SampleInfo_CH1->setEnabled(this->checkBox_CH1->isChecked());
    if( Data::sign_TestCHNum == 0)
    {
        this->Btn_IsZero->setEnabled(false);
        this->Btn_Measure->setEnabled(false);
    }
    else
    {
        this->Btn_IsZero->setEnabled(true);
        this->Btn_Measure->setEnabled(false);
    }
}

void FoodTest_Dlg::on_checkBox_CH2_stateChanged(int )
{
    if(this->checkBox_CH2->isChecked())
        Data::sign_TestCHNum |=0x02;
    else
        Data::sign_TestCHNum &=0x05;
    this->comboBox_TestItems_CH2->setEnabled(this->checkBox_CH2->isChecked());
    this->comboBox_SampleTypes_CH2->setEnabled(this->checkBox_CH2->isChecked());
    this->DiluteNum_CH2->setEnabled(this->checkBox_CH2->isChecked());
    this->Btn_SampleInfo_CH2->setEnabled(this->checkBox_CH2->isChecked());
    if( Data::sign_TestCHNum == 0)
    {
        this->Btn_IsZero->setEnabled(false);
        this->Btn_Measure->setEnabled(false);
    }
    else
    {
        this->Btn_IsZero->setEnabled(true);
        this->Btn_Measure->setEnabled(false);
    }
}

void FoodTest_Dlg::on_checkBox_CH3_stateChanged(int )
{
    if(this->checkBox_CH3->isChecked())
        Data::sign_TestCHNum |=0x04;
    else
        Data::sign_TestCHNum &=0x03;
    this->comboBox_TestItems_CH3->setEnabled(this->checkBox_CH3->isChecked());
    this->comboBox_SampleTypes_CH3->setEnabled(this->checkBox_CH3->isChecked());
    this->DiluteNum_CH3->setEnabled(this->checkBox_CH3->isChecked());
    this->Btn_SampleInfo_CH3->setEnabled(this->checkBox_CH3->isChecked());
    if( Data::sign_TestCHNum == 0)
    {
        this->Btn_IsZero->setEnabled(false);
        this->Btn_Measure->setEnabled(false);
    }
    else
    {
        this->Btn_IsZero->setEnabled(true);
        this->Btn_Measure->setEnabled(false);
    }

}

void FoodTest_Dlg::on_Btn_SampleInfo_CH1_clicked()
{
    SampleInfo_CHNum = 0;
    p_sample_infoinput->show();
}

void FoodTest_Dlg::on_Btn_SampleInfo_CH2_clicked()
{
    SampleInfo_CHNum = 1;
    p_sample_infoinput->show();
}

void FoodTest_Dlg::on_Btn_SampleInfo_CH3_clicked()
{
    SampleInfo_CHNum = 2;
    p_sample_infoinput->show();
}

void FoodTest_Dlg::on_DiluteNum_CH1_editingFinished()
{
    //    QRegExp DiluteNum_CH1("\w[0-9]");
    Num_ExtraDilute[0] = this->DiluteNum_CH1->text().toFloat();
}

void FoodTest_Dlg::on_DiluteNum_CH2_editingFinished()
{
    //    QRegExp DiluteNum_CH2("\w[0-9]");
    Num_ExtraDilute[1] = this->DiluteNum_CH2->text().toFloat();
}

void FoodTest_Dlg::on_DiluteNum_CH3_editingFinished()
{
    //    QRegExp DiluteNum_CH3("^[0-9]$");
    Num_ExtraDilute[2] = this->DiluteNum_CH3->text().toFloat();
}
void FoodTest_Dlg::mydelay(int needtime)
{
    ////wait
    QTime nt;
    nt.start();
    while(nt.elapsed()<needtime)
        QCoreApplication::processEvents();
}

void FoodTest_Dlg::on_Btn_Del_clicked()
{
    QItemSelectionModel *selections = tableView->selectionModel();
    QModelIndexList selecteds;
    selecteds = selections->selectedRows();
    int ok = QMessageBox::warning(this,tr("ɾ��ѡ�е���!"),tr("��ȷ��ɾ����ǰѡȡ�е�����?"),QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::Yes)
    {
        foreach (QModelIndex index, selecteds)
        {
            int curRow = index.row();
            qTableModel->removeRow(curRow);
        }
        qTableModel->submitAll();
    }
    else
    {
        qTableModel->revertAll();
    }
}

void FoodTest_Dlg::on_Btn_Display_clicked()
{
    QString Strdate = this->dateEdit->date().toString("yyyy-MM-dd");
    display_QTable(Strdate);
}

void FoodTest_Dlg::on_Btn_Print_clicked()
{
    QItemSelectionModel *selections = tableView->selectionModel();
    QModelIndexList selecteds;
    selecteds = selections->selectedRows();
    PrintString("  ʳƷ��ȫ���ټ����");
    Printchar(0x0A);
    foreach (QModelIndex index, selecteds)
    {
        int curRow = index.row();
        PrintString("�����Ŀ��"+qTableModel->record(curRow).value("prjname").toString());
        Printchar(0x0A);
        PrintString("��Ʒ���ͣ�"+qTableModel->record(curRow).value("sampletype").toString());
        Printchar(0x0A);
        PrintString("ͨ����"+qTableModel->record(curRow).value("detectchn").toString());
        PrintString("�����"+qTableModel->record(curRow).value("judgeresult").toString());
        Printchar(0x0A);
        PrintString(qTableModel->record(curRow).value("prjresult").toString()+qTableModel->record(curRow).value("unit").toString());
        Printchar(0x0A);
        PrintString("����ʱ�䣺"+qTableModel->record(curRow).value("time").toString());
        Printchar(0x0A);
    }
}
void FoodTest_Dlg::Printchar(char Pdata1)
{
    qDebug()<<Pdata1 ;
    p_serialPrint2->Printchar(Pdata1);
}
void FoodTest_Dlg::PrintString(QString Pdata2)
{
    qDebug()<<Pdata2 ;
    p_serialPrint2->PrintString(Pdata2);
}
void FoodTest_Dlg::PrintInit(void)
{
    /*
    ��ӡ���ô��ڳ�ʼ��������9600
    */
    Printchar(0x1b);	//   ��ӡ����ʼ��
    Printchar(0x40);

    Printchar(0x1b);     //   ��ӡ�������ӡ
    Printchar(0x63);
    Printchar(0x0);

    Printchar(0x1c);	//�򿪺�����ʾģʽ
    Printchar(0x26);

    Printchar(0x1B);     //����Ŵ�
    Printchar(0x55);
    Printchar(0x02);

}
