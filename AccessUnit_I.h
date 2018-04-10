#include "AccessUnit.h"

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_I_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_I_H

class AccessUnit_I : public AccessUnit {

public:
    AccessUnit_I(uint32_t id) {
        access_unit_id = id;
        num_blocks = 8;
        parameter_set_id = 0;
        AU_type = 4;
        reads_count = 0;
        reference_id = 0;
        sequence_id = 0;
        AU_start_position = 0;
        AU_end_position = 0;

        descriptors.push_back(AccessUnitBlock(0)); // pos descriptor
        descriptors.push_back(AccessUnitBlock(1)); // rcomp descriptor
        descriptors.push_back(AccessUnitBlock(2)); // flags descriptor
        descriptors.push_back(AccessUnitBlock(3)); // mmpos descriptor
        descriptors.push_back(AccessUnitBlock(4)); // mmtype descriptor
        descriptors.push_back(AccessUnitBlock(7)); // rlen descriptor
        descriptors.push_back(AccessUnitBlock(8)); // pair descriptor
    }

    ~AccessUnit_I() { }

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

    void insertMmtypeDescriptor(int value) {
        descriptors[4].insertValue(value);
    }

    void insertRlenDescriptor(int value) {
        descriptors[5].insertValue(value);
    }

    void insertPairDescriptor(int value) {
        descriptors[6].insertValue(value);
    }
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_I_H
