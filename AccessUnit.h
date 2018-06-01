#include "AccessUnitBlock.h"

/**
 * @file AccessUnit.h
 * @Author Omair Iqbal
 * @date 04/2018
 * @class AccessUnit
 * @brief Data structure that contains a coded representation of genomic information
 *        or related metadata to facilitate the bit stream access and manipulation
 * @contact Omair95@protonmail.com
*/

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_H

class AccessUnit {

protected:

    uint32_t access_unit_id;                    /// zero-base unambiguous identifier, linearly increased by 1
    uint8_t num_blocks;                         /// number of blocks in the access unit
    uint16_t parameter_set_id;                  /// unique identifier of the Parameters Set in the Dataset Group
    uint8_t AU_type;                            /// identifies the type of Access Unit and the class of data carried
    uint32_t reads_count;                       /// number of genomic sequence reads encoded in the Access Unit
    uint8_t reference_id;                       /// unique identifier of the Raw Reference to be used to decode the Access Unit
    uint16_t sequence_id;                       /// unique identifier of the Reference Sequence to be used to decode the Access Unit
    uint32_t AU_start_position;                 /// indexing lower information
    uint32_t AU_end_position;                   /// indexing higher information
    std::vector<AccessUnitBlock> descriptors;   /// blocks containing descriptors

public:

    /** @brief Constructor method
    * @param void
    * @return void
    * */
    AccessUnit();

    /** @brief Destructor method
    * @param void
    * @return void
    * */
    virtual ~AccessUnit();

    /** @brief Get the type of the access unit
     * @param void
     * @return unsigned integer representing the type of access unit
     * */
    uint8_t getType();

    /** @brief Set start position
    * @param start unsigned integer representing the start position of the access unit in the reference sequence
    * @return void
    * */
    void setStartPosition(uint32_t start);

    /** @brief Set end position
    * @param endpos unsigned integer representing the end position of the access unit in the reference sequence
    * @return void
    * */
    void setEndPosition(uint32_t endpos);

    /** @brief Set sequence id
    * @param id identifier of the sequence id
    * @return void
    * */
    void setSequenceID(uint8_t id);

    /** @brief Insert pos descriptor value inside the corresponding block in the access unit's payload
    * @param value unsigned integer representing the value of the pos descriptor
    * @return void
    * */
    void insertPosdescriptor(uint32_t value);

    /** @brief Insert rcomp descriptor value inside the corresponding block in the access unit's payload
    * @param value unsigned integer representing the value of the rcomp descriptor
    * @return void
    * */
    void insertRcompDescriptor(uint8_t value);

    /** @brief Insert flags descriptor value inside the corresponding block in the access unit's payload
    * @param value unsigned integer representing the value of the flags descriptor
    * @return void
    * */
    void insertFlagsDescriptor(uint8_t value);

    /** @brief Insert rlen descriptor value inside the corresponding block in the access unit's payload
    * @param value unsigned int representing the value of the rlen descriptor
    * @return void
    * */
    void insertRlenDescriptor(uint8_t value);

    /** @brief Inserts pair descriptor value inside the corresponding block in the access unit's payload
    * @param value string representing the value of the pair descriptor
     * @param size uinsigned integer representing the size of the value of the pair descriptor
    * @return void
    * */
    void insertPairDescriptor(std::string value, uint8_t size);

    /** @brief Insert mmpos descriptor value inside the corresponding block in the access unit's payload
    * @param value string representing the value of the mmpos descriptor
    * @return void
    * */
    void insertMmposDescriptor(std::string value);

    /** @brief Insert mmtype descriptor value inside the corresponding block in the access unit's payload
    * @param value string representing the value of the mmtype descriptor
    * @return void
    * */
    void insertMmtypeDescriptor(std::string value);

    /** @brief Inserts soft clips descriptor value inside the corresponding block in the access unit's payload
    * @param value string representing the value of the sclips descriptor
     * @param size uinsigned integer representing the size of the value of the pair descriptor
    * @return void
    * */
    void insertSclipsDescriptor(std::string value, uint8_t size);

    /** @brief Gets all the values of the pos descriptor in the access unit
    * @param void
    * @return void
    * */
    std::vector<std::pair<std::string, uint8_t > > getPosDescriptorValues();

    /** @brief Gets all the values of the rcomp descriptor in the access unit
    * @param void
    * @return void
    * */
    std::vector<std::pair<std::string, uint8_t > > getRcompDescriptorValues();

    /** @brief Gets all the value of the flags descriptor in the access unit
    * @param void
    * @return void
    * */
    std::vector<std::pair<std::string, uint8_t > > getFlagsDescriptorValues();

    /** @brief Get all the values of the rlen descriptor in the access unit
    * @param void
    * @return void
    * */
    std::vector<std::pair<std::string, uint8_t > > getRLenDescriptorValues();

    /** @brief Gets all the values of the pair descriptor in the access unit
    * @param void
    * @return void
    * */
    std::vector<std::pair<std::string, uint8_t > > getPairDescriptorValues();

    /** @brief Gets all the values of the mmpos descriptor in the access unit
    * @param void
    * @return void
    * */
    std::vector<std::pair<std::string, uint8_t > > getMmposDescriptorValues();

    /** @brief Gets all the values of the mmtype descriptor in the access unit
    * @param void
    * @return void
    * */
    std::vector<std::pair<std::string, uint8_t > > getMmtypeDescriptorValues();

    /** @brief Updates the numbers of reads stored in the access unit
    * @param void
    * @return void
    * */
    void updateReads();

    /** @brief Get the number of reads stored in the access unit
    * @param void
    * @return the number of reads in the access unit
    * */
    uint32_t getReadsCount();

    /** @brief Writes via command line the type of the access unit, the start and end
     * position and the number of reads stored in it.
    * @param void
    * @return void
    * */
    void write();
};

#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_H