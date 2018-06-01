#include "AccessUnit.h"

/**
 * @file AccessUnit_I.h
 * @Author Omair Iqbal
 * @date 04/2018
 * @class AccessUnit_M
 * @brief Access unit of type M: reads containing at least one substitution, and optionally unknown bases,
 *        but no insertions, no deletions and no clipped bases
 * @contact Omair95@protonmail.com
*/

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_M_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_M_H

class AccessUnit_M : public AccessUnit {

private:
    uint16_t mm_threshold;  /// number of mismatches in a read
    uint16_t mm_count;      /// represents the number reads encoded in the Access Unit which are below the specified threshold

public:
    /** @brief Constructor method
    * @param id identifier of the access unit of type M (3)
    * @return void
    * */
    AccessUnit_M(uint32_t id);

    /** @brief Destructor method
    * @param void
    * @return void
    * */
    ~AccessUnit_M();
};

#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_M_H