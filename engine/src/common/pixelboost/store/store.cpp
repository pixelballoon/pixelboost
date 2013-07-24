#include "pixelboost/debug/assert.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/store/store.h"

using namespace pb;

int g_PaymentsPending = 0;

#if defined(PIXELBOOST_PLATFORM_IOS) or (PIXELBOOST_PLATFORM_OSX)

#include <StoreKit/StoreKit.h>

@interface StorePaymentDelegate : NSObject <SKPaymentTransactionObserver, SKRequestDelegate, SKProductsRequestDelegate>
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
- (void)paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray *)transactions;
- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error;
- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue;
- (void)paymentQueue:(SKPaymentQueue *)queue updatedDownloads:(NSArray *)downloads;
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response;
@end

@implementation StorePaymentDelegate
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction* transaction in transactions)
    {
        std::string identifier = [transaction.payment.productIdentifier UTF8String];
        
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                g_PaymentsPending--;
            case SKPaymentTransactionStateRestored:
            {
                if (Store::Instance()->OnItemPurchased(identifier))
                {
                    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                }

                break;
            }
            case SKPaymentTransactionStatePurchasing:
            {
                g_PaymentsPending++;
                break;
            }
            case SKPaymentTransactionStateFailed:
            {
                g_PaymentsPending--;
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];;
                break;
            }                
        }
    }
}
- (void)paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray *)transactions
{
    
}
- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    g_PaymentsPending--;
}
- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    g_PaymentsPending--;
}
- (void)paymentQueue:(SKPaymentQueue *)queue updatedDownloads:(NSArray *)downloads
{
    
}
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    for(id product in response.products)
    {
        NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
        [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
        [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
        [numberFormatter setLocale:[(SKProduct*)product priceLocale]];
        NSString *priceString = [numberFormatter stringFromNumber:[(SKProduct*)product price]];
        
        [product retain];
        StoreItem item([[(SKProduct*)product productIdentifier] UTF8String], [[(SKProduct*)product productIdentifier] UTF8String], [priceString UTF8String]);
        item._Product = (SKProduct*)product;
        
        Store::Instance()->AddItem(item);
    }
}
@end

#endif

#if defined(PIXELBOOST_PLATFORM_ANDROID)

#endif

StoreItem::StoreItem(const std::string& identifier, const std::string& description, const std::string& priceString, bool consumable)
{
    _Identifier = identifier;
    _Description = description;
    _PriceString = priceString;
    _Consumable = consumable;
}

StoreItem::~StoreItem()
{
    
}

const std::string& StoreItem::GetIdentifier() const
{
    return _Identifier;
}

const std::string& StoreItem::GetDescription() const
{
    return _Description;
}

const std::string& StoreItem::GetPriceString() const
{
    return _PriceString;
}

bool StoreItem::IsConsumable() const
{
    return _Consumable;
}

Store* Store::_Instance = 0;

Store::Store()
{
    PbAssert(_Instance == 0);
    _Instance = this;
    
#if defined(PIXELBOOST_PLATFORM_IOS) or (PIXELBOOST_PLATFORM_OSX)
    _Delegate = [StorePaymentDelegate new];
    
    [[SKPaymentQueue defaultQueue] addTransactionObserver:_Delegate];
#endif
}

Store::~Store()
{
#if defined(PIXELBOOST_PLATFORM_IOS) or (PIXELBOOST_PLATFORM_OSX)
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:_Delegate];
    
    [_Delegate release];
#endif
    
    _Instance = 0;
}

Store* Store::Instance()
{
    return _Instance;
}

bool Store::ArePaymentsAllowed()
{
#if defined(PIXELBOOST_PLATFORM_IOS) or (PIXELBOOST_PLATFORM_OSX)
    return [SKPaymentQueue canMakePayments];
#endif
    
    return false;
}

void Store::AddIdentifier(const std::string& identifier, std::function<void(void)> purchaseCallback)
{
    _Identifiers.push_back(identifier);
    _Callbacks[identifier] = purchaseCallback;
}

void Store::RefreshItems()
{
#if defined(PIXELBOOST_PLATFORM_IOS) or (PIXELBOOST_PLATFORM_OSX)
    NSSet *productList = [NSSet set];
    
    for (std::string identifier : _Identifiers)
    {
        productList = [productList setByAddingObject:[NSString stringWithUTF8String:identifier.c_str()]];
    }
    
    SKProductsRequest* request = [[SKProductsRequest alloc] initWithProductIdentifiers:productList];
    request.delegate = _Delegate;
    [request start];
#endif
}

StoreItem* Store::GetStoreItem(const std::string& identifier)
{
    return _Items[identifier];
}

void Store::RestorePurchases()
{
#if defined(PIXELBOOST_PLATFORM_IOS) or (PIXELBOOST_PLATFORM_OSX)
    g_PaymentsPending++;
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
#endif
}

void Store::PurchaseItem(const StoreItem& item)
{
#if defined(PIXELBOOST_PLATFORM_IOS) or (PIXELBOOST_PLATFORM_OSX)
    if (ArePaymentsAllowed())
    {
        [[SKPaymentQueue defaultQueue] addPayment:[SKPayment paymentWithProduct:item._Product]];
    }
#endif
}

void Store::AddItem(const StoreItem& item)
{
    _Items[item.GetIdentifier()] = new StoreItem(item);
}

bool Store::OnItemPurchased(const std::string& identifier)
{
    auto it = _Callbacks.find(identifier);
    
    if (it == _Callbacks.end())
        return false;
    
    it->second();
    
    return true;
}
