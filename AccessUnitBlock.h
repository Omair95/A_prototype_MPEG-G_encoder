#include <seqan/bam_io.h>
using namespace seqan;

/**
 * @file AccessUnitBlock.h
 * @Author Omair Iqbal
 * @date 04/2018
 * @class AccessUnitBlock
 * @brief block data structure that goes inside the access unit. 
 *        It has some header information and payload containing the descriptor values
 * @contact Omair95@protonmail.com
*/

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNITBLOCK_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNITBLOCK_H

class AccessUnitBlock {

private:
    uint8_t descriptor_id;                 /// identifier of the type of encoded genomic descriptor carried by this block
    uint8_t reserved;                      /// bits used to preserve byte alignment
    uint8_t padding;                       /// number of padding bits added at the end of the encoded payload
    uint32_t block_size;                   /// size in bytes of the payload
    std::vector<std::pair<std::string, uint8_t> > descriptor;   /// the values of the descriptor

public:

    /** @brief Constructor method
     *  @param descriptorId identifier of the descriptor to be stored
     *  The result is a block with an id and an empty payload
     * */
    AccessUnitBlock(uint8_t descriptorId);

    /** @brief Destructor method
     * */
    ~AccessUnitBlock();

    /** @brief Inserts a value inside the block
     * @param value value to be inserted in the block
     * @return void
     * */
    void insertValue(std::pair<std::string, uint8_t>& value);

    /** @brief Gets the payload of the block, which contains
     * all the values of the descriptor
     * @param void
     * @return vector containing all the values of the block descriptor
     * */
    std::vector<std::pair<std::string, uint8_t > > getPayload();

    /** @brief Gets the size of the payload of the block
     * @param void
     * @return an unsigned 32-bit integer representing the size of the payload
     * */
    uint32_t getPayloadSize();
};

#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNITBLOCK_H