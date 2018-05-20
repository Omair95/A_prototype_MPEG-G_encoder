#include "AccessUnit.h"

/*! \file AccessUnit_P.h */

/**
 * \class AccessUnit_P
 * \brief Access unit of type P: reads perfectly matching to the reference sequence.
 *
 * \author Omair Iqbal
 *
 * \date 04/2018
 *
 * Contact: Omair95@protonmail.com
 * */

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_P_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_P_H

class AccessUnit_P : public AccessUnit {

public:
    /** \brief Constructor method
    * \param id identifier of the access unit of type P (1)
    * \return void
    * */
    explicit AccessUnit_P(uint32_t id);

    /** \brief Destructor method
    * \param void
    * \return void
    * */
    ~AccessUnit_P();
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_P_H
