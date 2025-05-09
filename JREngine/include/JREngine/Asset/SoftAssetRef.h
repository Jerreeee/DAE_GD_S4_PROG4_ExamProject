#pragma once
#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Asset/IResourceManager.h"
#include <memory>

namespace JRE
{
    template<IsAsset T>
    class SoftAssetRef
    {
    public:
        SoftAssetRef() = default;
        explicit SoftAssetRef(std::nullptr_t) {};

        //Pass a valid handle to an asset that exists in the ResourceManager
        explicit SoftAssetRef(AssetHandle handle) :
            m_Handle{ handle }
        {}
        //Pass a valid handle and asset that exists in the ResourceManager
        explicit SoftAssetRef(AssetHandle handle, AssetRef<T> asset) :
            m_Handle{ handle },
            m_Asset{ asset }
        {}
        //Pass an asset that may or may not be owned by the ResourceManager
        //This depends on if the handle is valid or not
        explicit SoftAssetRef(AssetRef<T> asset) :
            m_Handle{ asset ? std::static_pointer_cast<Asset>(asset)->GetHandle() : AssetHandle::InvalidUUID },
            m_Asset{ asset }
        {}

        //If the internal asset is not nullptr
        bool IsLoaded() const { return m_Asset != nullptr; }

        //If the asset is not yet loaded and the asset is owned by ResourceManager,
        //then try to load it from the ResourceManager
        bool TryLoad() const
        {
            if (!m_Asset && m_Handle.IsValid())
                m_Asset = static_pointer_cast<T>(ServiceLocator::GetResourceManager().GetAsset(m_Handle, AssetLoadMode::Unspecified));
            return m_Asset != nullptr;
        }

        //If the asset is not yet loaded and the asset is owned by ResourceManager,
        //then load it form the ResourceManager
        void Load() const
        {
            if (!m_Asset && m_Handle.IsValid())
                m_Asset = static_pointer_cast<T>(ServiceLocator::GetResourceManager().GetAsset(m_Handle, AssetLoadMode::Immediate));
        }

        //Only try reloading if the asset is owned by ResourceManager
        bool TryReload()
        {
            if (!m_Handle.IsValid())
                return false;

            //Only updates m_Asset if GetAsset() was successful
            auto newAsset = static_pointer_cast<T>(ServiceLocator::GetResourceManager().GetAsset(m_Handle, AssetLoadMode::Unspecified));
            if (newAsset)
                m_Asset = newAsset;

            return m_Asset != nullptr;
        }

        //Only reload if the asest exists in the ResourceManager
        void Reload() const
        {
            if (!m_Handle.IsValid())
                return;

            m_Asset = static_pointer_cast<T>(ServiceLocator::GetResourceManager().GetAsset(m_Handle, AssetLoadMode::Immediate));
        }

        //Tries to get the asset, doesnt do anything if its not yet loaded
        AssetRef<T> TryGet() const
        {
            TryLoad();
            return m_Asset;
        }

        //Gets the asset, blocking if not yet loaded and asset is owned by the ResourceManager
        AssetRef<T> Get() const
        {
            Load();
            return m_Asset;
        }

        // smart pointer style API
        explicit operator bool() const { return Get() != nullptr; }

        T* operator->() { return Get().get(); }
        const T* operator->() const { return Get().get(); }

        T& operator*() { return *Get(); }
        const T& operator*() const { return *Get(); }

        AssetHandle GetHandle() const { return m_Handle; }
    private:
        AssetHandle m_Handle{ AssetHandle::InvalidUUID };
        mutable AssetRef<T> m_Asset{ nullptr };
    };
}
