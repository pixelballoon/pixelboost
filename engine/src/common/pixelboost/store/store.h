#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

#if defined(PIXELBOOST_PLATFORM_IOS) or defined(PIXELBOOST_PLATFORM_OSX)
@class SKProduct;
@class StorePaymentDelegate;
#endif

namespace pb
{
    
    class StoreItem
    {
    public:
        StoreItem(const std::string& identifier, const std::string& description, const std::string& priceString);
        ~StoreItem();
        
        const std::string& GetIdentifier() const;
        const std::string& GetDescription() const;
        const std::string& GetPriceString() const;
        
    private:
        std::string _Identifier;
        std::string _Description;
        std::string _PriceString;
        
    public:
#if defined(PIXELBOOST_PLATFORM_IOS) or defined(PIXELBOOST_PLATFORM_OSX)
        SKProduct* _Product;
#endif
    };
    
    class Store
    {
    public:
        Store();
        ~Store();
        
        static Store* Instance();
        
        bool ArePaymentsAllowed();
        
        void AddIdentifier(const std::string& identifier, std::function<void(void)> purchaseCallback, bool consumable);
        
        void RefreshItems();
        std::vector<StoreItem> GetStoreItems();
        StoreItem* GetStoreItem(const std::string& identifier);
        
        void RestorePurchases();
        void PurchaseItem(const StoreItem& item);
        
        void AddItem(const StoreItem& item);
        
        bool OnItemPurchased(const std::string& identifier);
        bool OnIsConsumable(const std::string& identifier);
        bool OnIsPurchasable(const std::string& identifier);
    
    private:
        static Store* _Instance;
        
        std::map<std::string, StoreItem*> _Items;
        
#if defined(PIXELBOOST_PLATFORM_IOS) or defined(PIXELBOOST_PLATFORM_OSX)
        StorePaymentDelegate* _Delegate;
#endif
        
        std::vector<std::string> _Identifiers;
        std::map<std::string, std::function<void(void)>> _Callbacks;
        std::map<std::string, bool> _Consumable;
    };
    
}
