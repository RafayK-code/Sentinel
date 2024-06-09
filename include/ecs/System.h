#pragma once
#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <functional>

#include "Scene.h"
#include "sys/ThreadPool.h"

namespace sntl
{
    template<typename... ComponentTypes>
    class System
    {
    public:
        System(Scene& scene, std::function<void(Scene&, EntityID)> initFunction, std::function<void(Scene&, EntityID)> updateFunction, ThreadPool* pool = nullptr) : 
            scene_(scene), 
            minComponentPool_(nullptr), 
            all_(false), 
            minComponentPoolSize_(scene.maxEntities_ + 1), 
            initFunction_(initFunction), 
            updateFunction_(updateFunction),
            pool_(pool)
        {
            if (sizeof...(ComponentTypes) == 0)
                all_ = true;
            else
            {
                int componentIds[] = { 0, getComponentID<ComponentTypes>() ... };
                for (int i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
                {
                    signature_.set(componentIds[i]);
                    if (scene_.componentPools_[componentIds[i]]->numChunks() < minComponentPoolSize_)
                    {
                        minComponentPoolSize_ = scene_.componentPools_[componentIds[i]]->numChunks();
                        minComponentPool_ = scene_.componentPools_[componentIds[i]];
                    }
                }
            }
        }

        ~System()
        {
        }

        void assignThreadpool(ThreadPool* pool) { pool_ = pool; }

        void init(int numThreads = 0)
        {
            if (!pool || numThreads <= 1) // just do on current core
            {
                execute(initFunction_);
                return;
            }

            executeInParallel(numThreads, initFunction_);
        }

        void update(int numThreads = 0)
        {
            if (!pool_ || numThreads <= 1) // just do on current core
            {
                execute(updateFunction_);
                return;
            }

            executeInParallel(numThreads, updateFunction_);
        }

    private:
        void execute(const std::function<void(Scene&, EntityID)>& f)
        {
            const std::vector<size_t>& denseIds = minComponentPool_->getDenseChunks();
            for (size_t i = 0; i < denseIds.size(); i++)
            {
                size_t index = denseIds[i];
                if (validIndex(index))
                    f(scene_, scene_.entities_[index].id);
            }
        }

        void executeInParallel(int numThreads, const std::function<void(Scene&, EntityID)>& f)
        {
            const std::vector<size_t>& denseIds = minComponentPool_->getDenseChunks();
            size_t numElements = denseIds.size();
            size_t chunkSize = (numElements + numThreads - 1) / numThreads;

            std::vector<std::future<void>> futures;
            for (int i = 0; i < numThreads; i++)
            {
                size_t start = i * chunkSize;
                size_t end = (i == numThreads - 1) ? numElements : (start + chunkSize);

                if (start >= end)
                    break;

                futures.emplace_back(pool_->enqueue([this, &f, &denseIds, start, end, entities = scene_.entities_] {
                    for (size_t j = start; j < end; j++)
                    {
                        size_t index = denseIds[j];
                        if (validIndex(index))
                            f(scene_, entities[index].id);
                    }
                }));
            }

            for (auto& future : futures)
                future.get();
        }

        bool validIndex(Scene::EntityIndex index)
        {
            return (signature_ == (signature_ & scene_.entities_[index].signature));
        }

        Scene& scene_;
        ThreadPool* pool_;
        ComponentSignature signature_;
        bool all_;

        ComponentPool* minComponentPool_;
        size_t minComponentPoolSize_;

        std::function<void(Scene&, EntityID)> initFunction_;
        std::function<void(Scene&, EntityID)> updateFunction_;
    };
}

#endif