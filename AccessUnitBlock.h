#include <seqan/bam_io.h>
using namespace seqan;

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_BLOCK_H
#define A_PROTOTYPE_MPEG_G_ENCODER_BLOCK_H

class AccessUnitBlock {

private:
    uint8_t descriptor_id;  // identifier of the type of encoded genomic descriptor carried by this block
    uint8_t reserved;       // bits used to preserve byte alignment
    uint8_t padding;        // number of padding bits added at the end of the encoded payload
    uint32_t block_size;    // size in bytes of the payload

    std::vector<int> descriptor;

public:

    AccessUnitBlock(uint8_t descriptorId) {
        descriptor_id = descriptorId;
        reserved = 0;
        padding = 0;
        block_size = 0;
        std::vector<int> descriptor(0);
    }

    ~AccessUnitBlock() {

    }

    void insertValue(int value) {
        descriptor.push_back(value);
        block_size += sizeof(int);
    }

    std::vector<int> getPayload() {
        return descriptor;
    }

    uint32_t getPayloadSize() {
        return block_size;
    }

    void write() {
        for (int i = 0; i < descriptor.size(); ++i) {
            std::cout << descriptor[i] << std::endl;
        }
    }
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_BLOCK_H
