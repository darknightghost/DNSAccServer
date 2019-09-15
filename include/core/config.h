#pragma once

#include <string>
#include <memory>
#include <vector>
#include <type_traits>

#include <rapidjson/document.h>
#include <rapidjson/pointer.h>

/**
 * brief    JSON config file.
 */
class Config {
    protected:
        ::std::unique_ptr<::rapidjson::Document>    m_doc;      //< Document.

    protected:
        /**
         * @brief       Constructor.
         *
         * @param[in]   doc     Document.
         */
        Config(::std::unique_ptr<::rapidjson::Document>  doc);

    public:
        /**
         * @brief       Constructor.
         *
         * @param[in]   path    Path.
         */
        Config(const ::std::string&     path);

        /**
         * @brief       Move constructor.
         *
         * @param[in]   o       Old object.
         */
        Config(Config&&                 o);

        /**
         * @brief       Get value.
         *
         * @param[in]   key         Key.
         * @param[in]   defaultVal  Default value.
         *
         * @return      If the key exists, the value of the key is returned.
         *              Otherwise returns the value of defaultVal.
         */
        template <typename T>
        typename ::std::enable_if<::std::is_signed<T>::value, T>::type
        get(
            const ::std::string&    key,
            T                       defaultVal)     const;

        /**
         * @brief       Get value.
         *
         * @param[in]   key         Key.
         * @param[in]   defaultVal  Default value.
         *
         * @return      If the key exists, the value of the key is returned.
         *              Otherwise returns the value of defaultVal.
         */
        template <typename T>
        typename ::std::enable_if<::std::is_unsigned<T>::value, T>::type
        get(
            const ::std::string&    key,
            T                       defaultVal)     const;

        /**
         * @brief       Get value.
         *
         * @param[in]   key         Key.
         * @param[in]   defaultVal  Default value.
         *
         * @return      If the key exists, the value of the key is returned.
         *              Otherwise returns the value of defaultVal.
         */
        template <typename T>
        typename ::std::enable_if<::std::is_floating_point<T>::value, T>::type
        get(
            const ::std::string&    key,
            T                       defaultVal)     const;

        /**
         * @brief       Get value.
         *
         * @param[in]   key         Key.
         * @param[in]   defaultVal  Default value.
         *
         * @return      If the key exists, the value of the key is returned.
         *              Otherwise returns the value of defaultVal.
         */
        ::std::string       get(
            const ::std::string&    key,
            const ::std::string&    defaultVal)     const;

        /**
         * @brief       Get array.
         *
         * @param[in]   key         Key.
         *
         * @return      If the key exists, a vertor with values is returned,
         *              otherwise returns an empty vector.
         */
        template <typename T>
        ::std::vector<T>    get(
            const ::std::string&    key)            const;

        /**
         * @brief       Get array.
         *
         * @param[in]   key         Key.
         *
         * @return      If the key exists, a vertor with values is returned,
         *              otherwise returns an empty vector.
         */
        template <typename T>
        ::std::vector<typename
        ::std::enable_if<::std::is_signed<T>::value, T>::type>    get(
            const ::std::string&    key)            const;

        /**
         * @brief       Get array.
         *
         * @param[in]   key         Key.
         *
         * @return      If the key exists, a vertor with values is returned,
         *              otherwise returns an empty vector.
         */
        template <typename T>
        ::std::vector<typename
        ::std::enable_if<::std::is_unsigned<T>::value, T>::type>    get(
            const ::std::string&    key)            const;

        /**
         * @brief       Get array.
         *
         * @param[in]   key         Key.
         *
         * @return      If the key exists, a vertor with values is returned,
         *              otherwise returns an empty vector.
         */
        template <typename T>
        ::std::vector<typename
        ::std::enable_if<::std::is_floating_point<T>::value, T>::type>    get(
            const ::std::string&    key)            const;

        /**
         * @brief       Get array.
         *
         * @param[in]   key         Key.
         *
         * @return      If the key exists, a vertor with values is returned,
         *              otherwise returns an empty vector.
         */
        ::std::vector<Config>    get(
            const ::std::string&    key)            const;

        /**
         * @brief       Destructor.
         */
        virtual             ~Config();
};
