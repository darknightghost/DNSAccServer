#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <type_traits>

namespace dnsAccServer {

/**
 * @brief   Config register.
 */
class ConfigNode {
    public:
        /**
         * @brief   Value types.
         */
        enum Type {
            Int8_t          = 0,
            Uint8_t         = 1,
            Int16_t         = 2,
            Uint16_t        = 3,
            Int32_t         = 4,
            Uint32_t        = 5,
            Int64_t         = 6,
            Uint64_t        = 7,
            Float           = 8,
            Double          = 9,
            Bool            = 10,
            String          = 11,
            Array           = 12,
            Object          = 13
        };

        /**
         * @brief   Type converter.
         */
        template <Type  tp>
        class BasicType {
        };
    protected:
        Type                                m_type;         //< Data type.
        ::std::string                       m_path;         //< Node path.
        bool                                m_required;     //< True if required, otherwise returns false.

    protected:
        /**
         * @brief       Constructor.
         *
         * @param[in]   type            Data type.
         * @param[in]   path            Path of config node.
         * @param[in]   required        True if required, otherwise returns false.
         */
        ConfigNode(
            Type                            type,
            const ::std::string&            path,
            bool                            required);

    public:
        /**
         * @brief   Get config type.
         *
         * @return  Contig type.
         */
        Type                        type()          const;

        /**
         * @brief   Get config path.
         *
         * @return  Path of config node.
         */
        const ::std::string&        path()          const;

        /**
         * @brief   Get required status.
         *
         * @return  True if the config node must exists, otherwise returns false.
         */
        bool                        required()      const;

        /**
         * @brief   Destructor.
         */
        virtual     ~ConfigNode();
};

/**
 * @brief   Type converter.
 */
template <>
class ConfigNode::BasicType<ConfigNode::Type::Int8_t> {
    public:
        typedef int8_t      type;   //< Converted type.
};

/**
 * @brief   Type converter.
 */
template <>
class ConfigNode::BasicType<ConfigNode::Type::Uint8_t> {
    public:
        typedef uint8_t     type;   //< Converted type.
};

/**
 * @brief   Type converter.
 */
template <>
class ConfigNode::BasicType<ConfigNode::Type::Int16_t> {
    public:
        typedef int16_t     type;   //< Converted type.
};

/**
 * @brief   Type converter.
 */
template <>
class ConfigNode::BasicType<ConfigNode::Type::Uint16_t> {
    public:
        typedef uint16_t    type;   //< Converted type.
};

/**
 * @brief   Type converter.
 */
template <>
class ConfigNode::BasicType<ConfigNode::Type::Int32_t> {
    public:
        typedef int32_t     type;   //< Converted type.
};

/**
 * @brief   Type converter.
 */
template <>
class ConfigNode::BasicType<ConfigNode::Type::Uint32_t> {
    public:
        typedef uint32_t    type;   //< Converted type.
};

/**
 * @brief   Type converter.
 */
template <>
class ConfigNode::BasicType<ConfigNode::Type::Int64_t> {
    public:
        typedef int64_t     type;   //< Converted type.
};

/**
 * @brief   Type converter.
 */
template <>
class ConfigNode::BasicType<ConfigNode::Type::Uint64_t> {
    public:
        typedef uint64_t    type;   //< Converted type.
};

/**
 * @brief   Type converter.
 */
template <>
class ConfigNode::BasicType<ConfigNode::Type::Float> {
    public:
        typedef float       type;   //< Converted type.
};

/**
 * @brief   Type converter.
 */
template <>
class ConfigNode::BasicType<ConfigNode::Type::Double> {
    public:
        typedef double      type;   //< Converted type.
};

/**
 * @brief   Type converter.
 */
template <>
class ConfigNode::BasicType<ConfigNode::Type::Bool> {
    public:
        typedef bool        type;   //< Converted type.
};

/**
 * @brief   Type converter.
 */
template <>
class ConfigNode::BasicType<ConfigNode::Type::String> {
    public:
        typedef ::std::string   type;   //< Converted type.
};
};

