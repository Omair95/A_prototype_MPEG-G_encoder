#include "AccessUnit_I.h"

AccessUnit_I::AccessUnit_I(uint32_t id) {
    access_unit_id = id;
    num_blocks = 8;
    parameter_set_id = 0;
    AU_type = 4;
    reads_count = 0;
    reference_id = 0;
    sequence_id = 0;
    AU_start_position = 0;
    AU_end_position = 0;

    descriptors.emplace_back(0); // pos descriptor
    descriptors.emplace_back(1); // rcomp descriptor
    descriptors.emplace_back(2); // flags descriptor
    descriptors.emplace_back(7); // rlen descriptor
    descriptors.emplace_back(8); // pair descriptor
    descriptors.emplace_back(3); // mmpos descriptor
    descriptors.emplace_back(4); // mmtype descriptor
    descriptors.emplace_back(5); // sclips descriptor
}

AccessUnit_I::~AccessUnit_I() = default;

void AccessUnit_I::insertMmposDescriptor(uint16_t value) {
    descriptors[3].insertValue(std::to_string(value));
}

void AccessUnit_I::insertMmtypeDescriptor(uint8_t value) {
    descriptors[4].insertValue(std::to_string(value));
}

void AccessUnit_I::insertSclipsDescriptor() {

}