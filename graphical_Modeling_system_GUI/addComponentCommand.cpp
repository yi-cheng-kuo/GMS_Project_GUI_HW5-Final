#include "addComponentCommand.h"

AddComponentCommand::AddComponentCommand(GMSModel* model,string componentType, string componentName)
{
    this->addedId = 0;
    this->addComponentType = componentType;
    this->AddName = componentName;
    this->model = model;
    this->width = this->height = this->x1 = this->x2 = this->y1 = this->y2 = -1;

}
AddComponentCommand::AddComponentCommand(GMSModel* model, string componentType, string componentName, DrawableData data){
    this->addedId = 0;
    this->addComponentType = componentType;
    this->AddName = componentName;
    this->model = model;
    //設定繪製的資料
    this->width = data.width;
    this->height = data.height;
    this->x1 = data.x;
    this->y1 = data.y;
    this->x2 = data.x2;
    this->y2 = data.y2;
}



AddComponentCommand::~AddComponentCommand(){

}
void AddComponentCommand::execute(){
    model->AddComponents(addComponentType,AddName,&addedId);
    Component* addComponent = model->GetComponents().GetComponentById(addedId);
    //如果是線段,用另外的方法加入
    if(addComponent->GetType() == Constants::ComponentType::LineTypeString){
        addComponent->SetLinePosition(x1,y1,x2,y2);
    }
    else{ //否則是一般元件,設定左上角座標與寬高
        addComponent->SetPositionX(x1);
        addComponent->SetPositionY(y1);
        addComponent->SetWidth(width);
        addComponent->SetHeight(height);
    }
}
void AddComponentCommand::unexecute(){
    model->UnDoAddComponents(addedId);
}
