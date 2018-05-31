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
    block_size += sizeof(int);
}

std::vector<std::pair<std::string, uint8_t > > AccessUnitBlock::getPayload() {
    return descriptor;
}

uint32_t AccessUnitBlock::getPayloadSize() {
    return block_size;
}

void AccessUnitBlock::write() {
    for (int i = 0; i < descriptor.size(); ++i) {
        std::cout << descriptor[i].first << std::endl;
    }
}