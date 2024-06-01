#pragma once
#ifndef _SCENE_H
#define _SCENE_H

#include <vector>
#include <memory>

#include "Core.h"
#include "Types.h"
#include "ComponentPool.h"
#include "Dbg.h"

namespace sntl
{
    class ENGINE_API Scene      //TODO: Add copy constructor to allow users to copy a scene (??? Should this even be allowed?)
    {
    public:
        Scene(size_t maxEntities);
        ~Scene();

        EntityID createEntity();
        void destroyEntity(EntityID entity);

        template<typename T>
        T& attachComponent(EntityID entity)
        {
            static T defaultComponent = T();
            ComponentID cID = getComponentID<T>();

            if (entities_[getEntityIndex(entity)].id != entity)
            {
                DBG_WARN("Entity {0} is no longer valid", entity);
                return defaultComponent;
            }

            if (componentPools_.size() <= cID)
            {
                componentPools_.resize(cID + 1);
                componentPools_[cID] = new ComponentPool(sizeof(T), maxEntities_);
            }

            T& component = *(new (componentPools_[cID]->get(entity)) T());

            entities_[entity].signature.set(cID);
            return component;
        }

        template<typename T>
        T& getComponent(EntityID entity)
        {
            static T defaultComponent = T();
            ComponentID cID = getComponentID<T>();

            if (entities_[getEntityIndex(entity)].id != entity)
            {
                DBG_WARN("Entity {0} is no longer valid", entity);
                return defaultComponent;
            }

            if (!entities_[entity].signature.test(cID))
            {
                DBG_ERROR("Entity {0} does not have component {1}. Returning dummy component", entity, typeid(T).name());
                return defaultComponent;
            }

            T& component = *(static_cast<T*>(componentPools_[cID]->get(entity)));
            return component;
        }

        template<typename T>
        void removeComponent(EntityID entity)
        {
            if (entities_[getEntityIndex(entity)].id != entity)
            {
                DBG_WARN("Entity {0} is no longer valid", entity);
                return;
            }

            ComponentID cID = getComponentID<T>();
            entities_[entity].signature.reset(cID);
        }

        struct EntityDesc
        {
            EntityID id;
            ComponentSignature signature;
        };

    private:
        using EntityIndex = uint32_t;
        using EntityVersion = uint32_t;

        EntityID createEntityId(EntityIndex index, EntityVersion version);
        EntityIndex getEntityIndex(EntityID entity);
        EntityVersion getEntityVersion(EntityID entity);
        bool isEntityValid(EntityID entity);

        size_t maxEntities_;

        std::vector<EntityDesc> entities_;
        std::vector<EntityIndex> freeEntities_;
        std::vector<ComponentPool*> componentPools_;
    };
}

#endif