#include "AccessUnit.h"

/**
 * @file AccessUnit_N.h
 * @Author Omair Iqbal
 * @date 04/2018
 * @class AccessUnit_N
 * @brief Access unit of type N: reads containing mismatches which are unknown bases only
 * @contact Omair95@protonmail.com
*/

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_N_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_N_H

class AccessUnit_N : public AccessUnit {

private:
    uint16_t mm_threshold;
    uint16_t mm_count;

public:

    /** @brief Constructor method
    * @param id identifier of the access unit of type N (2)
    * @return void
    * */
    AccessUnit_N(uint32_t id);

    /** @brief Destructor method
    * @param void
    * @return void
    * */
    ~AccessUnit_N();

    /** @brief Insert a single mmpos descriptor value inside the corresponding block in the access unit's payload
    * @param value unsigned integer representing the value of the mmpos descriptor
    * @return void
    * */
    void insertMmposDescriptor(uint16_t value);

};

#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_N_H