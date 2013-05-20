#pragma once

#include <functional>
#include <vector>

#if defined(PIXELBOOST_PLATFORM_IOS) or (PIXELBOOST_PLATFORM_OSX)
@class StorePaymentDelegate;
#endif

namespace pb
{
    
    class StoreItem
    {
    public:
        bool IsConsumable();
    };
    
    class StorePurchasedItem
    {
    public:
        
    };
    
    class StoreTransaction
    {
    public:
        StoreTransaction(const StoreItem* item);
        
        const StoreItem& GetStoreItem();
        
        void Finalise(std::function<void(bool)> callback);
        
    private:
        const StoreItem* _StoreItem;
    };
    
    class Store
    {
    public:
        Store();
        ~Store();
        
        static Store* Instance();
        
        void RefreshItems();
        std::vector<StoreItem> GetStoreItems();
        
        void PurchaseItem(const StoreItem& item);
        
        std::vector<StoreTransaction> GetPendingTransactions();
    
    private:
        static Store* _Instance;
        
        std::vector<StoreItem> _Items;
        std::vector<StoreTransaction> _Pending;
        
#if defined(PIXELBOOST_PLATFORM_IOS) or (PIXELBOOST_PLATFORM_OSX)
        StorePaymentDelegate* _Delegate;
#endif
    };
    
}
