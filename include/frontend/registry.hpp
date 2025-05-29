#pragma once

#include "../utilities/logging.hpp"

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
                Utilities::Throw("No object of type {} named {} found", boost::typeindex::type_id<T>().pretty_name(), id);
            }

            return Handle<T>{id};
        }

        template <typename T, typename... Args>
        void Add(const std::string& id, Args&... args)
        {
            if (Contains<T>(id))
            {
                Utilities::Throw("Object of type {} named {} already exists", boost::typeindex::type_id<T>().pretty_name(), id);
            }

            auto resource = std::make_shared<T>(args...);

            auto& storage = GetOrCreateStorage<T>();

            storage.objects[id] = std::move(resource);
        }

        template <typename T, typename... Args>
        std::string Add(Args&... args)
        {
            auto resource = std::make_shared<T>(args...);

            auto& storage = GetOrCreateStorage<T>();
            auto& counter = mNameCounters[std::type_index(typeid(T))];

            std::string id;
            do
            {
                id = std::format("{}#{}", boost::typeindex::type_id<T>().pretty_name(), counter++);
            } while (storage.objects.contains(id));

            storage.objects[id] = std::move(resource);

            return id;
        }

        template <typename T>
        T& Get(const Handle<T>& handle) const
        {
            return Get<T>(handle.GetID());
        }

        template <typename T>
        const std::unordered_map<std::string, std::shared_ptr<T>>& GetAll() const
        {
            auto it = mStorageMap.find(std::type_index(typeid(T)));
            if (it == mStorageMap.end())
            {
                Utilities::Throw("No objects of type {} found in registry", boost::typeindex::type_id<T>().pretty_name());
            }

            return static_cast<const Storage<T>*>(it->second.get())->objects;
        }

        template <typename T>
        T& Get(const std::string& id) const
        {
            if (not Contains<T>(id))
            {
                Utilities::Throw("No object of type {} named {} found", boost::typeindex::type_id<T>().pretty_name(), id);
            }

            const auto& storage = *static_cast<const Storage<T>*>(mStorageMap.at(std::type_index(typeid(T))).get());

            return *storage.objects.at(id).get();
        }

        template <typename T>
        void Remove(const std::string& id)
        {
            auto it = mStorageMap.find(std::type_index(typeid(T)));
            if (it == mStorageMap.end())
            {
                Utilities::Throw("No objects of type {} found in registry", boost::typeindex::type_id<T>().pretty_name());
            }

            auto& storage = *static_cast<Storage<T>*>(it->second.get());

            if (not storage.objects.contains(id))
            {
                Utilities::Throw("No object named {} of type {} found in registry", id, boost::typeindex::type_id<T>().pretty_name());
            }

            storage.objects.erase(id);

            LogCurrentResources<T>();
        }

        template <typename T>
        void Remove(const T& object)
        {
            const std::string id = GetID(&object);
            Remove<T>(id);
        }

        template <typename T>
        void RemoveAll()
        {
            auto it = mStorageMap.find(std::type_index(typeid(T)));
            if (it == mStorageMap.end())
            {
                Utilities::Throw("No objects of type {} found in registry", boost::typeindex::type_id<T>().pretty_name());
            }

            mStorageMap.erase(it);
        }

        template <typename T>
        std::string GetID(const T* ptr) const
        {
            auto it = mStorageMap.find(std::type_index(typeid(T)));
            if (it == mStorageMap.end())
            {
                Utilities::Throw("No objects of type {} found in registry", boost::typeindex::type_id<T>().pretty_name());
            }

            const auto& storage = *static_cast<const Storage<T>*>(it->second.get());

            for (const auto& [id, objPtr] : storage.objects)
            {
                if (objPtr.get() == ptr)
                {
                    return id;
                }
            }

            Utilities::Throw("Object pointer of type {} not found in registry", boost::typeindex::type_id<T>().pretty_name());
        }

        template <typename T>
        bool Contains(const std::string& id) const
        {
            auto it = mStorageMap.find(std::type_index(typeid(T)));
            if (it == mStorageMap.end())
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
        void LogCurrentResources() const
        {
            try
            {
                const auto& storage = GetAll<T>();
                std::string resourceList = "Current resources of type " + std::string(boost::typeindex::type_id<T>().pretty_name()) + ":\n";
                for (const auto& [id, _] : storage)
                {
                    resourceList += " - " + id + "\n";
                }
                Utilities::LogNotice("{}", resourceList);
            }
            catch (...)
            {
                Utilities::LogNotice("No resources of type {} found", boost::typeindex::type_id<T>().pretty_name());
            }
        }

        template <typename T>
        Storage<T>& GetOrCreateStorage()
        {
            auto type = std::type_index(typeid(T));
            auto& ptr = mStorageMap[type];

            if (not ptr)
            {
                ptr = std::make_unique<Storage<T>>();
            }

            return *static_cast<Storage<T>*>(ptr.get());
        }

        std::unordered_map<std::type_index, std::unique_ptr<IStorage>> mStorageMap;
        std::unordered_map<std::type_index, size_t> mNameCounters;
    };
}