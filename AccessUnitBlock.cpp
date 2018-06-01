#include "AccessUnitBlock.h"

AccessUnitBlock::AccessUnitBlock(uint8_t descriptorId) {
    descriptor_id = descriptorId;
    reserved = 0;
    padding = 0;
    block_size = 0;
    std::vector<std::pair<std::string, uint8_t > > descriptor(0);
}

AccessUnitBlock::~AccessUnitBlock() { }

void AccessUnitBlock::insertValue(std::pair<std::string, uint8_t>& value) {
    descriptor.emplace_back(value);
    if (value.second == 8) block_size += sizeof(uint8_t);
    else if (value.second == 16) block_size += sizeof(uint16_t);
    else if (value.second == 32) block_size += sizeof(uint32_t);
    else if (value.second == 64) block_size += sizeof(uint16_t);
}

std::vector<std::pair<std::string, uint8_t > > AccessUnitBlock::getPayload() {
    return descriptor;
}

uint32_t AccessUnitBlock::getPayloadSize() {
    return block_size;
}