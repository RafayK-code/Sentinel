#pragma once
#ifndef _VIEW_H
#define _VIEW_H

#include <functional>
#include <utility>
#include <type_traits>

#include "Scene.h"
#include "core/thread/ThreadPool.h"

namespace sntl
{
    template<typename... ComponentTypes>
    class View
    {
    public:
        View(Scene& scene) : 
            scene_(scene), minComponentPool_(nullptr), all_(false), minComponentPoolSize_(scene.maxEntities_ + 1)
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

        ~View()
        {
        }

        class ViewIterator
        {
        public:
            ViewIterator(Scene& scene, ComponentPool* pool, ComponentSignature signature, bool all, std::vector<internal::EntityIndex>::const_iterator curr)
                : scene_(scene), pool_(pool), signature_(signature), current_(curr), end_(pool->getDenseChunks().begin() + pool->numChunks())
            {
            }

            EntityID operator*() const
            {
                return scene_.entities_[*current].id;
            }

            ViewIterator& operator++()
            {
                advance();
                return *this;
            }

            ViewIterator operator++(int)
            {
                ViewIterator tmp = *this;
                advance();
                return tmp;
            }

            bool operator==(const ViewIterator& other) const
            {
                return pool_ == other.pool_ && current_ == other.current_;
            }

            bool operator!=(const ViewIterator& other) const
            {
                return pool_ != other.pool_ || current_ != other.current_;
            }

        private:
            bool validIndex(internal::EntityIndex index) const
            {
                return all_ || (signature_ == (signature_ & scene_.entities_[index].signature));
            }

            void advance()
            {
                ++current_;
                while (current_ != end_ && !validIndex(*current_))
                    ++current_;
            }

            Scene& scene_;
            ComponentPool* pool_;
            std::vector<internal::EntityIndex>::const_iterator current_;
            std::vector<internal::EntityIndex>::const_iterator end_;
            ComponentSignature signature_;
            bool all_;
        };

        using Iterator = ViewIterator;

        Iterator begin()
        {
            return Iterator(scene_, minComponentPool_, signature_, all_, minComponentPool_->getDenseChunks().begin());
        }


        Iterator end()
        {
            return Iterator(scene_, minComponentPool_, signature_, all_, minComponentPool_->getDenseChunks().end());
        }
        
        void forEach(const std::function<void(EntityID)>& f) const
        {
            const std::vector<internal::EntityIndex>& denseIds = minComponentPool_->getDenseChunks();
            for (size_t i = 0; i < denseIds.size(); i++)
            {
                internal::EntityIndex index = denseIds[i];
                if (validIndex(index))
                    f(scene_.entities_[index].id);
            }
        }

        void forEachThreaded(const std::function<void(EntityID)>& f, ThreadPool& pool, int numThreads) const
        {
            const std::vector<internal::EntityIndex>& denseIds = minComponentPool_->getDenseChunks();
            size_t numElements = denseIds.size();
            size_t chunkSize = (numElements + numThreads - 1) / numThreads;

            std::vector<std::future<void>> futures;
            for (int i = 0; i < numThreads; i++)
            {
                size_t start = i * chunkSize;
                size_t end = (i == numThreads - 1) ? numElements : (start + chunkSize);

                if (start >= end)
                    break;

                futures.emplace_back(pool.enqueue([this, &f, &denseIds, start, end, entities = scene_.entities_] {
                    for (size_t j = start; j < end; j++)
                    {
                        internal::EntityIndex index = denseIds[j];
                        if (validIndex(index))
                        {
                            f(entities[index].id);
                        }
                    }
                }));
            }

            for (auto& future : futures)
                future.get();
        }

    private:
        bool validIndex(internal::EntityIndex index) const
        {
            return all_ || (signature_ == (signature_ & scene_.entities_[index].signature));
        }

    private:
        Scene& scene_;
        ComponentSignature signature_;
        bool all_;

        ComponentPool* minComponentPool_;
        size_t minComponentPoolSize_;

        friend class Iterator;
    };
}

#endif