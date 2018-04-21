#include "AccessUnit.h"

/*! \file AccessUnit_U.h */

/**
 * \class AccessUnit_U
 * \brief Access unit of type U: Unmapped reads.
 *
 * \author $Author: Omair Iqbal $
 *
 * \date $Date: 04/2018 $
 *
 * Contact: Omair95@protonmail.com
 * */

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_U_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_U_H

class AccessUnit_U : public AccessUnit {

public:
    /** \brief Constructor method
    * \param id the id of the access unit of type M (3)
    * \return void
    * */
    AccessUnit_U(uint32_t id);

    /** \brief Destructor method
    * \param void
    * \return void
    * */
    ~AccessUnit_U();

    /** \brief Inserts the rlen descriptor value inside the corresponding block in the payload
    * \param value an string representing the value in hexadecimal of the rlen descriptor
    * \return void
    * */
    void insertRlenDescriptor(std::string value);
};

#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_U_H
