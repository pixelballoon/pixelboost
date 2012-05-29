#include "Gwen/Controls/TextBox.h"

#include "pixelboost/network/networkServer.h"

#include "view/ui/settings/networkWindow.h"
#include "core.h"

using namespace pixeleditor;

NetworkWindow::NetworkWindow(Gwen::Controls::Base* parent)
    : Gwen::Controls::WindowControl(parent)
{
    SetSize(230, 70);
    SetPos(250, 40);
    SetTitle("Device Address");
    
    Gwen::Controls::TextBox* textBox = new Gwen::Controls::TextBox(this);
    textBox->SetText("");
    textBox->SetPos(10, 10);
    textBox->SetSize(200, 25);
    textBox->onTextChanged.Add(this, &NetworkWindow::OnAddressChanged);
}

NetworkWindow::~NetworkWindow()
{
    
}

void NetworkWindow::OnAddressChanged(Gwen::Controls::Base* item)
{
    Core::Instance()->GetNetworkManager()->OpenClient(item->GetValue().c_str(), 9090);
}
