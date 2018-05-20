#include "AccessUnit_M.h"

AccessUnit_M::AccessUnit_M(uint32_t id) {
    access_unit_id = id;
    num_blocks = 7;
    parameter_set_id = 0;
    AU_type = 3;
    reads_count = 0;
    reference_id = 0;
    sequence_id = 0;
    AU_start_position = 0;
    AU_end_position = 0;
    mm_threshold = 0;
    mm_count = 0;

    descriptors.emplace_back(0); // pos descriptor
    descriptors.emplace_back(1); // rcomp descriptor
    descriptors.emplace_back(2); // flags descriptor
    descriptors.emplace_back(7); // rlen descriptor
    descriptors.emplace_back(8); // pair descriptor
    descriptors.emplace_back(3); // mmpos descriptor
    descriptors.emplace_back(4); // mmtype descriptor
}

AccessUnit_M::~AccessUnit_M() = default;

void AccessUnit_M::insertMmposDescriptor(uint16_t value) {
    descriptors[5].insertValue(std::to_string(value));
    ++mm_threshold;
}

void AccessUnit_M::insertMmtypeDescriptor(uint8_t value) {
    descriptors[6].insertValue(std::to_string(value));
}