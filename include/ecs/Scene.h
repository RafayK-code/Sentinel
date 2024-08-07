#pragma once
#ifndef _SCENE_H
#define _SCENE_H

#include <vector>
#include <memory>
#include <shared_mutex>

#include "core/Core.h"
#include "core/Dbg.h"
#include "Types.h"
#include "ComponentPool.h"

namespace sntl
{
    template<typename... ComponentTypes>
    class View;

    class SNTL_API Scene      //TODO: Add copy constructor to allow users to copy a scene (??? Should this even be allowed?)
    {
        template<typename... ComponentTypes>
        friend class View;
    public:
        Scene(size_t maxEntities);
        ~Scene();

        EntityID createEntity();
        void destroyEntity(EntityID entity);

        template<typename T>
        bool hasComponent(EntityID entity)
        {
            if (entities_[getEntityIndex(entity)].id != entity)
            {
                DBG_ERROR("Entity {0} is no longer valid", entity);
                return false;
            }

            ComponentID cID = getComponentID<T>();
            return entities_[getEntityIndex(entity)].signature.test(cID);
        }

        template<typename T>
        T* attachComponent(EntityID entity)
        {
            if (entities_[getEntityIndex(entity)].id != entity)
            {
                DBG_ERROR("Entity {0} is no longer valid", entity);
                return nullptr;
            }

            if (hasComponent<T>(entity))
            {
                DBG_WARN("Entity {0} already has component {1}. Returning component instead", entity, typeid(T).name());
                return getComponent<T>(entity);
            }

            ComponentID cID = getComponentID<T>();
            
            if (componentPools_.size() <= cID)
            {
                std::unique_lock<std::shared_mutex> lock(mtx_);

                componentPools_.resize(cID + 1);
                componentDestructors_.resize(cID + 1);
                componentPools_[cID] = new ComponentPool(sizeof(T), maxEntities_);
                componentDestructors_[cID] = [](void* component) { static_cast<T*>(component)->~T(); };
            }

            std::unique_lock<std::shared_mutex> lock(mtx_);
            T* component = (new (componentPools_[cID]->getChunk(getEntityIndex(entity))) T());
            entities_[getEntityIndex(entity)].signature.set(cID);

            return component;
        }

        template<typename T>
        T* getComponent(EntityID entity)
        {
            if (entities_[getEntityIndex(entity)].id != entity)
            {
                DBG_ERROR("Entity {0} is no longer valid", entity);
                return nullptr;
            }

            ComponentID cID = getComponentID<T>();

            if (!hasComponent<T>(entity))
            {
                DBG_ERROR("Entity {0} does not have component {1}", entity, typeid(T).name());
                return nullptr;
            }

            T* component = (static_cast<T*>(componentPools_[cID]->getChunk(getEntityIndex(entity))));
            return component;
        }

        template<typename T>
        void removeComponent(EntityID entity)
        {
            if (entities_[getEntityIndex(entity)].id != entity)
            {
                DBG_ERROR("Entity {0} is no longer valid", entity);
                return;
            }

            if (!hasComponent<T>(entity))
            {
                DBG_ERROR("Entity {0} does not have component {1}", entity, typeid(T).name());
                return;
            }

            std::unique_lock<std::shared_mutex> lock(mtx_);

            ComponentID cID = getComponentID<T>();
            (static_cast<T*>(componentPools_[cID]->getChunk(getEntityIndex(entity))))->~T();
            componentPools_[cID]->freeChunk(getEntityIndex(entity));

            entities_[getEntityIndex(entity)].signature.reset(cID);
        }

    private:
        using ComponentDestructor = std::function<void(void*)>;

        struct EntityDesc
        {
            EntityID id;
            ComponentSignature signature;
        };

        EntityID createEntityId(internal::EntityIndex index, internal::EntityVersion version);
        internal::EntityIndex getEntityIndex(EntityID entity);
        internal::EntityVersion getEntityVersion(EntityID entity);
        bool isEntityValid(EntityID entity);

    private:
        size_t maxEntities_;

        std::vector<EntityDesc> entities_;
        std::vector<internal::EntityIndex> freeEntities_;
        std::vector<ComponentPool*> componentPools_;
        std::vector<ComponentDestructor> componentDestructors_;

        std::shared_mutex mtx_;
    };
}

#endif