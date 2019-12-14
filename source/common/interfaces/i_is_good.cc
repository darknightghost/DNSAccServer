#include <common/interfaces/i_is_good.h>

namespace dnsAccServer {

/**
 * @brief   Constructor, initialize and set m_good to false.
 */
IIsGood::IIsGood() :
    m_good(false)
{
}

/**
 * @brief   Get initialize result.
 *
 * @return  Initialize result.
 */
bool        IIsGood::good()
{
    return m_good;
}

/**
 * @brief   Destructor.
 */
IIsGood::~IIsGood()
{
}

/**
 * @brief   Set initialize status to true.
 */
void        IIsGood::setGood()
{
    m_good = true;
}

}
