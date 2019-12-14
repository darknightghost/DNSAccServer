#pragma once

#include <cstdint>
#include <vector>
#include <type_traits>

#include <rapidjson/document.h>

#include <common/design_models/singleton.h>
#include <common/config/config_node.h>

namespace dnsAccServer {

/**
 * @brief   Config.
 */
class Config : public Singleton<Config, const ::std::string&, ::std::string&> {
    protected:
        const ::std::string             m_path;     //< Path of config file.
        ::rapidjson::Document           m_document; //< Json document.

    protected:
        static ::std::vector<::std::shared_ptr<ConfigNode>>         _registedNodes; //< Registed config nodes

    protected:
        /**
         * @brief       Constructor.
         *
         * @param[in]   path        Path of config file.
         * @param[out]  err         Error info if falied.
         */
        Config(const ::std::string& path,
               ::std::string&       err);

    public:
        /**
         * @brief       Regist config node.
         *
         * @param[in]   node        Config node.
         */
        static void     registerConfigNode(
            ::std::shared_ptr<ConfigNode>       node);

    public:
        /**
         * @brief       Get path of config file.
         *
         * @return      Path of config file.
         */
        const ::std::string&                path() const;

        /**
         * @brief       Get value by path.
         *
         * @param[in]   path            Path.
         * @param[in]   defaultValue    Default value.
         *
         * @return      If the values exists, the value is returned, otherwise
         *              returns default value.
         */
        int8_t          getInt8(const ::std::string&    path,
                                int8_t                  defaultValue);

        /**
         * @brief       Get value by path.
         *
         * @param[in]   path            Path.
         * @param[in]   defaultValue    Default value.
         *
         * @return      If the values exists, the value is returned, otherwise
         *              returns default value.
         */
        uint8_t         getUint8(const ::std::string&   path,
                                 uint8_t                defaultValue);

        /**
         * @brief       Get value by path.
         *
         * @param[in]   path            Path.
         * @param[in]   defaultValue    Default value.
         *
         * @return      If the values exists, the value is returned, otherwise
         *              returns default value.
         */
        int16_t         getInt16(const ::std::string&   path,
                                 int16_t                defaultValue);

        /**
         * @brief       Get value by path.
         *
         * @param[in]   path            Path.
         * @param[in]   defaultValue    Default value.
         *
         * @return      If the values exists, the value is returned, otherwise
         *              returns default value.
         */
        uint16_t         getUint16(const ::std::string& path,
                                   uint16_t             defaultValue);

        /**
         * @brief       Get value by path.
         *
         * @param[in]   path            Path.
         * @param[in]   defaultValue    Default value.
         *
         * @return      If the values exists, the value is returned, otherwise
         *              returns default value.
         */
        int32_t         getInt32(const ::std::string&   path,
                                 int32_t                 defaultValue);

        /**
         * @brief       Get value by path.
         *
         * @param[in]   path            Path.
         * @param[in]   defaultValue    Default value.
         *
         * @return      If the values exists, the value is returned, otherwise
         *              returns default value.
         */
        uint32_t        getUint32(const ::std::string&  path,
                                  uint32_t                defaultValue);

        /**
         * @brief       Get value by path.
         *
         * @param[in]   path            Path.
         * @param[in]   defaultValue    Default value.
         *
         * @return      If the values exists, the value is returned, otherwise
         *              returns default value.
         */
        int64_t         getInt64(const ::std::string&   path,
                                 int64_t                 defaultValue);

        /**
         * @brief       Get value by path.
         *
         * @param[in]   path            Path.
         * @param[in]   defaultValue    Default value.
         *
         * @return      If the values exists, the value is returned, otherwise
         *              returns default value.
         */
        uint64_t        getUint64(const ::std::string&  path,
                                  uint64_t                defaultValue);

        /**
         * @brief       Get value by path.
         *
         * @param[in]   path            Path.
         * @param[in]   defaultValue    Default value.
         *
         * @return      If the values exists, the value is returned, otherwise
         *              returns default value.
         */
        float           getFloat(const ::std::string&   path,
                                 float                   defaultValue);

        /**
         * @brief       Get value by path.
         *
         * @param[in]   path            Path.
         * @param[in]   defaultValue    Default value.
         *
         * @return      If the values exists, the value is returned, otherwise
         *              returns default value.
         */
        double          getDouble(const ::std::string&  path,
                                  double                  defaultValue);

        /**
         * @brief       Get value by path.
         *
         * @param[in]   path            Path.
         * @param[in]   defaultValue    Default value.
         *
         * @return      If the values exists, the value is returned, otherwise
         *              returns default value.
         */
        bool            getBool(const ::std::string&    path,
                                bool                    defaultValue);

        /**
         * @brief       Get value by path.
         *
         * @param[in]   path            Path.
         * @param[in]   defaultValue    Default value.
         *
         * @return      If the values exists, the value is returned, otherwise
         *              returns default value.
         */
        ::std::string   getString(const ::std::string&  path,
                                  const ::std::string&    defaultValue);

        /**
         * @brief   Destructor.
         */
        virtual     ~Config();

    protected:
        /**
         * @brief       Implements of getXXX functions.
         *
         * @param[in]   path            Path.
         * @param[out]  value           Value returned.
         *
         * @return      If the values exists, true is returned, otherwise
         *              returns false.
         */
        template <typename T>
        bool        getValueImpl(
            const ::std::string&  path,
            typename ::std::enable_if <
            ::std::is_same<T, int8_t>::value
            ||::std::is_same<T, int16_t>::value
            ||::std::is_same<T, int32_t>::value
            ||::std::is_same<T, int64_t>::value,
            T >::type&             value);

        /**
         * @brief       Implements of getXXX functions.
         *
         * @param[in]   path            Path.
         * @param[out]  value           Value returned.
         *
         * @return      If the values exists, true is returned, otherwise
         *              returns false.
         */
        template <typename T>
        bool        getValueImpl(
            const ::std::string&  path,
            typename ::std::enable_if <
            ::std::is_same<T, uint8_t>::value
            ||::std::is_same<T, uint16_t>::value
            ||::std::is_same<T, uint32_t>::value
            ||::std::is_same<T, uint64_t>::value,
            T >::type&             value);

        /**
         * @brief       Implements of getXXX functions.
         *
         * @param[in]   path            Path.
         * @param[out]  value           Value returned.
         *
         * @return      If the values exists, true is returned, otherwise
         *              returns false.
         */
        template <typename T>
        bool        getValueImpl(
            const ::std::string&  path,
            typename ::std::enable_if <
            ::std::is_same<T, float>::value
            ||::std::is_same<T, double>::value,
            T >::type&             value);

        /**
         * @brief       Implements of getXXX functions.
         *
         * @param[in]   path            Path.
         * @param[out]  value           Value returned.
         *
         * @return      If the values exists, true is returned, otherwise
         *              returns false.
         */
        template <typename T>
        bool        getValueImpl(
            const ::std::string&  path,
            typename ::std::enable_if <
            ::std::is_same<T, bool>::value,
            T >::type&             value);

        /**
         * @brief       Implements of getXXX functions.
         *
         * @param[in]   path            Path.
         * @param[out]  value           Value returned.
         *
         * @return      If the values exists, true is returned, otherwise
         *              returns false.
         */
        template <typename T>
        bool        getValueImpl(
            const ::std::string&  path,
            typename ::std::enable_if <
            ::std::is_same<T, ::std::string>::value,
            T >::type&             value);

};
}