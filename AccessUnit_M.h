#include "AccessUnit.h"

/*! \file AccessUnit_M.h */

/**
 * \class AccessUnit_M
 * \brief Access unit of type M: reads containing at least one substitution, and optionally unknown bases,
 *  but no insertions, no deletions and no clipped bases.
 *
 * \author Omair Iqbal
 *
 * \date 04/2018
 *
 * Contact: Omair95@protonmail.com
 * */

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_M_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_M_H

class AccessUnit_M : public AccessUnit {

private:
    uint16_t mm_threshold;  /// number of mismatches in a read
    uint16_t mm_count;      /// represents the number reads encoded in the Access Unit which are below the specified threshold

public:
    /** \brief Constructor method
    * \param id identifier of the access unit of type M (3)
    * \return void
    * */
    AccessUnit_M(uint32_t id);

    /** \brief Destructor method
    * \param void
    * \return void
    * */
    ~AccessUnit_M();

    /** \brief Inserts the mmpos descriptor value inside the corresponding block in the access unit's payload
    * \param value string representing the value in hexadecimal of the mmpos descriptor
    * \return void
    * */
    void insertMmposDescriptor(uint16_t value);

    /** \brief Inserts the mmtype descriptor value inside the corresponding block in the access unit's payload
    * \param value string representing the value in hexadecimal of the mmtype descriptor
    * \return void
    * */
    void insertMmtypeDescriptor(uint8_t value);

    /** \brief Inserts the rlen descriptor value inside the corresponding block in the access unit's payload
    * \param value string representing the value in hexadecimal of the rlen descriptor
    * \return void
    * */
    void insertRlenDescriptor(uint8_t value);

    /** \brief Inserts the pair descriptor value inside the corresponding block in the access unit's payload
    * \param value string representing the value in hexadecimal of the pair descriptor
    * \return void
    * */
    void insertPairDescriptor(std::string value);
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_M_H
