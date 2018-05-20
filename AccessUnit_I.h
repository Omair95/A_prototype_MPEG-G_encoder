#include "AccessUnit.h"

/*! \file AccessUnit_I.h */

/**
 * \class AccessUnit_I
 * \brief Access unit of type I: reads containing at least one insertion, deletion or clipped base, and
 *  optionally unknown bases or substitutions.
 *
 * \author Omair Iqbal
 *
 * \date 04/2018
 *
 * Contact: Omair95@protonmail.com
 * */

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_I_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_I_H

class AccessUnit_I : public AccessUnit {

public:
    /** \brief Constructor method
    * \param id identifier of the access unit of type I (4)
    * \return void
    * */
    explicit AccessUnit_I(uint32_t id);

    /** \brief Destructor method
    * \param void
    * \return void
    * */
    ~AccessUnit_I();

    /** \brief Inserts the mmpos descriptor value inside the corresponding block in the access unit's payload
    * \param value string representing the value of the mmpos descriptor
    * \return void
    * */
    void insertMmposDescriptor(uint16_t value);

    /** \brief Inserts the mmtype descriptor value inside the corresponding block in the access unit's payload
    * \param value string representing the value of the mmtype descriptor
    * \return void
    * */
    void insertMmtypeDescriptor(uint8_t value);


    /** \brief Inserts the sclips descriptor value inside the corresponding block in the access unit's payload
    * \param value string representing the value of the mmtype descriptor
    * \return void
    * */
    void insertSclipsDescriptor();
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_I_H
