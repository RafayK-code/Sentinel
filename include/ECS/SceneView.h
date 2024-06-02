#pragma once
#ifndef _SCENE_VIEW_H
#define _SCENE_VIEW_H

#include "Scene.h"

namespace sntl
{
    template<typename... ComponentTypes>
    class SceneView
    {
    public:
        SceneView(Scene* scene)
            : scene_(scene), all_(false), minComponentPoolSize_(scene->maxEntities_ + 1), minComponentPool_(nullptr)
        {
            if (sizeof...(ComponentTypes) == 0)
                all_ = true;
            else
            {
                int componentIds[] = { 0, getComponentID<ComponentTypes>() ... };
                for (int i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
                {
                    signature_.set(componentIds[i]);
                    if (scene_->componentPools_[componentIds[i]]->numChunks() < minComponentPoolSize_)
                    {
                        minComponentPoolSize_ = scene_->componentPools_[componentIds[i]]->numChunks();
                        minComponentPool_ = scene_->componentPools_[componentIds[i]];
                    }
                }
            }
        }

        ~SceneView()
        {
        }

        class SceneIterator
        {
        public:
            SceneIterator(Scene* scene, ComponentPool* pool, ComponentSignature signature, bool all, std::vector<size_t>::const_iterator curr)
                : scene_(scene), pool_(pool), signature_(signature), all_(all), current_(curr), end_(pool->getDenseChunks().end())
            {
            }

            EntityID operator*() const
            {
                return scene_->entities_[*current_].id;
            }

            SceneIterator& operator++()
            {
                advance();
                return *this;
            }

            SceneIterator operator++(int)
            {
                Iterator tmp = *this;
                advance();
                return tmp;
            }

            bool operator==(const SceneIterator& other) const
            {
                return pool_ == other.pool_ && current_ == other.current_;
            }

            bool operator!=(const SceneIterator& other) const
            {
                return pool_ != other.pool_ || current_ != other.current_;
            }

        private:
            bool validIndex(Scene::EntityIndex index)
            {
                return (all_ || signature_ == (signature_ & scene_->entities_[index].signature));
            }

            void advance()
            {
                ++current_;
                while (current_ != end_ && !validIndex(*current_))
                    ++current_;
            }

            Scene* scene_;
            ComponentPool* pool_;
            std::vector<size_t>::const_iterator current_;
            std::vector<size_t>::const_iterator end_;
            ComponentSignature signature_;
            bool all_;
        };

        using Iterator = SceneIterator;

        Iterator begin()
        {
            return Iterator(scene_, minComponentPool_, signature_, all_, minComponentPool_->getDenseChunks().begin());
        }

        Iterator end()
        {
            return Iterator(scene_, minComponentPool_, signature_, all_, minComponentPool_->getDenseChunks().end());
        }

    private:
        Scene* scene_;
        ComponentSignature signature_;
        bool all_;

        ComponentPool* minComponentPool_;
        size_t minComponentPoolSize_;

        friend Iterator;
    };
}

#endif