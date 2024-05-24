#pragma once

#include <memory>

namespace Syrius{

    template<typename T>
    using Resource = std::unique_ptr<T>;

    template<typename T>
    class ResourceView{
    public:
        ResourceView(): m_ResourcePtr(nullptr){}

        explicit ResourceView(const Resource<T>& resource): m_ResourcePtr(resource.get()){}

        ResourceView(const ResourceView<T>& other): m_ResourcePtr(other.m_ResourcePtr){}

        ResourceView(ResourceView<T>&& other) noexcept: m_ResourcePtr(other.m_ResourcePtr){}

        ResourceView<T>& operator=(const ResourceView<T>& other){
            if (this == &other){
                return *this;
            }
            m_ResourcePtr = other.m_ResourcePtr;
            return *this;
        }

        ResourceView<T>& operator=(ResourceView<T>&& other) noexcept{
            if (this == &other){
                return *this;
            }
            m_ResourcePtr = other.m_ResourcePtr;
            return *this;
        }

        ~ResourceView(){
            /*
             * We do not want to delete the resource here because it is managed by the Resource class
             */
            m_ResourcePtr = nullptr;
        }

        T* operator->() const{
            return m_ResourcePtr;
        }

        T& operator*() const{
            return *m_ResourcePtr;
        }

        T* get() const{
            return m_ResourcePtr;
        }

        explicit operator bool() const{
            return m_ResourcePtr != nullptr;
        }

        bool operator==(std::nullptr_t) const{
            return m_ResourcePtr == nullptr;
        }

        bool operator!=(std::nullptr_t) const{
            return m_ResourcePtr != nullptr;
        }

        bool operator<(std::nullptr_t) const{
            return m_ResourcePtr < nullptr;
        }

        bool operator>(std::nullptr_t) const{
            return m_ResourcePtr > nullptr;
        }

        bool operator<=(std::nullptr_t) const{
            return m_ResourcePtr <= nullptr;
        }

        bool operator>=(std::nullptr_t) const{
            return m_ResourcePtr >= nullptr;
        }

        bool operator==(const Resource<T>& other) const{
            return m_ResourcePtr == other.get();
        }

        bool operator!=(const Resource<T>& other) const{
            return m_ResourcePtr != other.get();
        }

        bool operator<(const Resource<T>& other) const{
            return m_ResourcePtr < other.get();
        }

        bool operator>(const Resource<T>& other) const{
            return m_ResourcePtr > other.get();
        }

        bool operator<=(const Resource<T>& other) const{
            return m_ResourcePtr <= other.get();
        }

        bool operator>=(const Resource<T>& other) const{
            return m_ResourcePtr >= other.get();
        }

        bool operator==(const ResourceView<T>& other) const{
            return m_ResourcePtr == other.get();
        }

        bool operator!=(const ResourceView<T>& other) const{
            return m_ResourcePtr != other.m_ResourcePtr;
        }

        bool operator<(const ResourceView<T>& other) const{
            return m_ResourcePtr < other.m_ResourcePtr;
        }

        bool operator>(const ResourceView<T>& other) const{
            return m_ResourcePtr > other.m_ResourcePtr;
        }

        bool operator<=(const ResourceView<T>& other) const{
            return m_ResourcePtr <= other.m_ResourcePtr;
        }

        bool operator>=(const ResourceView<T>& other) const{
            return m_ResourcePtr >= other.m_ResourcePtr;
        }

        bool operator==(const T* other) const{
            return m_ResourcePtr == other;
        }

        bool operator!=(const T* other) const{
            return m_ResourcePtr != other;
        }

        bool operator<(const T* other) const{
            return m_ResourcePtr < other;
        }

        bool operator>(const T* other) const{
            return m_ResourcePtr > other;
        }

        bool operator<=(const T* other) const{
            return m_ResourcePtr <= other;
        }

        bool operator>=(const T* other) const{
            return m_ResourcePtr >= other;
        }

    private:
        T* m_ResourcePtr;

    };

    template<typename T, typename... Args>
    inline Resource<T> createResource(Args&&... args){
        return std::make_unique<T>(args...);
    }

    template<typename T>
    inline ResourceView<T> createResourceView(Resource<T>& resource){
        return ResourceView<T>(resource);
    }

    template<typename T>
    inline ResourceView<T> createResourceView(const Resource<T>& resource){
        return ResourceView<T>(resource);
    }
}