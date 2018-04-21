#include "AccessUnit.h"

/*! \file AccessUnit_P.h */

/**
 * \class AccessUnit_P
 * \brief Access unit of type P: reads perfectly matching to the reference sequence.
 *
 * \author $Author: Omair Iqbal $
 *
 * \date $Date: 04/2018 $
 *
 * Contact: Omair95@protonmail.com
 * */

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_P_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_P_H

class AccessUnit_P : public AccessUnit {

public:
    /** \brief Constructor method
    * \param id the id of the access unit of type P (1)
    * \return void
    * */
    AccessUnit_P(uint32_t id);

    /** \brief Destructor method
    * \param void
    * \return void
    * */
    ~AccessUnit_P();

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


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_P_H
