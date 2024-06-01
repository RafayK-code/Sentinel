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
    template<typename... ComponentTypes>
    class SceneView;

    class ENGINE_API Scene      //TODO: Add copy constructor to allow users to copy a scene (??? Should this even be allowed?)
    {
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
        T& attachComponent(EntityID entity)
        {
            static T defaultComponent = T();

            if (entities_[getEntityIndex(entity)].id != entity)
            {
                DBG_ERROR("Entity {0} is no longer valid. Returning dummy component", entity);
                return defaultComponent;
            }

            if (hasComponent<T>(entity))
            {
                DBG_WARN("Entity {0} already has component {1}. Returning component instead", entity, typeid(T).name());
                return getComponent<T>(entity);
            }

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
            static T defaultComponent = T();

            if (entities_[getEntityIndex(entity)].id != entity)
            {
                DBG_ERROR("Entity {0} is no longer valid. Returning dummy component", entity);
                return defaultComponent;
            }

            ComponentID cID = getComponentID<T>();

            if (!hasComponent<T>(entity))
            {
                DBG_ERROR("Entity {0} does not have component {1}. Returning dummy component", entity, typeid(T).name());
                return defaultComponent;
            }

            T& component = *(static_cast<T*>(componentPools_[cID]->getChunk(getEntityIndex(entity))));
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

            ComponentID cID = getComponentID<T>();
            (static_cast<T*>(componentPools_[cID]->getChunk(getEntityIndex(entity))))->~T();
            componentPools_[cID]->freeChunk(getEntityIndex(entity));

            entities_[getEntityIndex(entity)].signature.reset(cID);
        }

    private:
        using EntityIndex = uint32_t;
        using EntityVersion = uint32_t;
        using ComponentDestructor = std::function<void(void*)>;

        struct EntityDesc
        {
            EntityID id;
            ComponentSignature signature;
        };

        EntityID createEntityId(EntityIndex index, EntityVersion version);
        EntityIndex getEntityIndex(EntityID entity);
        EntityVersion getEntityVersion(EntityID entity);
        bool isEntityValid(EntityID entity);

        size_t maxEntities_;

        std::vector<EntityDesc> entities_;
        std::vector<EntityIndex> freeEntities_;
        std::vector<ComponentPool*> componentPools_;
        std::vector<ComponentDestructor> componentDestructors_;

        template<typename... ComponentTypes>
        friend class SceneView;
    };
}

#endif