#include "AccessUnitBlock.h"

/*! \file AccessUnit.h */

/**
 * \class AccessUnit
 * \brief Main data structure to be used
 *
 * This class implements the access unit structure that contains header information
 * and a number of blocks as a payload.
 *
 * \author $Author: Omair Iqbal $
 *
 * \date $Date: 04/2018 $
 *
 * Contact: Omair95@protonmail.com
 * */

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_H

class AccessUnit {

protected:

    uint32_t access_unit_id;                    /// zero-base unambiguous identifier, linearly increased by 1
    uint8_t num_blocks;                         /// number of blocks in the Access Unit
    uint16_t parameter_set_id;                  /// unique identifier of the Parameters Set in the Dataset Group
    uint8_t AU_type;                            /// identifies the type of Access Unit and the class of data carried
    uint32_t reads_count;                       /// number of genomic sequence reads encoded in the Access Unit
    uint8_t reference_id;                       /// unique identifier of the Raw Reference to be used to decode the Access Uni
    uint16_t sequence_id;                       /// unique identifier of the Reference Sequence to be used to decode the Access Unit
    uint32_t AU_start_position;                 /// indexing lower information
    uint32_t AU_end_position;                   /// indexing higher information
    std::vector<AccessUnitBlock> descriptors;   /// blocks containing descriptors

public:

    /** \brief Constructor method
    * \param void
    * \return void
    * */
    AccessUnit();

    /** \brief Destructor method
    * \param void
    * \return void
    * */
    ~AccessUnit();

    /** \brief Sets the start position of the access unit
    * \param start an integer representing the start position
    * \return void
    * */
    void setStartPosition(uint32_t start);

    /** \brief Sets the end position of the access unit
    * \param endpos an integer representing the end position
    * \return void
    * */
    void setEndPosition(uint32_t endpos);

    /** \brief Inserts the pos descriptor value inside the corresponding block in the payload
    * \param value an string representing the value in hexadecimal of the pos descriptor
    * \return void
    * */
    void insertPosdescriptor(std::string value);

    /** \brief Inserts the rcomp descriptor value inside the corresponding block in the payload
    * \param value an string representing the value in hexadecimal of the rcomp descriptor
    * \return void
    * */
    void insertRcompDescriptor(std::string value);

    /** \brief Inserts the flags descriptor value inside the corresponding block in the payload
    * \param value an string representing the value in hexadecimal of the flags descriptor
    * \return void
    * */
    void insertFlagsDescriptor(std::string value);

    /** \brief Gets all the values of the pos descriptor in the access unit
    * \param void
    * \return void
    * */
    void getPosDescriptorValues();

    /** \brief Gets all the values of the rcomp descriptor in the access unit
    * \param void
    * \return void
    * */
    void getRcompDescriptorValues();

    /** \brief Gets all the value of the flags descriptor in the access unit
    * \param void
    * \return void
    * */
    void getFlagsDescriptorValues();

    /** \brief Updates the numbers of reads stored in the access unit
    * \param void
    * \return void
    * */
    void updateReads();

    /** \brief Gets the number of reads stored in the access unit
    * \param void
    * \return the number of reads in the access unit
    * */
    uint32_t getReadsCount();

    /** \brief Writes via command line the type of the access unit, the start and end
     * position and the number of reads stored in it.
    * \param void
    * \return void
    * */
    void write();
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_H
