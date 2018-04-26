#include "AccessUnit_U.h"

AccessUnit_U::AccessUnit_U(uint32_t id) {
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

AccessUnit_U::~AccessUnit_U() {}

void AccessUnit_U::insertRlenDescriptor(uint8_t value) {
    descriptors[0].insertValue(std::to_string(value));
}

