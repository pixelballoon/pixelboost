#pragma once

#include <string>

#include "sigslot/signal.h"

#include "pixelboost/framework/definitions.h"

namespace pb
{

    struct DbStructData
    {
        virtual ~DbStructData();
        
        template <typename T> const T* As() const;
        template <typename T> T* As();
        template <typename T> bool IsA() const;
        
        virtual bool IsA(Uid type) const = 0;
        virtual Uid GetType() const = 0;
    };
    
    class DbStruct
    {
    public:
        DbStruct(Uid uid, Uid type, DbStructData* data);
        virtual ~DbStruct();
        
        Uid GetType() const;
        Uid GetUid() const;
        
        DbStructData* GetData() const;
        template<typename T> const T* GetData() const;
        
    public:
        sigslot::Signal0<> structDestroyed;
        sigslot::Signal0<> structReloaded;
        
    public:
        Uid _Uid;
        Uid _Type;
        
        DbStructData* _Data;
    };
    
}

#include "pixelboost/db/struct.inl"
