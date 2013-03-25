/*
#include "Gwen/Controls/ComboBox.h"
#include "Gwen/Controls/TextBox.h"

#include "core.h"
#include "project/project.h"
#include "project/schema.h"
#include "view/ui/create/createRecord.h"

CreateRecordWindow::CreateRecordWindow(Gwen::Controls::Base* parent)
    : Gwen::Controls::WindowControl(parent)
{
    SetSize(230, 150);
    SetPos(250, 40);
    SetTitle("Create Record");
    
    _NameField = new Gwen::Controls::TextBox(this);
    _NameField->SetText("");
    _NameField->SetPos(10, 10);
    _NameField->SetSize(200, 25);
    
    Schema* schema = Core::Instance()->GetProject()->GetSchema();
    
    _TypeField = new Gwen::Controls::ComboBox(this);
    _TypeField->SetPos(10, 45);
    _TypeField->SetSize(200, 25);
    for (Schema::RecordMap::const_iterator it = schema->GetRecords().begin(); it != schema->GetRecords().end(); ++it)
    {
        _TypeField->AddItem(Gwen::Utility::StringToUnicode(it->second->GetName().c_str()));
    }
    
    _CreateButton = new Gwen::Controls::Button(this);
    _CreateButton->SetPos(10, 80);
    _CreateButton->SetSize(200, 25);
    _CreateButton->SetText("Create");
    _CreateButton->onPress.Add(this, &CreateRecordWindow::OnCreate);
}

CreateRecordWindow::~CreateRecordWindow()
{
    
}

void CreateRecordWindow::OnCreate(Gwen::Controls::Base* item)
{
    if (_NameField->GetValue().length() && _TypeField->GetValue().length())
    {
        if (Core::Instance()->GetProject()->AddRecord(_NameField->GetValue(), _TypeField->GetValue()))
        {
            DelayedDelete();
        }
    }
}
*/
