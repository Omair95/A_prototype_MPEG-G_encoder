#include "AccessUnit.h"

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_HM_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_HM_H

class AccessUnit_HM : public AccessUnit {

public:
    AccessUnit_HM(uint32_t id) {
        access_unit_id = id;
        num_blocks = 5;
        parameter_set_id = 0;
        AU_type = 5;
        reads_count = 0;
        reference_id = 0;
        sequence_id = 0;
        AU_start_position = 0;
        AU_end_position = 0;

        descriptors.push_back(AccessUnitBlock(0)); // pos descriptor
        descriptors.push_back(AccessUnitBlock(1)); // rcomp descriptor
        descriptors.push_back(AccessUnitBlock(2)); // flags descriptor
        descriptors.push_back(AccessUnitBlock(7)); // rlen descriptor
        descriptors.push_back(AccessUnitBlock(8)); // pair descriptor
    }

    ~AccessUnit_HM() {}

    void insertPosdescriptor(std::string value) {
        descriptors[0].insertValue(value);
    }

    void insertRcompDescriptor(std::string value) {
        descriptors[1].insertValue(value);
    }

    void insertFlagsDescriptor(std::string value) {
        descriptors[2].insertValue(value);
    }

    void insertRlenDescriptor(std::string value) {
        descriptors[3].insertValue(value);
    }

    void insertPairDescriptor(std::string value) {
        descriptors[4].insertValue(value);
    }
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_HM_H
