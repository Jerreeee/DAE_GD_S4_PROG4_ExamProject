#pragma once
#include <cassert>
#include <string>
#include <vector>
#include <iostream>

namespace JRE
{
    using TypeID = size_t;

    template<typename CategoryTag>
    struct TypeInfo
    {
        TypeID id;
        std::string name;
    };

    class IRegisteredType
    {
    public:
        virtual ~IRegisteredType() = default;
        virtual TypeID GetTypeID() const = 0;
        virtual const std::string& GetTypeName() const = 0;
    };

    template<typename CategoryTag>
    class TypeRegistry
    {
    public:
        using CategoryTag_t = CategoryTag;

        template<typename T>
        static TypeID RegisterType(const std::string& typeName)
        {
            static_assert(std::is_same_v<typename T::CategoryTag_t, CategoryTag_t>,
                "Type T is not registered under the correct category registry.");

            TypeID id = s_NextID++;
            s_TypeInfos.emplace_back(TypeInfo<CategoryTag_t>{ id, typeName });
            return id;
        }

        template<typename T>
        static const TypeInfo<CategoryTag_t>& GetInfo()
        {
            static_assert(std::is_same_v<typename T::CategoryTag_t, CategoryTag_t>,
                "Type T is not registered under the correct category registry.");

            TypeID id = T::GetStaticTypeID();
            assert(id < s_TypeInfos.size());
            return s_TypeInfos[id];
        }

        static const std::vector<TypeInfo<CategoryTag_t>>& GetTypesInfo() { return s_TypeInfos; };
    private:
        inline static std::vector<TypeInfo<CategoryTag_t>> s_TypeInfos{};
        inline static TypeID s_NextID{};
    };
}

#define REGISTER_TYPE_HEADER(CategoryTag) \
public: \
    using CategoryTag_t = CategoryTag; \
    static JRE::TypeID GetStaticTypeID(); \
    virtual JRE::TypeID GetTypeID() const override; \
    static const std::string& GetStaticTypeName(); \
    virtual const std::string& GetTypeName() const override; \
    static_assert(true, "REGISTER_TYPE_HEADER requires a semicolon")

#define REGISTER_TYPE_SOURCE(T, CategoryTag, QualifiedNameString) \
    JRE::TypeID T::GetStaticTypeID() { \
        static const JRE::TypeID id = JRE::TypeRegistry<CategoryTag>::RegisterType<T>(QualifiedNameString); \
        return id; \
    } \
    JRE::TypeID T::GetTypeID() const { \
        return GetStaticTypeID(); \
    } \
    const std::string& T::GetStaticTypeName() { \
        GetStaticTypeID(); \
        return JRE::TypeRegistry<CategoryTag>::GetInfo<T>().name; \
    } \
    const std::string& T::GetTypeName() const { \
        return GetStaticTypeName(); \
    } \
    namespace { \
        const bool s_Registered_##T = []() { \
            T::GetStaticTypeID(); \
            return true; \
        }(); \
    } \
    static_assert(true, "REGISTER_TYPE_SOURCE requires a semicolon")

