#include <seqan/bam_io.h>
using namespace seqan;

/*! \file AccessUnitBlock.h */

/**
 * \class AccessUnitBlock
 * \brief Block data structure of the access unit
 *
 * This class implements the block data structure that goes inside
 * the access unit. It has some header information and payload.
 *
 * \author Omair Iqbal
 *
 * \date 04/2018
 *
 * Contact: Omair95@protonmail.com
 * */

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNITBLOCK_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNITBLOCK_H

class AccessUnitBlock {

private:
    uint8_t descriptor_id;                 /// identifier of the type of encoded genomic descriptor carried by this block
    uint8_t reserved;                      /// bits used to preserve byte alignment
    uint8_t padding;                       /// number of padding bits added at the end of the encoded payload
    uint32_t block_size;                   /// size in bytes of the payload
    std::vector<std::string> descriptor;   /// the values of the descriptor

public:

    /** \brief Constructor method
     *  \param descriptorId identifier of the descriptor to be stored
     *  The result is a block with an id and an empty payload
     * */
    AccessUnitBlock(uint8_t descriptorId);

    /** \brief Destructor method
     * */
    ~AccessUnitBlock();

    /** \brief Inserts a value inside the block
     * \param value value to be inserted in the block
     * \return void
     * */
    void insertValue(std::string value);

    /** \brief Gets the payload of the block, which contains
     * all the values of the descriptor
     * \param void
     * \return vector containing all the values of the block descriptor
     * */
    std::vector<std::string> getPayload();

    /** \brief Gets the size of the payload of the block
     * \param void
     * \return an unsigned 32-bit integer representing the size of the payload
     * */
    uint32_t getPayloadSize();

    /** \brief Writes via to the commnad line the content of the payload
     * \param void
     * \return void
     * */
    void write();
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNITBLOCK_H
