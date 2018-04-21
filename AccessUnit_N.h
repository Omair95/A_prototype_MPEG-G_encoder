#include "AccessUnit.h"

/*! \file AccessUnit_N.h */

/**
 * \class AccessUnit_N
 * \brief Access unit of type N: reads containing mismatches which are unknown bases only.
 *
 * \author $Author: Omair Iqbal $
 *
 * \date $Date: 04/2018 $
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
    * \param id the id of the access unit of type N (2)
    * \return void
    * */
    AccessUnit_N(uint32_t id);

    /** \brief Destructor method
    * \param void
    * \return void
    * */
    ~AccessUnit_N();

    /** \brief Inserts the mmpos descriptor value inside the corresponding block in the payload
    * \param value an string representing the value in hexadecimal of the mmpos descriptor
    * \return void
    * */
    void insertMmposDescriptor(std::string value);

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

#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_N_H
