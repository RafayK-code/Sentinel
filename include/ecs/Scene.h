#pragma once
#ifndef _SCENE_H
#define _SCENE_H

#include <vector>
#include <memory>

#include "core/Core.h"
#include "core/Dbg.h"
#include "Types.h"
#include "ComponentPool.h"
#include "sys/ThreadUtil.h"

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
            SNTL_SHARED_LOCK_SCOPE;

            if (entities_[getEntityIndex(entity)].id != entity)
            {
                DBG_ERROR("Entity {0} is no longer valid", entity);
                return false;
            }

            ComponentID cID = getComponentID<T>();
            return entities_[getEntityIndex(entity)].signature.test(cID);
        }

        template<typename T>
        T& attachComponent(EntityID entity)
        {
            SNTL_UNIQUE_LOCK_SCOPE;

            static T defaultComponent = T();

            if (entities_[getEntityIndex(entity)].id != entity)
            {
                DBG_ERROR("Entity {0} is no longer valid. Returning dummy component", entity);
                return defaultComponent;
            }

            SNTL_UNIQUE_LOCK_UNLOCK;

            if (hasComponent<T>(entity))
            {
                DBG_WARN("Entity {0} already has component {1}. Returning component instead", entity, typeid(T).name());
                return getComponent<T>(entity);
            }

            SNTL_UNIQUE_LOCK_LOCK;

            ComponentID cID = getComponentID<T>();
            
            if (componentPools_.size() <= cID)
            {
                componentPools_.resize(cID + 1);
                componentDestructors_.resize(cID + 1);
                componentPools_[cID] = new ComponentPool(sizeof(T), maxEntities_);
                componentDestructors_[cID] = [](void* component) { static_cast<T*>(component)->~T(); };
            }

            T& component = *(new (componentPools_[cID]->getChunk(getEntityIndex(entity))) T());
            entities_[getEntityIndex(entity)].signature.set(cID);

            return component;
        }

        template<typename T>
        T& getComponent(EntityID entity)
        {
            SNTL_SHARED_LOCK_SCOPE;

            static T defaultComponent = T();

            if (entities_[getEntityIndex(entity)].id != entity)
            {
                DBG_ERROR("Entity {0} is no longer valid. Returning dummy component", entity);
                return defaultComponent;
            }

            ComponentID cID = getComponentID<T>();

            SNTL_SHARED_LOCK_UNLOCK;

            if (!hasComponent<T>(entity))
            {
                DBG_ERROR("Entity {0} does not have component {1}. Returning dummy component", entity, typeid(T).name());
                return defaultComponent;
            }

            SNTL_SHARED_LOCK_LOCK;

            T& component = *(static_cast<T*>(componentPools_[cID]->getChunk(getEntityIndex(entity))));
            return component;
        }

        template<typename T>
        void removeComponent(EntityID entity)
        {
            SNTL_UNIQUE_LOCK_SCOPE;

            if (entities_[getEntityIndex(entity)].id != entity)
            {
                DBG_ERROR("Entity {0} is no longer valid", entity);
                return;
            }

            SNTL_UNIQUE_LOCK_UNLOCK;

            if (!hasComponent<T>(entity))
            {
                DBG_ERROR("Entity {0} does not have component {1}", entity, typeid(T).name());
                return;
            }

            SNTL_UNIQUE_LOCK_LOCK;

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

        size_t maxEntities_;

        std::vector<EntityDesc> entities_;
        std::vector<internal::EntityIndex> freeEntities_;
        std::vector<ComponentPool*> componentPools_;
        std::vector<ComponentDestructor> componentDestructors_;

        SNTL_SHARED_MUTEX;
    };
}

#endif