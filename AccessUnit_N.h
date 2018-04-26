#include "AccessUnit.h"

/*! \file AccessUnit_N.h */

/**
 * \class AccessUnit_N
 * \brief Access unit of type N: reads containing mismatches which are unknown bases only.
 *
 * \author Omair Iqbal
 *
 * \date 04/2018
 *
 * Contact: Omair95@protonmail.com
 * */

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_N_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_N_H

class AccessUnit_N : public AccessUnit {

private:
    uint16_t mm_threshold;
    uint16_t mm_count;

public:
    /** \brief Constructor method
    * \param id identifier of the access unit of type N (2)
    * \return void
    * */
    AccessUnit_N(uint32_t id);

    /** \brief Destructor method
    * \param void
    * \return void
    * */
    ~AccessUnit_N();

    /** \brief Inserts the mmpos descriptor value inside the corresponding block in the access unit's payload
    * \param value string representing the value of the mmpos descriptor
    * \return void
    * */
    void insertMmposDescriptor(uint16_t value);

    /** \brief Inserts the rlen descriptor value inside the corresponding block in the access unit's payload
    * \param value string representing the value of the rlen descriptor
    * \return void
    * */
    void insertRlenDescriptor(uint8_t value);

    /** \brief Inserts the pair descriptor value inside the corresponding block in the access unit's payload
    * \param value string representing the value of the pair descriptor
    * \return void
    * */
    void insertPairDescriptor(std::string value);

};

#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_N_H
