#include "AccessUnitBlock.h"

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_H

class AccessUnit {

protected:

    uint32_t access_unit_id;                    // zero-base unambiguous identifier, linearly increased by 1
    uint8_t num_blocks;                         // number of blocks in the Access Unit
    uint16_t parameter_set_id;                  // unique identifier of the Parameters Set in the Dataset Group
    uint8_t AU_type;                            // identifies the type of Access Unit and the class of data carried
    uint32_t reads_count;                       // number of genomic sequence reads encoded in the Access Unit
    uint8_t reference_id;                       // unique identifier of the Raw Reference to be used to decode the Access Uni
    uint16_t sequence_id;                       // unique identifier of the Reference Sequence to be used to decode the Access Unit
    uint32_t AU_start_position;                 // indexing lower information
    uint32_t AU_end_position;                   // indexing higher information
    std::vector<AccessUnitBlock> descriptors;   // blocks containing descriptors

public:

    AccessUnit();

    ~AccessUnit();

    void setStartPosition(uint32_t start);

    void setEndPosition(uint32_t endpos);

    void insertPosdescriptor(std::string value);

    void insertRcompDescriptor(std::string value);

    void insertFlagsDescriptor(std::string value);

    void getPosDescriptorValues();

    void getRcompDescriptorValues();

    void getFlagsDescriptorValues();

    void updateReads();

    void write();

    uint32_t getReadsCount();
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_H
