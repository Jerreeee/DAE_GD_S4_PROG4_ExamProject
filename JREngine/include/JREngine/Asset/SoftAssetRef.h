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

        //Tries to get the asset, doesnt do anything if its not yet loaded
        AssetRef<T> TryGet() const
        {
            if (!m_Asset && m_Handle.IsValid())
                m_Asset = static_pointer_cast<T>(ServiceLocator::GetResourceManager().GetAsset(m_Handle, AssetLoadMode::Unspecified));
            return m_Asset;
        }

        //Gets the asset, blocking if not yet loaded and asset is owned by the ResourceManager
        AssetRef<T> Get() const
        {
            if (!m_Asset && m_Handle.IsValid())
                m_Asset = static_pointer_cast<T>(ServiceLocator::GetResourceManager().GetAsset(m_Handle, AssetLoadMode::Immediate));
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
