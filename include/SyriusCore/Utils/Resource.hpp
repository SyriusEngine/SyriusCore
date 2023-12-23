#pragma once

#include <unordered_map>
#include "SyriusCoreInclude.hpp"

namespace Syrius{

    template<typename T>
    class Resource;

    // Non owning handle to the resource
    template<typename T>
    class ResourceView{
    public:
        ResourceView()
        : m_Resource(nullptr),
        m_ID(0){

        };

        ResourceView(Resource<T>* resource, uint64 id):
        m_Resource(resource),
        m_ID(id){
            m_Resource->updateViewReference(m_ID, this);
        }

        ResourceView(const ResourceView& other){
            m_Resource = other.m_Resource;
            m_ID = other.m_Resource->createViewCopy(this);
        }

        ResourceView(ResourceView&& other) noexcept {
            m_Resource = other.m_Resource;
            m_ID = other.m_Resource->createViewCopy(this);
        }

        ResourceView& operator=(const ResourceView& other) {
            if (this != &other){
                m_Resource = other.m_Resource;
                m_ID = other.m_Resource->createViewCopy(this);
            }
            return *this;
        }

        ResourceView& operator=(ResourceView&& other) noexcept {
            if (this != &other){
                m_Resource = other.m_Resource;
                m_ID = other.m_Resource->createViewCopy(this);
            }
            return *this;
        }

        ~ResourceView(){
            if (m_Resource){
                m_Resource->removeViewReference(m_ID);
            }
        }

        [[nodiscard]] T* get() const{
            return m_Resource->get();
        }

        [[nodiscard]] T* operator->() const{
            return m_Resource->get();
        }


        [[nodiscard]] Resource<T>* getResource() const{
            return m_Resource;
        }

        [[nodiscard]] bool isValid() const{
            return (m_Resource != nullptr);
        }

        void release(){
            if (m_Resource){
                m_Resource->removeViewReference(m_ID);
                m_Resource = nullptr;
                m_ID = 0;
            }
        }


    private:
        friend class Resource<T>;

        uint64 m_ID;
        Resource<T>* m_Resource;
    };

    // owning object of the resource, can have multiple views
    template<typename T>
    class Resource{
    public:
        struct PointerType{
            using type = T*;
        };

        using Pointer = typename PointerType::type;
    public:
        Resource():
        m_Resource(nullptr),
        m_NextViewID(1){

        }

        explicit Resource(Pointer resource):
        m_Resource(resource),
        m_NextViewID(1){

        }

        Resource(const Resource& other) = delete;

        Resource(Resource&& other) noexcept:
        m_Resource(other.m_Resource),
        m_NextViewID(other.m_NextViewID){
            other.m_Resource = nullptr;
            other.m_NextViewID = 0;
            m_Views = std::move(other.m_Views);
            for (auto& view : m_Views){
                view.second->m_Resource = this;
            }
        }

        Resource& operator=(const Resource& other) = delete;

        Resource& operator=(Resource&& other) noexcept{
            if(this != &other){
                m_Resource = other.m_Resource;
                m_NextViewID = other.m_NextViewID;
                other.m_Resource = nullptr;
                other.m_NextViewID = 0;
                m_Views = std::move(other.m_Views);
                for (auto& view : m_Views){
                    view.second->m_Resource = this;
                }
            }

            return *this;
        }

        ~Resource(){
            for (auto& [id, view] : m_Views){
                view->m_Resource = nullptr;
                view->m_ID = 0;
            }
            delete m_Resource;
        }

        [[nodiscard]] inline T* get() const{
            return m_Resource;
        }

        [[nodiscard]] inline T* operator->() const{
            return m_Resource;
        }

        [[nodiscard]] ResourceView<T> createView() {
            return ResourceView<T>(this, m_NextViewID++);
        }

        [[nodiscard]] Size getViewCount() const{
            return m_Views.size();
        }

        [[nodiscard]] bool isValid() const{
            return m_Resource != nullptr;
        }

        void destroy(){
            for (auto& view : m_Views){
                view.second->m_Resource = nullptr;
                view.second->m_ID = 0;
            }
            m_Views.clear();
            delete m_Resource;
            m_Resource = nullptr;
        }

        [[nodiscard]] uint64 getResourceID() const {
            return static_cast<uint64>(m_Resource);
        }

    protected:
        friend class ResourceView<T>;

        void updateViewReference(uint64 id, ResourceView<T>* resource){
            m_Views[id] = resource;
        }

        void removeViewReference(uint64 id){
            if (id != 0){
                m_Views.erase(id);
            }
        }

        uint64 createViewCopy(ResourceView<T>* resource){
            auto id = m_NextViewID++;
            m_Views[id] = resource;
            return id;
        }

    private:
        uint64 m_NextViewID;
        std::unordered_map<uint64, ResourceView<T>*> m_Views;
        Pointer m_Resource;

    };

    template<typename T, typename... Args>
    Resource<T> createResource(Args&&... args){
        return Resource<T>(new T(std::forward<Args>(args)...));
    }

}