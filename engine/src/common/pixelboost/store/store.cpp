#include "pixelboost/debug/assert.h"
#include "pixelboost/store/store.h"

using namespace pb;

#if defined(PIXELBOOST_PLATFORM_IOS) or (PIXELBOOST_PLATFORM_OSX)
#include <StoreKit/StoreKit.h>

@interface StorePaymentDelegate : NSObject <SKPaymentTransactionObserver>
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
- (void)paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray *)transactions;
- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error;
- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue;
- (void)paymentQueue:(SKPaymentQueue *)queue updatedDownloads:(NSArray *)downloads;
@end

@implementation StorePaymentDelegate
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    
}
- (void)paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray *)transactions
{
    
}
- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    
}
- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    
}
- (void)paymentQueue:(SKPaymentQueue *)queue updatedDownloads:(NSArray *)downloads
{
    
}
@end

#endif

#if defined(PIXELBOOST_PLATFORM_ANDROID)

#endif

Store* Store::_Instance = 0;

Store::Store()
{
    PbAssert(_Instance == 0);
    _Instance = this;
    
#if defined(PIXELBOOST_PLATFORM_IOS) or (PIXELBOOST_PLATFORM_OSX)
    _Delegate = [StorePaymentDelegate new];
#endif
}

Store::~Store()
{
#if defined(PIXELBOOST_PLATFORM_IOS) or (PIXELBOOST_PLATFORM_OSX)
    [_Delegate release];
#endif
    
    _Instance = 0;
}

Store* Store::Instance()
{
    return _Instance;
}

std::vector<StoreTransaction> Store::GetPendingTransactions()
{
    return _Pending;
}
