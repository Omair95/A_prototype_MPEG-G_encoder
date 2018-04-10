#include "AccessUnit.h"

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_U_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_U_H

class AccessUnit_U : public AccessUnit {

public:
    AccessUnit_U(uint32_t id) {
        access_unit_id = id;
        num_blocks = 1;
        parameter_set_id = 0;
        AU_type = 6;
        reads_count = 0;
        reference_id = 0;
        sequence_id = 0;
        AU_start_position = 0;
        AU_end_position = 0;

        descriptors.push_back(AccessUnitBlock(7)); // rlen descriptor
    }

    ~AccessUnit_U() {}

    void insertRlenDescriptor(int value) {
        descriptors[0].insertValue(value);
    }
};

#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_U_H
