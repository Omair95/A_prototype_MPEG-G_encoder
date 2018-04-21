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

    descriptors.push_back(AccessUnitBlock(0)); // pos descriptor
    descriptors.push_back(AccessUnitBlock(1)); // rcomp descriptor
    descriptors.push_back(AccessUnitBlock(2)); // flags descriptor
    descriptors.push_back(AccessUnitBlock(3)); // mmpos descriptor
    descriptors.push_back(AccessUnitBlock(4)); // mmtype descriptor
    descriptors.push_back(AccessUnitBlock(7)); // rlen descriptor
    descriptors.push_back(AccessUnitBlock(8)); // pair descriptor
}

AccessUnit_M::~AccessUnit_M() { }

void AccessUnit_M::insertMmposDescriptor(std::string value) {
    descriptors[3].insertValue(value);
}

void AccessUnit_M::insertMmtypeDescriptor(std::string value) {
    descriptors[4].insertValue(value);
}

void AccessUnit_M::insertRlenDescriptor(std::string value) {
    descriptors[5].insertValue(value);
}

void AccessUnit_M::insertPairDescriptor(std::string value) {
    descriptors[6].insertValue(value);
}