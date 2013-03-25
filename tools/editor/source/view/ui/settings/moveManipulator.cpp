/*
#include "Gwen/Controls/TextBox.h"

#include "pixelboost/network/networkServer.h"

#include "view/manipulator/manipulator.h"
#include "view/manipulator/move.h"
#include "view/ui/settings/moveManipulator.h"
#include "view.h"

MoveManipulatorWindow::MoveManipulatorWindow(Gwen::Controls::Base* parent)
    : Gwen::Controls::WindowControl(parent)
{
    SetSize(350, 100);
    SetPos(250, 40);
    SetTitle("Move Manipulator");
    
    Gwen::Controls::Label* label = new Gwen::Controls::Label(this);
    label->SetText("Snap");
    label->SetSize(label->GetSize().x, 20);
    
    MoveManipulator* manipulator = static_cast<MoveManipulator*>(View::Instance()->GetManipulatorManager()->GetManipulator("move"));
    
    char buffer[100];
    
    sprintf(buffer,"%f", manipulator->GetSnap().x);
    _SnapX = new Gwen::Controls::TextBox(this);
    _SnapX->SetText(buffer);
    _SnapX->SetPos(10, 20);
    _SnapX->SetSize(100, 25);
    _SnapX->onTextChanged.Add(this, &MoveManipulatorWindow::OnSnapChanged);
    
    sprintf(buffer,"%f", manipulator->GetSnap().y);
    _SnapY = new Gwen::Controls::TextBox(this);
    _SnapY->SetText(buffer);
    _SnapY->SetPos(120, 20);
    _SnapY->SetSize(100, 25);
    _SnapY->onTextChanged.Add(this, &MoveManipulatorWindow::OnSnapChanged);
    
    sprintf(buffer,"%f", manipulator->GetSnap().z);
    _SnapZ = new Gwen::Controls::TextBox(this);
    _SnapZ->SetText(buffer);
    _SnapZ->SetPos(230, 20);
    _SnapZ->SetSize(100, 25);
    _SnapZ->onTextChanged.Add(this, &MoveManipulatorWindow::OnSnapChanged);
}

MoveManipulatorWindow::~MoveManipulatorWindow()
{
    
}

void MoveManipulatorWindow::OnSnapChanged(Gwen::Controls::Base* item)
{
    float snapX = atof(_SnapX->GetValue().c_str());
    float snapY = atof(_SnapY->GetValue().c_str());
    float snapZ = atof(_SnapZ->GetValue().c_str());
    
    MoveManipulator* manipulator = static_cast<MoveManipulator*>(View::Instance()->GetManipulatorManager()->GetManipulator("move"));
    manipulator->SetSnap(glm::vec3(snapX, snapY, snapZ));
}
*/
