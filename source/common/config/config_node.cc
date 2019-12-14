#include <common/common.h>

#include <common/config/config.h>
#include <common/config/config_node.h>

namespace dnsAccServer {

/**
 * @brief       Constructor.
 *
 * @param[in]   type            Data type.
 * @param[in]   path            Path of config node.
 * @param[in]   required        True if required, otherwise returns false.
 */
ConfigNode::ConfigNode(
    ConfigNode::Type            type,
    const ::std::string&            path,
    bool                            required) :
    m_type(type),
    m_path(path),
    m_required(required)
{
}

/**
 * @brief   Get config type.
 *
 * @return  Contig type.
 */
ConfigNode::Type ConfigNode::type() const
{
    return m_type;
}

/**
 * @brief   Get config path.
 *
 * @return  Path of config node.
 */
const ::std::string& ConfigNode::path() const
{
    return m_path;
}

/**
 * @brief   Get required status.
 *
 * @return  True if the config node must exists, otherwise returns false.
 */
bool ConfigNode::required() const
{
    return m_required;
}

/**
 * @brief   Destructor.
 */
ConfigNode::~ConfigNode()
{
}


}
