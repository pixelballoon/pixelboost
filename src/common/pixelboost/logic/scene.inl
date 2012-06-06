namespace pb
{

template <class T>T* Scene::GetSystemByType()
{
    SystemMap::iterator it = _Systems.find(T::GetStaticType());
    if (it != _Systems.end())
    {
        return static_cast<T*>(it->second);
    }
    return 0;    
}

}
