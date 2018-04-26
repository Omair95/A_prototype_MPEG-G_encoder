#include "AccessUnit_HM.h"

AccessUnit_HM::AccessUnit_HM(uint32_t id) {
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

AccessUnit_HM::~AccessUnit_HM() {}

void AccessUnit_HM::insertRlenDescriptor(uint8_t value) {
    descriptors[3].insertValue(std::to_string(value));
}

void AccessUnit_HM::insertPairDescriptor(std::string value) {
    descriptors[4].insertValue(value);
}

