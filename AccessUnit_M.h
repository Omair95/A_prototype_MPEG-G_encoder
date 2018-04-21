#include "AccessUnit.h"

/*! \file AccessUnit_M.h */

/**
 * \class AccessUnit_M
 * \brief Access unit of type M: reads containing at least one substitution, and optionally unknown bases,
 *  but no insertions, no deletions and no clipped bases.
 *
 * \author $Author: Omair Iqbal $
 *
 * \date $Date: 04/2018 $
 *
 * Contact: Omair95@protonmail.com
 * */

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_M_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_M_H

class AccessUnit_M : public AccessUnit {

public:
    /** \brief Constructor method
    * \param id the id of the access unit of type M (3)
    * \return void
    * */
    AccessUnit_M(uint32_t id);

    /** \brief Destructor method
    * \param void
    * \return void
    * */
    ~AccessUnit_M();

    /** \brief Inserts the mmpos descriptor value inside the corresponding block in the payload
    * \param value an string representing the value in hexadecimal of the mmpos descriptor
    * \return void
    * */
    void insertMmposDescriptor(std::string value);

    /** \brief Inserts the mmtype descriptor value inside the corresponding block in the payload
    * \param value an string representing the value in hexadecimal of the mmtype descriptor
    * \return void
    * */
    void insertMmtypeDescriptor(std::string value);

    /** \brief Inserts the rlen descriptor value inside the corresponding block in the payload
    * \param value an string representing the value in hexadecimal of the rlen descriptor
    * \return void
    * */
    void insertRlenDescriptor(std::string value);

    /** \brief Inserts the pair descriptor value inside the corresponding block in the payload
    * \param value an string representing the value in hexadecimal of the pair descriptor
    * \return void
    * */
    void insertPairDescriptor(std::string value);
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_M_H
