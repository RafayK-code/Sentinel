#include "ECS/Scene.h"
#include "Dbg.h"

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
            EntityIndex newIndex = freeEntities_.back();
            freeEntities_.pop_back();
            EntityID newID = createEntityId(newIndex, getEntityVersion(entities_[newIndex].id));
            entities_[newIndex].id = newID;
            return newID;
        }

        EntityID eID = createEntityId(EntityIndex(entities_.size()), 0);
        entities_.push_back({ eID, ComponentSignature() });
        return eID;
    }

    void Scene::destroyEntity(EntityID entity)
    {
        EntityID newID = createEntityId(EntityIndex(-1), getEntityVersion(entity) + 1);
        entities_[getEntityIndex(entity)].id = newID;

        for (ComponentID cID = 0; cID < MAX_COMPONENTS; cID++)
        {
            if (entities_[getEntityIndex(entity)].signature.test(cID))
            {
                void* component = componentPools_[cID]->getChunk(getEntityIndex(entity));
                componentDestructors_[cID](component);
                //componentPools_[cID]->freeChunk(getEntityIndex(entity));
            }
        }

        entities_[getEntityIndex(entity)].signature.reset();

        freeEntities_.push_back(getEntityIndex(entity));
    }

    EntityID Scene::createEntityId(Scene::EntityIndex index, Scene::EntityVersion version)
    {
        return ((EntityID)index << 32) | (EntityID)version;
    }

    Scene::EntityIndex Scene::getEntityIndex(EntityID entity)
    {
        return entity >> 32;
    }

    Scene::EntityVersion Scene::getEntityVersion(EntityID entity)
    {
        return (EntityVersion)entity;
    }

    bool Scene::isEntityValid(EntityID entity)
    {
        return (entity >> 32) != EntityIndex(-1);
    }
}