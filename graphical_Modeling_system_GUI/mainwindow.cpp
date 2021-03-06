#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1024,700);
    view = new DrawView(&gms,this); //取得GMSModel

    //為了畫線而記錄輸入名字時用的,設定為空的
    //因為有可能使用者沒有點完畫線的兩個座標就又去選別的指令
    lineName = "";

    //如果Window中只有一個主要原件,可使用,如果有多數元件,如加入ListWidget等 就要移掉此行
    //this->setCentralWidget(ui->drawViewScrollArea);
    ui->drawViewScrollArea->setWidget(view);

    //透過此行讓放在ScrollArea中的Widget可以與scrollArea一起縮放,不過便不會有Scrollbar的出現
    //scrollArea.setWidgetResizable(true);

    //設定ScrollArea的固定高寬 讓他不要跟著Widget一起變動,然後再設定卷軸
    //Widget再設定更大的寬高(要用SetFixedSize才行) 既可以產生卷軸
    ui->drawViewScrollArea->setFixedSize(690,600);
    //設定滾軸的位置
    ui->drawViewScrollArea->horizontalScrollBar()->setValue(0);
    ui->drawViewScrollArea->verticalScrollBar()->setValue(0);

    //設定ListWidget的標頭資料
    ui->componentsListWidget->addItem(QString::fromStdString(Constants::GUIMainWindowParameter::COMPONENTS_LIST_WIDGET_HEADER));
    ui->groupsListWidget->addItem(QString::fromStdString(Constants::GUIMainWindowParameter::GROUPS_LIST_WIDGET_HEADER));

    //Register synchronous event(SINGAL & SLOT) to close MainWindow
    //by menubar
    QObject::connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    //open FileDialog
    QObject::connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(OnOpenFileButtonClicked()));
    //save
    QObject::connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(OnSaveFileButtonClicked()));
    //AddComponent
    QObject::connect(ui->actionAddCube,SIGNAL(triggered()),this,SLOT(OnAddCubeComponentClicked()));
    QObject::connect(ui->actionAddPyramid,SIGNAL(triggered()),this,SLOT(OnAddPyramidComponentClicked()));
    QObject::connect(ui->actionAddSphere,SIGNAL(triggered()),this,SLOT(OnAddSphereComponentClicked()));
    QObject::connect(ui->actionAddLine,SIGNAL(triggered()),this,SLOT(OnAddLineComponentClicked()));
    //Redo Undo
    QObject::connect(ui->actionRedo,SIGNAL(triggered()),this,SLOT(OnRedoClicked()));
    QObject::connect(ui->actionUndo,SIGNAL(triggered()),this,SLOT(OnUndoClicked()));
    //AddGroup
    QObject::connect(ui->actionAddGroup,SIGNAL(triggered()),this,SLOT(OnAddGroupClicked()));

    view->update();

}

MainWindow::~MainWindow()
{
    delete ui;
}
//更新Component ListWidget的資料
void MainWindow::UpdateComponentListWidget(vector<Component*> componentList){
    stringstream ss;
    ui->componentsListWidget->clear();
    ui->componentsListWidget->addItem(QString::fromStdString(Constants::GUIMainWindowParameter::COMPONENTS_LIST_WIDGET_HEADER));
    vector<Component*> components = componentList;
    for(vector<Component*>::iterator it = components.begin();it != components.end();it++){
         //取得字串
        ss << (*it)->GetType()[0] << " , " << (*it)->GetID() << " , " << (*it)->GetName();
        ui->componentsListWidget->addItem(QString::fromStdString(ss.str()));
        //清空stringstream的資料
        ss.str(""); ss.flush();
    }
}
//更新Group ListWidget的資料
void MainWindow::UpdateGroupListWidget(map<string,Group*> groupList){
    stringstream ss;
    ui->groupsListWidget->clear();
    ui->groupsListWidget->addItem(QString::fromStdString(Constants::GUIMainWindowParameter::GROUPS_LIST_WIDGET_HEADER));
    map<string,Group*> groups = groupList;
    for(map<string,Group*>::iterator it = groups.begin();it != groups.end();it++){
        //取得字串
        ss << it->first.c_str() << " , " << (it->second)->GetName() << " , " << (it->second)->GetMembersIdByStringFormat();
        ui->groupsListWidget->addItem(QString::fromStdString(ss.str()));
        //清空stringstream的資料
        ss.str(""); ss.flush();
    }
}

//SLOT : When Click OpenFolder Action Control,it will have a slot OnOpenFileButtonClicked() and go here.
void MainWindow::OnOpenFileButtonClicked(){
   view->SetBeAddedLineComannd(false); //設定沒有要加入線段指令(已經離開加入線段的步驟,所以取消)
    QDir fileDir(QDir::currentPath());
    fileDir.cd("../" + QString::fromStdString(Constants::GUIMainWindowParameter::XML_FILES_PATH));
    fileName = QFileDialog::getOpenFileName(this,tr("Open GMS XML File"),
                                            fileDir.path(),tr("XML Files (*.xml)"));
    //載入檔案
    int code = gms.LoadXMLFormatRecord(fileName.toLocal8Bit().constData());
    //有讀取成功
    if(code == XMLErrorCode::OK){
        //Components
        if(this->gms.GetComponents().GetAllComponent().size() > 0){
            //更新顯示在ListWidget上的資料
            this->UpdateComponentListWidget(this->gms.GetComponents().GetAllComponent());
        }
        //Groups
        if(this->gms.GetGroups().GetAllGroups().size() >0){
            //更新顯示在ListWidget上的資料
            this->UpdateGroupListWidget(this->gms.GetGroups().GetAllGroups());
        }
        //印出載入資料(GUI程式會在應用程式輸出畫面顯示)
        this->gms.OutputComponentsDataByConsole();
        this->gms.OutputGroupsDataByConsole();
    }

    //設定出示繪圖座標
    view->SetLoadedGMSFileDrawPostion();
    //更新畫面
    view->update();
}
//存檔案(尚未測試) 先保留
void MainWindow::OnSaveFileButtonClicked(){
    view->SetBeAddedLineComannd(false); //設定沒有要加入線段指令(已經離開加入線段的步驟,所以取消)
    QDir fileDir(QDir::currentPath());
    fileDir.cd("../" + QString::fromStdString(Constants::GUIMainWindowParameter::XML_FILES_PATH));
    fileName = QFileDialog::getSaveFileName(this,tr("Open GMS XML File"),
                                            fileDir.path(),tr("XML Files (*.xml)"));
    gms.SaveXMLFormatRecord(fileName.toStdString());

}

void MainWindow::OnAddCubeComponentClicked(){
    view->SetBeAddedLineComannd(false); //設定沒有要加入線段指令(已經離開加入線段的步驟,所以取消)
    AddComponentDialog dialog;
    dialog.setModal(true);
    dialog.exec();
    if(!dialog.GetInputText().empty()){
        view->AddComponentDrawablePositionInfo(Constants::ComponentType::CubeTypeString,dialog.GetInputText());
        //印出載入資料(GUI程式會在應用程式輸出畫面顯示)
        this->gms.OutputComponentsDataByConsole();
        //更新畫面

        //更新顯示在ListWidget上的資料
        this->UpdateComponentListWidget(this->gms.GetComponents().GetAllComponent());

        view->update();
    }
    cout << "\nGet input :" << dialog.GetInputText() << endl;


}
void MainWindow::OnAddPyramidComponentClicked(){
    view->SetBeAddedLineComannd(false); //設定沒有要加入線段指令(已經離開加入線段的步驟,所以取消)
    AddComponentDialog dialog;
    dialog.setModal(true);
    dialog.exec();
    if(!dialog.GetInputText().empty()){
       view->AddComponentDrawablePositionInfo(Constants::ComponentType::PyramidTypeString,dialog.GetInputText());
        //印出載入資料(GUI程式會在應用程式輸出畫面顯示)
        this->gms.OutputComponentsDataByConsole();
        //更新畫面

        //更新顯示在ListWidget上的資料
        this->UpdateComponentListWidget(this->gms.GetComponents().GetAllComponent());

        view->update();
    }
    cout << "\nGet input :" << dialog.GetInputText() << endl;
}
void MainWindow::OnAddSphereComponentClicked(){
    view->SetBeAddedLineComannd(false); //設定沒有要加入線段指令(已經離開加入線段的步驟,所以取消)
    AddComponentDialog dialog;
    dialog.setModal(true);
    dialog.exec();
    if(!dialog.GetInputText().empty()){
      view->AddComponentDrawablePositionInfo(Constants::ComponentType::SphereTypeString,dialog.GetInputText());
        //印出載入資料(GUI程式會在應用程式輸出畫面顯示)
        this->gms.OutputComponentsDataByConsole();
        //更新畫面

        //更新顯示在ListWidget上的資料
        this->UpdateComponentListWidget(this->gms.GetComponents().GetAllComponent());

        view->update();
    }
    cout << "\nGet input :" << dialog.GetInputText() << endl;

}
void MainWindow::OnAddLineComponentClicked(){
    AddComponentDialog dialog;
    dialog.setModal(true);
    dialog.exec();
    if(!dialog.GetInputText().empty() ){
        lineName = dialog.GetInputText();
        //告知view要加入線段
        view->SetBeAddedLineComannd(true);
    }
    cout << "\nGet input :" << dialog.GetInputText() << endl;

}
//尚未測試
void MainWindow::OnRedoClicked(){
    view->SetBeAddedLineComannd(false); //設定沒有要加入線段指令(已經離開加入線段的步驟,所以取消)
    this->gms.Redo();
    //更新顯示在ListWidget上的資料
    this->UpdateComponentListWidget(this->gms.GetComponents().GetAllComponent());
    //更新顯示在ListWidget上的資料
    this->UpdateGroupListWidget(this->gms.GetGroups().GetAllGroups());

    view->update();
}
//尚未測試
void MainWindow::OnUndoClicked(){
    view->SetBeAddedLineComannd(false); //設定沒有要加入線段指令(已經離開加入線段的步驟,所以取消)
    this->gms.Undo();
    //更新顯示在ListWidget上的資料
    this->UpdateComponentListWidget(this->gms.GetComponents().GetAllComponent());
    //更新顯示在ListWidget上的資料
    this->UpdateGroupListWidget(this->gms.GetGroups().GetAllGroups());

    view->update();
}
void MainWindow::OnGetDrawLinePoints(QPoint start,QPoint end){
    DrawableData data;
    data.x = start.x(); data.y = start.y(); data.width = -1; data.height = -1; data.x2 = end.x(); data.y2 = end.y();
    gms.AddDrawableComponentsByCommand(Constants::ComponentType::LineTypeString,lineName,data);
    lineName = "";
    //印出載入資料(GUI程式會在應用程式輸出畫面顯示)
    this->gms.OutputComponentsDataByConsole();
    //更新畫面

    //更新顯示在ListWidget上的資料
    this->UpdateComponentListWidget(this->gms.GetComponents().GetAllComponent());
    //設定出示繪圖座標
    view->update();

    view->SetBeAddedLineComannd(false); //完成指令所以關閉
}
void MainWindow::OnWantedEditComponentbeSelected(int wantEditId){
    view->SetBeAddedLineComannd(false); //設定沒有要加入線段指令(已經離開加入線段的步驟,所以取消)
    EditComponentDialog dialog;
    dialog.setModal(true);
    dialog.exec();
    //輸入文字,沒已選類型
    if(!dialog.GetInputNameText().empty() && dialog.GetTypeText() == "Not Edit" ){
        gms.EditComponentNameByCommand(wantEditId,dialog.GetInputNameText());
    }
    //輸入類型沒有選文字
    else if(dialog.GetInputNameText().empty() && dialog.GetTypeText() != "Not Edit" ){
         gms.EditComponentTypeByCommand(wantEditId,dialog.GetTypeText());
    }
    //輸入類型與選文字
    else if(!dialog.GetInputNameText().empty() && dialog.GetTypeText() != "Not Edit" ){
         gms.EditComponentNameAndTypeByCommand(wantEditId,dialog.GetInputNameText(),dialog.GetTypeText());
    }
    cout << "\nGet input :" << dialog.GetInputNameText() << endl;
    cout << "\nGet type :" << dialog.GetTypeText() << endl;

    //印出載入資料(GUI程式會在應用程式輸出畫面顯示)
    this->gms.OutputComponentsDataByConsole();
    //更新畫面

    //更新顯示在ListWidget上的資料
    this->UpdateComponentListWidget(this->gms.GetComponents().GetAllComponent());
    //設定出示繪圖座標
    view->update();
}

void MainWindow::OnAddGroupClicked(){
    AddGroupDialog dialog;
    //用來判斷書入的component id有無錯誤
    dialog.SetComponents(gms.GetComponents());
    dialog.setModal(true);
    dialog.exec();
    if(!dialog.GetGroupNameText().empty() && dialog.GetAddMembersId().size() >0){
        //給予DrawView
        view->AddGroupDrawablePositionInfo(dialog.GetGroupNameText(),dialog.GetAddMembersId());

        //印出載入資料(GUI程式會在應用程式輸出畫面顯示)
        this->gms.OutputGroupsDataByConsole();
        //更新畫面

        //更新顯示在ListWidget上的資料
        this->UpdateGroupListWidget(this->gms.GetGroups().GetAllGroups());

        //設定出示繪圖座標
        view->update();
    }
}
