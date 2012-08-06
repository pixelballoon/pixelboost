#pragma once

#include <string>

class Model;

class ModelLoader
{
public:
    virtual ~ModelLoader();
    
    virtual bool Process() = 0;
    virtual const Model* GetModel() const = 0;
};

class ObjLoader : public ModelLoader
{
public:
    ObjLoader(const std::string& filename);
    virtual ~ObjLoader();
    
    virtual bool Process();
    virtual const Model* GetModel() const;
    
private:
    std::string _Filename;
    Model* _Model;
};
