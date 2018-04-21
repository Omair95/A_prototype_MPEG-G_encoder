#include "AccessUnit.h"

/*! \file AccessUnit_I.h */

/**
 * \class AccessUnit_I
 * \brief Access unit of type I: reads containing at least one insertion, deletion or clipped base, and
 *  optionally unknown bases or substitutions.
 *
 * \author $Author: Omair Iqbal $
 *
 * \date $Date: 04/2018 $
 *
 * Contact: Omair95@protonmail.com
 * */

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_I_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_I_H

class AccessUnit_I : public AccessUnit {

public:
    /** \brief Constructor method
    * \param id the id of the access unit of type M (3)
    * \return void
    * */
    AccessUnit_I(uint32_t id);

    /** \brief Destructor method
    * \param void
    * \return void
    * */
    ~AccessUnit_I();

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


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_I_H
