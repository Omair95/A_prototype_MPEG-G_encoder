#include <seqan/bam_io.h>
using namespace seqan;

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNITBLOCK_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNITBLOCK_H

class AccessUnitBlock {

private:
    uint8_t descriptor_id;                 // identifier of the type of encoded genomic descriptor carried by this block
    uint8_t reserved;                      // bits used to preserve byte alignment
    uint8_t padding;                       // number of padding bits added at the end of the encoded payload
    uint32_t block_size;                   // size in bytes of the payload
    std::vector<std::string> descriptor;   // the values of the descriptor

public:

    AccessUnitBlock(uint8_t descriptorId);

    ~AccessUnitBlock();

    void insertValue(std::string value);

    std::vector<std::string> getPayload();

    uint32_t getPayloadSize();

    void write();
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNITBLOCK_H
