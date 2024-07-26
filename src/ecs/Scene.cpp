#include "ecs/Scene.h"

namespace sntl
{
    Scene::Scene(size_t maxEntities)
        : maxEntities_(maxEntities)
    {
        if (maxEntities_ > ENGINE_MAX_ENTITIES)
        {
            DBG_WARN("Sentinel supports up to {0} entities", ENGINE_MAX_ENTITIES);
            maxEntities_ = ENGINE_MAX_ENTITIES;
        }
    }

    Scene::~Scene()
    {
        for (EntityDesc e : entities_)
        {
            for (ComponentID cID = 0; cID < MAX_COMPONENTS; cID++)
            {
                if (e.signature.test(cID))
                {
                    EntityID entity = e.id;
                    void* component = componentPools_[cID]->getChunk(getEntityIndex(entity));
                    componentDestructors_[cID](component);
                }
            }
        }

        for (ComponentPool* p : componentPools_)
            delete p;
    }

    EntityID Scene::createEntity()
    {
        if (!freeEntities_.empty())
        {
            internal::EntityIndex newIndex = freeEntities_.back();
            freeEntities_.pop_back();
            EntityID newID = createEntityId(newIndex, getEntityVersion(entities_[newIndex].id));
            entities_[newIndex].id = newID;
            return newID;
        }

        EntityID eID = createEntityId(internal::EntityIndex(entities_.size()), 0);
        entities_.push_back({ eID, ComponentSignature() });
        return eID;
    }

    void Scene::destroyEntity(EntityID entity)
    {
        EntityID newID = createEntityId(internal::EntityIndex(-1), getEntityVersion(entity) + 1);
        entities_[getEntityIndex(entity)].id = newID;

        for (ComponentID cID = 0; cID < MAX_COMPONENTS; cID++)
        {
            if (entities_[getEntityIndex(entity)].signature.test(cID))
            {
                void* component = componentPools_[cID]->getChunk(getEntityIndex(entity));
                componentDestructors_[cID](component);
                // free chunk is breaking. Look into this
                componentPools_[cID]->freeChunk(getEntityIndex(entity));
            }
        }

        entities_[getEntityIndex(entity)].signature.reset();

        freeEntities_.push_back(getEntityIndex(entity));
    }

    EntityID Scene::createEntityId(internal::EntityIndex index, internal::EntityVersion version)
    {
        return ((EntityID)index << 32) | (EntityID)version;
    }

    internal::EntityIndex Scene::getEntityIndex(EntityID entity)
    {
        return entity >> 32;
    }

    internal::EntityVersion Scene::getEntityVersion(EntityID entity)
    {
        return (internal::EntityVersion)entity;
    }

    bool Scene::isEntityValid(EntityID entity)
    {
        return (entity >> 32) != internal::EntityIndex(-1);
    }
}