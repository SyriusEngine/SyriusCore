#pragma once

#include <unordered_map>
#include "../Core/SyriusCoreInclude.hpp"

namespace Syrius{

    template<typename T>
    class Resource;

    // Non owning handle to the resource
    template<typename T>
    class ResourceView{
    public:
        ResourceView() = delete;

        ResourceView(Resource<T>* resource, uint64 id):
        m_Resource(resource),
        m_ID(id){
            m_Resource->updateViewReference(m_ID, this);
            printf("ResourceView::ResourceView() - %i created\n", m_ID);
        }

        ResourceView(const ResourceView& other){
            m_ID = other.m_Resource->createViewCopy(this);
            printf("ResourceView::ResourceView() - %i created\n", m_ID);
        }

        ResourceView(ResourceView&& other) noexcept = default;

        ResourceView& operator=(const ResourceView& other) = default;

        ResourceView& operator=(ResourceView&& other) noexcept = default;

        ~ResourceView(){
            m_Resource->removeViewReference(m_ID);
            printf("ResourceView::~ResourceView() - %i destroyed\n", m_ID);
        }

        [[nodiscard]] T* get() const{
            return m_Resource->get();
        }

        [[nodiscard]] T* operator->() const{
            return m_Resource->get();
        }

        [[nodiscard]] T& operator*() const{
            return *m_Resource->get();
        }

        [[nodiscard]] Resource<T>* getResource() const{
            return m_Resource;
        }


    private:
        uint64 m_ID;
        Resource<T>* m_Resource;
    };

    // owning object of the resource, can have multiple views
    template<typename T>
    class Resource{
    public:
        Resource():
        m_Resource(nullptr),
        m_NextViewID(0){

        }

        explicit Resource(T* resource):
        m_Resource(resource),
        m_NextViewID(0){

        }

        Resource(const Resource& other) = delete;

        Resource(Resource&& other) noexcept:
        m_Resource(other.m_Resource){
            other.m_Resource = nullptr;
        }

        Resource& operator=(const Resource& other) = delete;

        Resource& operator=(Resource&& other) noexcept{
            if(this != &other){
                m_Resource = other.m_Resource;
                other.m_Resource = nullptr;
            }
            return *this;
        }

        ~Resource(){
            delete m_Resource;
        }

        [[nodiscard]] inline T* get() const{
            return m_Resource;
        }

        [[nodiscard]] inline T* operator->() const{
            return m_Resource;
        }

        [[nodiscard]] inline T& operator*() const{
            return *m_Resource;
        }

        [[nodiscard]] bool isValid() const{
            return m_Resource != nullptr;
        }

        [[nodiscard]] ResourceView<T> createView(){
            return ResourceView<T>(this, m_NextViewID++);
        }

    protected:
        friend class ResourceView<T>;

        void updateViewReference(uint64 id, ResourceView<T>* resource){
            m_Views[id] = resource;
        }

        void removeViewReference(uint64 id){
            m_Views.erase(id);
        }

        uint64 createViewCopy(ResourceView<T>* resource){
            auto id = m_NextViewID++;
            m_Views[id] = resource;
            return id;
        }

    private:
        uint64 m_NextViewID;
        std::unordered_map<uint64, ResourceView<T>*> m_Views;
        T* m_Resource;

    };

}