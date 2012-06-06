namespace pb
{

template <class T>T* Entity::GetComponentByType()
{
    Entity::ComponentList* components = GetComponentsByType(T::GetStaticType());
    
    if (components && components->size())
    {
        return static_cast<T*>(components->at(0));
    }
    
    return 0;
}
    
}
