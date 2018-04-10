#include "AccessUnitBlock.h"

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_H

class AccessUnit {

protected:

    uint32_t access_unit_id;        // zero-base unambiguous identifier, linearly increased by 1
    uint8_t num_blocks;             // number of blocks in the Access Unit
    uint16_t parameter_set_id;      // unique identifier of the Parameters Set in the Dataset Group
    uint8_t AU_type;                // identifies the type of Access Unit and the class of data carried
    uint32_t reads_count;           // number of genomic sequence reads encoded in the Access Unit
    uint8_t reference_id;           // unique identifier of the Raw Reference to be used to decode the Access Uni
    uint16_t sequence_id;           // unique identifier of the Reference Sequence to be used to decode the Access Unit
    uint32_t AU_start_position;     // indexing lower information
    uint32_t AU_end_position;       // indexing higher information

    std::vector<AccessUnitBlock> descriptors; // blocks containing descriptors

public:

    AccessUnit() {

    }

    ~AccessUnit() { }

    void setStartPosition(uint32_t start) {
        AU_start_position = start;
    }

    void setEndPosition(uint32_t endpos) {
        AU_end_position = endpos;
    }

    void insertPosdescriptor(int value) {
        descriptors[0].insertValue(value);
    }

    void insertRcompDescriptor(int value) {
        descriptors[1].insertValue(value);
    }

    void insertFlagsDescriptor(int value) {
        descriptors[2].insertValue(value);
    }

    void getPosDescriptorValues() {
        descriptors[0].write();
    }

    void getRcompDescriptorValues() {
        descriptors[1].write();
    }

    void getFlagsDescriptorValues() {
        descriptors[2].write();
    }

    void updateReads() {
        ++reads_count;
    }

    void write() {
        std::cout << "Type : " << unsigned(AU_type) << " ";
        std::cout << "ID = " << access_unit_id << " " << AU_start_position << " " << AU_end_position << " number of reads = " << reads_count << std::endl;
    }

    uint32_t getReadsCount() {
        return reads_count;
    }
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_H
