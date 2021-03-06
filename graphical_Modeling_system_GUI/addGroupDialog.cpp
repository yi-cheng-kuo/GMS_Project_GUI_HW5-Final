#include "addGroupDialog.h"
#include "ui_addGroupDialog.h"

AddGroupDialog::AddGroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddGroupDialog)
{
    ui->setupUi(this);
    groupName = "";

    this->setWindowFlags(Qt::WindowCloseButtonHint); //顯示只要關閉的按鈕在右上角即可,如此可以移除問號
    QObject::connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(OnButtonAccepted()));
    QObject::connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(close()));

}

AddGroupDialog::~AddGroupDialog()
{
    delete ui;
}
void AddGroupDialog::SetComponents(Components components){
    this->compareComponents = components;
}
vector<int> AddGroupDialog::GetAddMembersId(){
    return this->members;
}
string AddGroupDialog::GetGroupNameText(){
    return this->groupName;
}
void AddGroupDialog::OnButtonAccepted(){
    bool isInputGroupNameError = false; //是否有輸入錯誤
    bool isComponentIDNotExisted = false; //是否有不存在ComponentId
    bool isErrorAddMemberFormatInput = false; //是否輸入的格式錯誤
    //檢查ID是否存在於Components
    vector <int> notExistComponentsIDList;
    //檢查Group Name的輸入
    CheckIsInputGroupName(&isInputGroupNameError);

    //檢查Member Id的輸入
    if(!ui->addMemberTextEdit->text().isEmpty()){
        string inputMemberStr = CheckInputMemberStrFormat(&isErrorAddMemberFormatInput);
        //輸入格式正確
        if(!isErrorAddMemberFormatInput){
            //處理輸入的ID是否存在
           notExistComponentsIDList =  HandleInputMembersIdIsExisted(inputMemberStr,&isComponentIDNotExisted);
        }
    }
    else{
        isErrorAddMemberFormatInput = true;
    }
    //顯示訊息
    ShowInputErrorMessage(isInputGroupNameError,isErrorAddMemberFormatInput,
                          isComponentIDNotExisted,notExistComponentsIDList);
}

void AddGroupDialog::CheckIsInputGroupName(bool* isInputGroupNameError){
    //檢查Group Name的輸入
    if(!ui->addGroupNameTextEdit->text().isEmpty()){
        string temp = ui->addGroupNameTextEdit->text().toStdString();
        temp.erase(remove(temp.begin(),temp.end(),' '),temp.end()); //清除不必要的空白
        if(temp.size() >0){
            this->groupName = ui->addGroupNameTextEdit->text().toStdString();
        }
        else{
            *isInputGroupNameError = true;
        }
    }
    else{
        *isInputGroupNameError = true;
    }
}
string AddGroupDialog::CheckInputMemberStrFormat(bool *IsInputFormatCorrect){
    string inputMemberStr = ui->addMemberTextEdit->text().toStdString();
    //清除不必要的空白
    inputMemberStr.erase(remove(inputMemberStr.begin(),inputMemberStr.end(),' '),inputMemberStr.end());
    //檢查有無輸入錯誤
    for(unsigned int index =0 ;index <inputMemberStr.size(); index++){
        if( inputMemberStr[index] == ',')
            inputMemberStr[index] = ' ' ;
        if(isalpha(inputMemberStr[index])){
            *IsInputFormatCorrect = true;
            break;
       }
    }
    return inputMemberStr;
}

vector<int> AddGroupDialog::HandleInputMembersIdIsExisted(string inputMemberStr, bool* IsComponentNotExisted){
    vector<int> wantAddMembersId; //使用者想要加入的MembersID
    //檢查ID是否存在於Components
    vector <int> notExistComponentsIdList;

    stringstream ss;
    ss << inputMemberStr;
    int memberId;
    while(ss >>memberId){
       //放入想要加入的ID vector
        wantAddMembersId.push_back(memberId);
    }
    //判斷輸入的ID是否是存在的Component
    for(vector<int>::iterator it = wantAddMembersId.begin();it != wantAddMembersId.end();it++){
        if(!compareComponents.CheckIDHasBeenExisted( *it ) ) //不存在
            notExistComponentsIdList.push_back(*it );
    }
    if(notExistComponentsIdList.size() >1){
        *IsComponentNotExisted = true;
    }
    else{
        //沒有問題,加入component ID
        for(vector<int>::iterator it = wantAddMembersId.begin();it != wantAddMembersId.end();it++){
            this->members.push_back(*it);
        }
    }
    return notExistComponentsIdList;
}

void AddGroupDialog::ShowInputErrorMessage(bool groupName, bool inputComponentFormatError, bool componentsNotExist,vector<int> notExistComponents){
    string outputMessage = "";
    if( (groupName || inputComponentFormatError || componentsNotExist) ){
        if(groupName)
            outputMessage += "You haven't input groupName\n";
        if(inputComponentFormatError)
            outputMessage += "Input Member format error!\n";
        if(componentsNotExist){
            stringstream ss;

            //判斷輸入的ID是否是存在的Component
            for(vector<int>::iterator it = notExistComponents.begin();it != notExistComponents.end();it++){
                ss << (*it) << " ";
            }
            outputMessage += "The id:" + ss.str() + "not existed";
        }
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText(QString::fromStdString(outputMessage));
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}
