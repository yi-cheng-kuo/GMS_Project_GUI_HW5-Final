#ifndef GROUP_H
#define GROUP_H

#include"component.h"
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Group
{
public:
    //建構Group
    Group(int id,string name,vector<int> members);
    ~Group();
    //取得ID
    int GetID();
    //取得Group名稱
    string GetName();
    //取得成員ID
    vector<int> GetMembersId();
    //取得字串型態的ID成員
    string GetMembersIdByStringFormat();
    //加入成員
    void AddMembers(vector<int> members);
    //加入單一成員 用在回復砍掉Component時用
    void AddSingleMember(int member);
    //確認MemberID是否存在
    bool CheckMemberHasBeenExisted(int memberId);
    //移除成員
    void RemoveMembers(vector<int> wantToRemoveId);
    //用在Delete Component時連帶移除Group中
    void RemoveSingleMember(int removeId);

    //GUI用 設定座標
    void SetDrawableData(float x,float y); //另一種設定方式
    void SetPositionX(float x);
    void SetPositionY(float Y);
    float GetPositionX();
    float GetPositionY();
    bool CheckBePressed(float x,float y);
    //確認有無被設定話圖的座標位置與寬高
    bool CheckBeSettedDrawableData();
private:
    int id;
    string groupName;
    vector<int> membersId;
    //排序Id
    void SortMembersById();
    float x,y;
};

#endif // GROUP_H
