#pragma once

#include "logging.hpp"

#include <boost/type_index.hpp>

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace Mosaic::Frontend
{
    template <typename T>
    class Handle
    {
    public:
        Handle(std::string id)
            : ID(std::move(id))
        {
        }

        const std::string& GetID() const
        {
            return ID;
        }

    private:
        std::string ID;
    };

    class Registry
    {
    public:
        template <typename T>
        Handle<T> MakeHandle(const std::string& id) const
        {
            if (not Contains<T>(id))
            {
                Throw("No object of type {} named {} found", boost::typeindex::type_id<T>().pretty_name(), id);
            }

            return Handle<T>{id};
        }

        template <typename T>
        void Add(const std::string& id, std::shared_ptr<T> object)
        {
            if (Contains<T>(id))
            {
                Throw("Object of type {} named {} already exists", boost::typeindex::type_id<T>().pretty_name(), id);
            }

            auto& storage = GetOrCreateStorage<T>();

            storage.objects[id] = std::move(object);
        }

        template <typename T>
        T& Get(const Handle<T>& handle) const
        {
            return Get<T>(handle.GetID());
        }

        template <typename T>
        const std::unordered_map<std::string, std::shared_ptr<T>>& GetAll() const
        {
            auto it = storageMap.find(std::type_index(typeid(T)));
            if (it == storageMap.end())
            {
                Throw("No objects of type {} found in registry", boost::typeindex::type_id<T>().pretty_name());
            }

            return static_cast<const Storage<T>*>(it->second.get())->objects;
        }

        template <typename T>
        T& Get(const std::string& id) const
        {
            if (not Contains<T>(id))
            {
                Throw("No object of type {} named {} found", boost::typeindex::type_id<T>().pretty_name(), id);
            }

            const auto& storage = *static_cast<const Storage<T>*>(storageMap.at(std::type_index(typeid(T))).get());

            return *storage.objects.at(id).get();
        }

        template <typename T>
        void Remove(const std::string& id)
        {
            auto it = storageMap.find(std::type_index(typeid(T)));
            if (it == storageMap.end())
            {
                Throw("No objects of type {} found in registry", boost::typeindex::type_id<T>().pretty_name());
            }

            auto& storage = *static_cast<Storage<T>*>(it->second.get());

            if (not storage.objects.contains(id))
            {
                Throw("No object named {} of type {} found in registry", id, boost::typeindex::type_id<T>().pretty_name());
            }

            storage.objects.erase(id);
        }

        template <typename T>
        void RemoveAll()
        {
            auto it = storageMap.find(std::type_index(typeid(T)));
            if (it == storageMap.end())
            {
                Throw("No objects of type {} found in registry", boost::typeindex::type_id<T>().pretty_name());
            }

            storageMap.erase(it);
        }

        template <typename T>
        std::string GetID(const T* ptr) const
        {
            auto it = storageMap.find(std::type_index(typeid(T)));
            if (it == storageMap.end())
            {
                Throw("No objects of type {} found in registry", boost::typeindex::type_id<T>().pretty_name());
            }

            const auto& storage = *static_cast<const Storage<T>*>(it->second.get());

            for (const auto& [id, objPtr] : storage.objects)
            {
                if (objPtr.get() == ptr)
                {
                    return id;
                }
            }

            Throw("Object pointer of type {} not found in registry", boost::typeindex::type_id<T>().pretty_name());

            return "";
        }

        template <typename T>
        bool Contains(const std::string& id) const
        {
            auto it = storageMap.find(std::type_index(typeid(T)));
            if (it == storageMap.end())
            {
                return false;
            }

            const auto& storage = *static_cast<const Storage<T>*>(it->second.get());
            return storage.objects.contains(id);
        }

    private:
        struct IStorage
        {
            virtual ~IStorage() = default;
        };

        template <typename T>
        struct Storage : IStorage
        {
            std::unordered_map<std::string, std::shared_ptr<T>> objects;
        };

        template <typename T>
        Storage<T>& GetOrCreateStorage()
        {
            auto type = std::type_index(typeid(T));
            auto& ptr = storageMap[type];

            if (not ptr)
            {
                ptr = std::make_unique<Storage<T>>();
            }

            return *static_cast<Storage<T>*>(ptr.get());
        }

        std::unordered_map<std::type_index, std::unique_ptr<IStorage>> storageMap;
    };
}