#ifndef ADDNEWGROUPCOMMAND_H
#define ADDNEWGROUPCOMMAND_H

#include <command.h>
#include <string>
#include "group.h"
#include "groups.h"
#include "gmsModel.h"
#include "constants.h"
using namespace std;

//新建Group的指令
class AddNewGroupCommand :public Command
{
public:
    //建構子 取得執行指令所需的資料
    AddNewGroupCommand(GMSModel* model,string name, vector<int> members);
    //新增Group,同時包含新增時顯示在圖上的座標寬高等資訊
    AddNewGroupCommand(GMSModel* model,string name, vector<int> members,DrawableData data);
    ~AddNewGroupCommand();
    //執行指令
    void execute();
    //反執行指令
    void unexecute();
private:
    //取得Groups (Receiver) 用來做相關操作(透過Model)
    GMSModel* model;
    //要加入的Group Name
    string addName;
    //要加入的成員
    vector<int> addMembers;
    //新建Group時給予的編號
    int addedId;
    int x,y; //繪製座標
};

#endif // ADDNEWGROUPCOMMAND_H
