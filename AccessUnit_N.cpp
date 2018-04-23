#include "AccessUnit_N.h"

AccessUnit_N::AccessUnit_N(uint32_t id) {
    access_unit_id = id;
    num_blocks = 6;
    parameter_set_id = 0;
    AU_type = 2;
    reads_count = 0;
    reference_id = 0;
    sequence_id = 0;
    AU_start_position = 0;
    AU_end_position = 0;
    mm_threshold = 0;
    mm_count = 0;

    descriptors.push_back(AccessUnitBlock(0)); // pos descriptor
    descriptors.push_back(AccessUnitBlock(1)); // rcomp descriptor
    descriptors.push_back(AccessUnitBlock(2)); // flags descriptor
    descriptors.push_back(AccessUnitBlock(3)); // mmpos descriptor
    descriptors.push_back(AccessUnitBlock(7)); // rlen descriptor
    descriptors.push_back(AccessUnitBlock(8)); // pair descriptor
}

AccessUnit_N::~AccessUnit_N() { }

void AccessUnit_N::insertMmposDescriptor(std::string value) {
    descriptors[3].insertValue(value);
    ++mm_threshold;
}

void AccessUnit_N::insertRlenDescriptor(std::string value) {
    descriptors[4].insertValue(value);
}

void AccessUnit_N::insertPairDescriptor(std::string value) {
    descriptors[5].insertValue(value);
}

