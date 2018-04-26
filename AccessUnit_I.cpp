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

    descriptors.push_back(AccessUnitBlock(0)); // pos descriptor
    descriptors.push_back(AccessUnitBlock(1)); // rcomp descriptor
    descriptors.push_back(AccessUnitBlock(2)); // flags descriptor
    descriptors.push_back(AccessUnitBlock(3)); // mmpos descriptor
    descriptors.push_back(AccessUnitBlock(4)); // mmtype descriptor
    descriptors.push_back(AccessUnitBlock(7)); // rlen descriptor
    descriptors.push_back(AccessUnitBlock(8)); // pair descriptor
}

AccessUnit_I::~AccessUnit_I() { }

void AccessUnit_I::insertMmposDescriptor(uint16_t value) {
    descriptors[3].insertValue(std::to_string(value));
}

void AccessUnit_I::insertMmtypeDescriptor(uint8_t value) {
    descriptors[4].insertValue(std::to_string(value));
}

void AccessUnit_I::insertRlenDescriptor(uint8_t value) {
    descriptors[5].insertValue(std::to_string(value));
}

void AccessUnit_I::insertPairDescriptor(std::string value) {
    descriptors[6].insertValue(value);
}