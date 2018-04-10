#include "AccessUnit.h"

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_N_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_N_H

class AccessUnit_N : public AccessUnit {

private:
    uint16_t mm_threshold;
    uint16_t mm_count;

public:
    AccessUnit_N(uint32_t id) {
        access_unit_id = id;
        num_blocks = 6;
        parameter_set_id = 0;
        AU_type = 2;
        reads_count = 0;
        reference_id = 0;
        sequence_id = 0;
        AU_start_position = 0;
        AU_end_position = 0;

        descriptors.push_back(AccessUnitBlock(0)); // pos descriptor
        descriptors.push_back(AccessUnitBlock(1)); // rcomp descriptor
        descriptors.push_back(AccessUnitBlock(2)); // flags descriptor
        descriptors.push_back(AccessUnitBlock(3)); // mmpos descriptor
        descriptors.push_back(AccessUnitBlock(7)); // rlen descriptor
        descriptors.push_back(AccessUnitBlock(8)); // pair descriptor
    }

    ~AccessUnit_N() { }

    void insertPosdescriptor(int value) {
        descriptors[0].insertValue(value);
    }

    void insertRcompDescriptor(int value) {
        descriptors[1].insertValue(value);
    }

    void insertFlagsDescriptor(int value) {
        descriptors[2].insertValue(value);
    }

    void insertMmposDescriptor(int value) {
        descriptors[3].insertValue(value);
    }

    void insertRlenDescriptor(int value) {
        descriptors[4].insertValue(value);
    }

    void insertPairDescriptor(int value) {
        descriptors[5].insertValue(value);
    }

};

#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_N_H
