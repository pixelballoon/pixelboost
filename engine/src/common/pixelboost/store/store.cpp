#include "glm/glm.hpp"

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
        StoreItem item([[(SKProduct*)product productIdentifier] UTF8String], "", [priceString UTF8String]);
        item._Product = (SKProduct*)product;
        
        Store::Instance()->AddItem(item);
    }
}
@end

#endif

#if defined(PIXELBOOST_PLATFORM_ANDROID)

#include "pixelboost/misc/jni.h"

#if defined(PIXELBOOST_ANDROID_STORE_GOOGLEPLAY)

extern "C" {
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_GoogleHelpers_onProductReceived(JNIEnv * env, jobject obj, jstring javaSku, jstring javaPrice);
    JNIEXPORT jboolean JNICALL Java_com_pixelballoon_pixelboost_GoogleHelpers_onProductPurchased(JNIEnv * env, jobject obj, jstring javaSku);
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_GoogleHelpers_onProductPurchaseFailed(JNIEnv * env, jobject obj, jstring javaSku);
    JNIEXPORT jboolean JNICALL Java_com_pixelballoon_pixelboost_GoogleHelpers_onIsConsumable(JNIEnv * env, jobject obj, jstring javaSku);
    JNIEXPORT jboolean JNICALL Java_com_pixelballoon_pixelboost_GoogleHelpers_onIsPurchasable(JNIEnv * env, jobject obj, jstring javaSku);
};

JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_GoogleHelpers_onProductReceived(JNIEnv * env, jobject obj, jstring javaSku, jstring javaPrice)
{
    const char* sku = env->GetStringUTFChars(javaSku, 0);
    const char* price = env->GetStringUTFChars(javaPrice, 0);

    StoreItem item(sku, "", price);    
    Store::Instance()->AddItem(item);

    env->ReleaseStringUTFChars(javaSku, sku);
    env->ReleaseStringUTFChars(javaPrice, price);
}

JNIEXPORT jboolean JNICALL Java_com_pixelballoon_pixelboost_GoogleHelpers_onProductPurchased(JNIEnv * env, jobject obj, jstring javaSku)
{
    jboolean success = false;

    const char* sku = env->GetStringUTFChars(javaSku, 0);

    if (sku && Store::Instance() && Store::Instance()->OnItemPurchased(sku))
    {
        success = true;
    }

    env->ReleaseStringUTFChars(javaSku, sku);

    g_PaymentsPending = glm::max(g_PaymentsPending-1, 0);

    return success;
}

JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_GoogleHelpers_onProductPurchaseFailed(JNIEnv * env, jobject obj, jstring javaSku)
{
    g_PaymentsPending = glm::max(g_PaymentsPending-1, 0);
}

JNIEXPORT jboolean JNICALL Java_com_pixelballoon_pixelboost_GoogleHelpers_onIsConsumable(JNIEnv * env, jobject obj, jstring javaSku)
{
    const char* sku = env->GetStringUTFChars(javaSku, 0);

    bool consumable = Store::Instance()->OnIsConsumable(sku);

    env->ReleaseStringUTFChars(javaSku, sku);

    return consumable;
}

JNIEXPORT jboolean JNICALL Java_com_pixelballoon_pixelboost_GoogleHelpers_onIsPurchasable(JNIEnv * env, jobject obj, jstring javaSku)
{
    const char* sku = env->GetStringUTFChars(javaSku, 0);

    bool purchasable = Store::Instance()->OnIsPurchasable(sku);

    env->ReleaseStringUTFChars(javaSku, sku);

    return purchasable;
}

void GH_init()
{
    JNIEnv* env = pb::Jni::GetJniEnv();

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/GoogleHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "init", "()V");
    env->CallStaticVoidMethod(classId, methodId);
}

bool GH_isBillingSupported()
{
    JNIEnv* env = pb::Jni::GetJniEnv();

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/GoogleHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "isBillingSupported", "()Z");
    jboolean result = env->CallStaticBooleanMethod(classId, methodId);

    return result;
}

void GH_refreshPurchases()
{
    JNIEnv* env = pb::Jni::GetJniEnv();

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/GoogleHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "refreshPurchases", "()V");
    env->CallStaticVoidMethod(classId, methodId);
}

void GH_addSku(const std::string& identifier)
{
    JNIEnv* env = pb::Jni::GetJniEnv();

    jstring jstr = env->NewStringUTF(identifier.c_str());
    jclass classId = env->FindClass("com/pixelballoon/pixelboost/GoogleHelpers");

    jmethodID methodId = env->GetStaticMethodID(classId, "addSku", "(Ljava/lang/String;)V");
    env->CallStaticVoidMethod(classId, methodId, jstr);

    env->DeleteLocalRef(jstr);
}

void GH_purchaseItem(const std::string& identifier)
{
    JNIEnv* env = pb::Jni::GetJniEnv();

    jstring jstr = env->NewStringUTF(identifier.c_str());
    jclass classId = env->FindClass("com/pixelballoon/pixelboost/GoogleHelpers");

    jmethodID methodId = env->GetStaticMethodID(classId, "purchase", "(Ljava/lang/String;)V");
    env->CallStaticVoidMethod(classId, methodId, jstr);

    env->DeleteLocalRef(jstr);
}

void GH_refreshSkus()
{
    JNIEnv* env = pb::Jni::GetJniEnv();

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/GoogleHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "refreshSkus", "()V");
    env->CallStaticVoidMethod(classId, methodId);
}

#endif

#if defined(PIXELBOOST_ANDROID_STORE_AMAZON)

extern "C" {
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_AmazonHelpers_onProductReceived(JNIEnv * env, jobject obj, jstring javaSku, jstring javaPrice);
    JNIEXPORT jboolean JNICALL Java_com_pixelballoon_pixelboost_AmazonHelpers_onProductPurchased(JNIEnv * env, jobject obj, jstring javaSku);
    JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_AmazonHelpers_onProductPurchaseFailed(JNIEnv * env, jobject obj, jstring javaSku);
};

JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_AmazonHelpers_onProductReceived(JNIEnv * env, jobject obj, jstring javaSku, jstring javaPrice)
{
    const char* sku = env->GetStringUTFChars(javaSku, 0);
    const char* price = env->GetStringUTFChars(javaPrice, 0);

    StoreItem item(sku, "", price);    
    Store::Instance()->AddItem(item);

    env->ReleaseStringUTFChars(javaSku, sku);
    env->ReleaseStringUTFChars(javaPrice, price);
}

JNIEXPORT jboolean JNICALL Java_com_pixelballoon_pixelboost_AmazonHelpers_onProductPurchased(JNIEnv * env, jobject obj, jstring javaSku)
{
    jboolean success = false;

    const char* sku = env->GetStringUTFChars(javaSku, 0);

    if (sku && Store::Instance() && Store::Instance()->OnItemPurchased(sku))
    {
        success = true;
    }

    env->ReleaseStringUTFChars(javaSku, sku);

    g_PaymentsPending = glm::max(g_PaymentsPending-1, 0);

    return success;
}

JNIEXPORT void JNICALL Java_com_pixelballoon_pixelboost_AmazonHelpers_onProductPurchaseFailed(JNIEnv * env, jobject obj, jstring javaSku)
{
    g_PaymentsPending = glm::max(g_PaymentsPending-1, 0);
}

void AH_init()
{
    JNIEnv* env = pb::Jni::GetJniEnv();

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/AmazonHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "init", "()V");
    env->CallStaticVoidMethod(classId, methodId);
}

bool AH_isBillingSupported()
{
    JNIEnv* env = pb::Jni::GetJniEnv();

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/AmazonHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "isBillingSupported", "()Z");
    jboolean result = env->CallStaticBooleanMethod(classId, methodId);

    return result;
}

void AH_refreshPurchases()
{
    JNIEnv* env = pb::Jni::GetJniEnv();

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/AmazonHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "refreshPurchases", "()V");
    env->CallStaticVoidMethod(classId, methodId);
}

void AH_addSku(const std::string& identifier)
{
    JNIEnv* env = pb::Jni::GetJniEnv();

    jstring jstr = env->NewStringUTF(identifier.c_str());
    jclass classId = env->FindClass("com/pixelballoon/pixelboost/AmazonHelpers");

    jmethodID methodId = env->GetStaticMethodID(classId, "addSku", "(Ljava/lang/String;)V");
    env->CallStaticVoidMethod(classId, methodId, jstr);

    env->DeleteLocalRef(jstr);
}

void AH_purchaseItem(const std::string& identifier)
{
    JNIEnv* env = pb::Jni::GetJniEnv();

    jstring jstr = env->NewStringUTF(identifier.c_str());
    jclass classId = env->FindClass("com/pixelballoon/pixelboost/AmazonHelpers");

    jmethodID methodId = env->GetStaticMethodID(classId, "purchase", "(Ljava/lang/String;)V");
    env->CallStaticVoidMethod(classId, methodId, jstr);

    env->DeleteLocalRef(jstr);
}

void AH_refreshSkus()
{
    JNIEnv* env = pb::Jni::GetJniEnv();

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/AmazonHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "refreshSkus", "()V");
    env->CallStaticVoidMethod(classId, methodId);
}

#endif

#endif

StoreItem::StoreItem(const std::string& identifier, const std::string& description, const std::string& priceString)
{
    _Identifier = identifier;
    _Description = description;
    _PriceString = priceString;
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

Store* Store::_Instance = 0;

Store::Store()
{
    PbAssert(_Instance == 0);
    _Instance = this;
    
#if defined(PIXELBOOST_PLATFORM_IOS) or (PIXELBOOST_PLATFORM_OSX)
    _Delegate = [StorePaymentDelegate new];
    
    [[SKPaymentQueue defaultQueue] addTransactionObserver:_Delegate];
#endif

#if defined(PIXELBOOST_PLATFORM_ANDROID)
    #if defined(PIXELBOOST_ANDROID_STORE_GOOGLEPLAY)
        GH_init();
    #endif
    #if defined(PIXELBOOST_ANDROID_STORE_AMAZON)
        AH_init();
    #endif
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

#if defined(PIXELBOOST_PLATFORM_ANDROID)
    #if defined(PIXELBOOST_ANDROID_STORE_GOOGLEPLAY)
        return GH_isBillingSupported();
    #endif
    #if defined(PIXELBOOST_ANDROID_STORE_AMAZON)
        return AH_isBillingSupported();
    #endif
#endif
    
    return false;
}

void Store::AddIdentifier(const std::string& identifier, std::function<void(void)> purchaseCallback, bool consumable)
{
    _Identifiers.push_back(identifier);
    _Callbacks[identifier] = purchaseCallback;
    _Consumable[identifier] = consumable;

#if defined(PIXELBOOST_PLATFORM_ANDROID)
    #if defined(PIXELBOOST_ANDROID_STORE_GOOGLEPLAY)
        GH_addSku(identifier);
    #endif
    #if defined(PIXELBOOST_ANDROID_STORE_AMAZON)
        AH_addSku(identifier);
    #endif
#endif
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

#if defined(PIXELBOOST_PLATFORM_ANDROID)
    #if defined(PIXELBOOST_ANDROID_STORE_GOOGLEPLAY)
        GH_refreshSkus();
    #endif
    #if defined(PIXELBOOST_ANDROID_STORE_AMAZON)
        AH_refreshSkus();
    #endif
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

#if defined(PIXELBOOST_PLATFORM_ANDROID)
    #if defined(PIXELBOOST_ANDROID_STORE_GOOGLEPLAY)
        GH_refreshPurchases();
    #endif
    #if defined(PIXELBOOST_ANDROID_STORE_AMAZON)
        AH_refreshPurchases();
    #endif
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

#if defined(PIXELBOOST_PLATFORM_ANDROID)
    #if defined(PIXELBOOST_ANDROID_STORE_GOOGLEPLAY)
        if (ArePaymentsAllowed())
        {
            g_PaymentsPending++;
            GH_purchaseItem(item.GetIdentifier());
        }
    #endif
    #if defined(PIXELBOOST_ANDROID_STORE_AMAZON)
        if (ArePaymentsAllowed())
        {
            g_PaymentsPending++;
            AH_purchaseItem(item.GetIdentifier());
        }
    #endif
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

bool Store::OnIsConsumable(const std::string& identifier)
{
    auto it = _Consumable.find(identifier);
    
    if (it == _Consumable.end())
        return false;
    
    return it->second;
}

bool Store::OnIsPurchasable(const std::string& identifier)
{
    auto it = _Callbacks.find(identifier);
    
    if (it == _Callbacks.end())
        return false;
    
    return true;
}
