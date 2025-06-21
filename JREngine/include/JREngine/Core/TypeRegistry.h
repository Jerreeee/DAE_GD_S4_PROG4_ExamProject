#pragma once
#include <cassert>
#include <string>
#include <vector>
#include <concepts>
#include <iostream>
#include "JREngine/Core/ServiceLocator.h"

namespace JRE
{
    using TypeID = size_t;

    template<typename T>
    concept HasNameMember = requires(T t) {
        { t.name } -> std::convertible_to<const std::string&>;
    };

    struct TypeInfoBase
    {
        virtual ~TypeInfoBase() = default;
        std::string name;
    };

    class IRegisteredType
    {
    public:
        virtual ~IRegisteredType() = default;
        virtual TypeID GetTypeID() const = 0;
        virtual const TypeInfoBase& GetTypeInfo() const = 0;
    };


    /// <summary>
    /// The TypeInfo type must be a unique struct for each different category of types that need to have registered ID's.
    /// And at a minimum it must have a std::string name member.
    /// Example:
    ///     struct TypeInfo
    ///     {
    ///         std::string name;
    ///     };
    /// </summary>
    template<typename TypeInfo>
    requires HasNameMember<TypeInfo>
    class TypeRegistry
    {
    public:
        TypeID RegisterType(const TypeInfo& typeInfo)
        {
            TypeID id = s_NextID++;
            s_TypeInfos.emplace_back(typeInfo);
            return id;
        }

        const TypeInfo& GetTypeInfo(TypeID id)
        {
            assert(id < s_TypeInfos.size());
            return s_TypeInfos[id];
        }

        const std::vector<TypeInfo>& GetTypesInfo() { return s_TypeInfos; };
    private:
        std::vector<TypeInfo> s_TypeInfos{};
        TypeID s_NextID{};
    };
}

/// <summary>
/// For each type that inherits from a base type that implements IRegisteredType, you must register it with the macro below in the header file
/// </summary>
/// <param name="TypeName">The name of the type without any scopes. For example write: MyType instead of Foo::Bar::MyType</param>
/// <param name="TypeInfoStruct_t">The unique struct type used to store typeInfo</param>
/// <param name="QualifiedNameString">The fully qualified name of the type. For example for type MyType inside  the namespace Foo::Bar write Foo::Bar::MyType</param>
#define REGISTER_TYPE_HEADER(TypeName, TypeInfoStruct, QualifiedNameString) \
    using TypeInfoStruct_t = TypeInfoStruct;
    static JRE::TypeID TypeName::GetStaticTypeID() \
    { \
        static const JRE::TypeID id = JRE::TypeRegistry<TypeInfoStruct_t>::RegisterType(QualifiedNameString); \
        return id; \
    } \
    virtual JRE::TypeID TypeName::GetTypeID() const override \
    { \
        return GetStaticTypeID(); \
    } \
    static_assert(true, "REGISTER_TYPE_HEADER requires a semicolon")

/// <summary>
/// For each type that inherits from a base type that implements IRegisteredType, you must register it with the macro below in the header file
/// </summary>
/// <param name="TypeName">The name of the type without any scopes. For example write: MyType instead of Foo::Bar::MyType</param>
#define REGISTER_TYPE_WITH_ID_FROM(TypeName) \
    static TypeID GetStaticTypeID() \
    { \
        return TypeName::GetStaticTypeID(); \
    } \
    virtual JRE::TypeID TypeName::GetTypeID() const override \
    { \
        return GetStaticTypeID(); \
    } \
    static_assert(true, "REGISTER_TYPE_WITH_ID_FROM requires a semicolon")

/// <summary>
/// For each type that inherits from a base type that implements IRegisteredType, you must register it with the macro below in the cpp file
/// </summary>
/// <param name="TypeName">The name of the type without any scopes. For example write: MyType instead of Foo::Bar::MyType</param>
#define REGISTER_TYPE_SOURCE(TypeName) \
    namespace \
    { \
        const bool s_Registered_##TypeName = []() { \
            TypeName::GetStaticTypeID(); \
            return true; \
        }(); \
    } \
    static_assert(true, "REGISTER_TYPE_SOURCE requires a semicolon")
