#include "Gwen/Controls/TextBox.h"

#include "pixelboost/network/networkServer.h"

#include "view/ui/settings/networkWindow.h"
#include "core.h"

NetworkWindow::NetworkWindow(Gwen::Controls::Base* parent)
    : Gwen::Controls::WindowControl(parent)
{
    SetSize(230, 150);
    SetPos(250, 40);
    SetTitle("Device Connection");
    
    _AddressBox = new Gwen::Controls::TextBox(this);
    _AddressBox->SetText(Core::Instance()->GetNetworkManager()->GetClientHost());
    _AddressBox->SetPos(10, 10);
    _AddressBox->SetSize(200, 25);
    _AddressBox->onTextChanged.Add(this, &NetworkWindow::OnAddressChanged);
    
    int port = Core::Instance()->GetNetworkManager()->GetClientPort();
    if (port == 0)
        port = 9090;
    char portString[32];
    snprintf(portString, 32, "%d", port);
    _PortBox = new Gwen::Controls::TextBox(this);
    _PortBox->SetText(portString);
    _PortBox->SetPos(10, 45);
    _PortBox->SetSize(200, 25);
    _PortBox->onTextChanged.Add(this, &NetworkWindow::OnAddressChanged);
    
    _ReconnectButton = new Gwen::Controls::Button(this);
    _ReconnectButton->SetPos(10, 80);
    _ReconnectButton->SetSize(200, 25);
    _ReconnectButton->SetText("Connect");
    _ReconnectButton->onPress.Add(this, &NetworkWindow::OnConnect);
}

NetworkWindow::~NetworkWindow()
{
    
}

void NetworkWindow::OnAddressChanged(Gwen::Controls::Base* item)
{
    Core::Instance()->GetNetworkManager()->OpenClient(_AddressBox->GetValue().c_str(), atoi(_PortBox->GetValue().c_str()));
}

void NetworkWindow::OnConnect(Gwen::Controls::Base* item)
{
    Core::Instance()->GetNetworkManager()->OpenClient(Core::Instance()->GetNetworkManager()->GetClientHost(), Core::Instance()->GetNetworkManager()->GetClientPort());
}
