#include <fstream>
#include <sstream>

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>
#include <rapidjson/pointer.h>

#include <common/config/config.h>

namespace dnsAccServer {

SIGNLETOM_DEFINATION(Config, const ::std::string&, ::std::string&);

::std::vector<::std::shared_ptr<ConfigNode>>         Config::_registedNodes; //< Registed config nodes

/**
 * @brief       Regist config node.
 *
 * @param[in]   node        Config node.
 */
void Config::registerConfigNode(
    ::std::shared_ptr<ConfigNode>       node)
{
    _registedNodes.push_back(node);
}

/**
 * @brief       Constructor.
 *
 * @param[in]   path        Path of config file.
 * @param[out]  err         Error info if falied.
 */
Config::Config(const ::std::string& path,
               ::std::string&       err) :
    m_path(path)
{
    ::std::ifstream ifs(path);

    ::rapidjson::IStreamWrapper iws(ifs);

    m_document.ParseStream(iws);

    if(m_document.HasParseError()) {
        ::std::ostringstream ss;
        ss << "Parse error at offset " << m_document.GetErrorOffset()
           << " : " <<::rapidjson::GetParseError_En(m_document.GetParseError());
        err = ss.str();
        return;
    }

    this->setGood();
}

/**
 * @brief       Get path of config file.
 *
 * @return      Path of config file.
 */
const ::std::string&                Config::path() const
{
    return m_path;
}

/**
 * @brief       Get value by path.
 *
 * @param[in]   path            Path.
 * @param[in]   defaultValue    Default value.
 *
 * @return      If the values exists, the value is returned, otherwise
 *              returns default value.
 */
int8_t          Config::getInt8(const ::std::string&    path,
                                int8_t                  defaultValue)
{
    int8_t ret;

    if(this->getValueImpl<int8_t>(path, ret)) {
        return ret;

    } else {
        return defaultValue;
    }
}

/**
 * @brief       Get value by path.
 *
 * @param[in]   path            Path.
 * @param[in]   defaultValue    Default value.
 *
 * @return      If the values exists, the value is returned, otherwise
 *              returns default value.
 */
uint8_t         Config::getUint8(const ::std::string&   path,
                                 uint8_t                defaultValue)
{
    uint8_t ret;

    if(this->getValueImpl<uint8_t>(path, ret)) {
        return ret;

    } else {
        return defaultValue;
    }
}

/**
 * @brief       Get value by path.
 *
 * @param[in]   path            Path.
 * @param[in]   defaultValue    Default value.
 *
 * @return      If the values exists, the value is returned, otherwise
 *              returns default value.
 */
int16_t         Config::getInt16(const ::std::string&   path,
                                 int16_t                defaultValue)
{
    int16_t ret;

    if(this->getValueImpl<int16_t>(path, ret)) {
        return ret;

    } else {
        return defaultValue;
    }
}

/**
 * @brief       Get value by path.
 *
 * @param[in]   path            Path.
 * @param[in]   defaultValue    Default value.
 *
 * @return      If the values exists, the value is returned, otherwise
 *              returns default value.
 */
uint16_t         Config::getUint16(const ::std::string& path,
                                   uint16_t             defaultValue)
{
    uint16_t ret;

    if(this->getValueImpl<uint16_t>(path, ret)) {
        return ret;

    } else {
        return defaultValue;
    }
}

/**
 * @brief       Get value by path.
 *
 * @param[in]   path            Path.
 * @param[in]   defaultValue    Default value.
 *
 * @return      If the values exists, the value is returned, otherwise
 *              returns default value.
 */
int32_t         Config::getInt32(const ::std::string&   path,
                                 int32_t                 defaultValue)
{
    int32_t ret;

    if(this->getValueImpl<int32_t>(path, ret)) {
        return ret;

    } else {
        return defaultValue;
    }
}

/**
 * @brief       Get value by path.
 *
 * @param[in]   path            Path.
 * @param[in]   defaultValue    Default value.
 *
 * @return      If the values exists, the value is returned, otherwise
 *              returns default value.
 */
uint32_t        Config::getUint32(const ::std::string&  path,
                                  uint32_t                defaultValue)
{
    uint32_t ret;

    if(this->getValueImpl<uint32_t>(path, ret)) {
        return ret;

    } else {
        return defaultValue;
    }
}

/**
 * @brief       Get value by path.
 *
 * @param[in]   path            Path.
 * @param[in]   defaultValue    Default value.
 *
 * @return      If the values exists, the value is returned, otherwise
 *              returns default value.
 */
int64_t         Config::getInt64(const ::std::string&   path,
                                 int64_t                 defaultValue)
{
    int64_t ret;

    if(this->getValueImpl<int64_t>(path, ret)) {
        return ret;

    } else {
        return defaultValue;
    }
}

/**
 * @brief       Get value by path.
 *
 * @param[in]   path            Path.
 * @param[in]   defaultValue    Default value.
 *
 * @return      If the values exists, the value is returned, otherwise
 *              returns default value.
 */
uint64_t        Config::getUint64(const ::std::string&  path,
                                  uint64_t                defaultValue)
{
    uint64_t ret;

    if(this->getValueImpl<uint64_t>(path, ret)) {
        return ret;

    } else {
        return defaultValue;
    }
}

/**
 * @brief       Get value by path.
 *
 * @param[in]   path            Path.
 * @param[in]   defaultValue    Default value.
 *
 * @return      If the values exists, the value is returned, otherwise
 *              returns default value.
 */
float           Config::getFloat(const ::std::string&   path,
                                 float                   defaultValue)
{
    float ret;

    if(this->getValueImpl<float>(path, ret)) {
        return ret;

    } else {
        return defaultValue;
    }
}

/**
 * @brief       Get value by path.
 *
 * @param[in]   path            Path.
 * @param[in]   defaultValue    Default value.
 *
 * @return      If the values exists, the value is returned, otherwise
 *              returns default value.
 */
double          Config::getDouble(const ::std::string&  path,
                                  double                  defaultValue)
{
    double ret;

    if(this->getValueImpl<double>(path, ret)) {
        return ret;

    } else {
        return defaultValue;
    }
}

/**
 * @brief       Get value by path.
 *
 * @param[in]   path            Path.
 * @param[in]   defaultValue    Default value.
 *
 * @return      If the values exists, the value is returned, otherwise
 *              returns default value.
 */
bool            Config::getBool(const ::std::string&    path,
                                bool                    defaultValue)
{
    bool ret;

    if(this->getValueImpl<bool>(path, ret)) {
        return ret;

    } else {
        return defaultValue;
    }
}

/**
 * @brief       Get value by path.
 *
 * @param[in]   path            Path.
 * @param[in]   defaultValue    Default value.
 *
 * @return      If the values exists, the value is returned, otherwise
 *              returns default value.
 */
::std::string   Config::getString(const ::std::string&  path,
                                  const ::std::string&    defaultValue)
{
    ::std::string ret;

    if(this->getValueImpl<::std::string>(path, ret)) {
        return ret;

    } else {
        return defaultValue;
    }
}

/**
 * @brief   Destructor.
 */
Config::~Config()
{
}

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
bool        Config::getValueImpl(
    const ::std::string&  path,
    typename ::std::enable_if <
    ::std::is_same<T, int8_t>::value
    ||::std::is_same<T, int16_t>::value
    ||::std::is_same<T, int32_t>::value
    ||::std::is_same<T, int64_t>::value,
    T >::type&             value)
{
    ::rapidjson::Value* v = ::rapidjson::Pointer(path.c_str()).Get(m_document);

    if(v == nullptr) {
        return false;
    }

    if(v->IsInt64()) {
        value = (T)(v->GetInt64());
        return true;

    } else {
        return false;

    }
}

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
bool        Config::getValueImpl(
    const ::std::string&  path,
    typename ::std::enable_if <
    ::std::is_same<T, uint8_t>::value
    ||::std::is_same<T, uint16_t>::value
    ||::std::is_same<T, uint32_t>::value
    ||::std::is_same<T, uint64_t>::value,
    T >::type&             value)
{
    ::rapidjson::Value* v = ::rapidjson::Pointer(path.c_str()).Get(m_document);

    if(v == nullptr) {
        return false;;
    }

    if(v->IsUint64()) {
        value = (T)(v->GetUint64());
        return true;

    } else {
        return false;

    }
}

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
bool        Config::getValueImpl(
    const ::std::string&  path,
    typename ::std::enable_if <
    ::std::is_same<T, float>::value
    ||::std::is_same<T, double>::value,
    T >::type&             value)
{
    ::rapidjson::Value* v = ::rapidjson::Pointer(path.c_str()).Get(m_document);

    if(v == nullptr) {
        return false;
    }

    if(v->IsDouble()) {
        value = (T)(v->GetDouble());
        return true;

    } else {
        return false;;

    }
}

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
bool        Config::getValueImpl(
    const ::std::string&  path,
    typename ::std::enable_if <
    ::std::is_same<T, bool>::value,
    T >::type&             value)
{
    ::rapidjson::Value* v = ::rapidjson::Pointer(path.c_str()).Get(m_document);

    if(v == nullptr) {
        return false;
    }

    if(v->IsBool()) {
        value = v->GetBool();
        return true;

    } else {
        return false;

    }
}

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
bool        Config::getValueImpl(
    const ::std::string&  path,
    typename ::std::enable_if <
    ::std::is_same<T, ::std::string>::value,
    T >::type&             value)
{
    ::rapidjson::Value* v = ::rapidjson::Pointer(path.c_str()).Get(m_document);

    if(v == nullptr) {
        return false;
    }

    if(v->IsString()) {
        value = ::std::move(::std::string(v->GetString()));
        return true;

    } else {
        return false;

    }
}
}
