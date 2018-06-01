#include "AccessUnit.h"

AccessUnit::AccessUnit() = default;

AccessUnit::~AccessUnit() = default;

uint8_t AccessUnit::getType() {
    return AU_type;
}

void AccessUnit::setStartPosition(uint32_t start) {
    AU_start_position = start;
}

void AccessUnit::setEndPosition(uint32_t endpos) {
    AU_end_position = endpos;
}

void AccessUnit::setSequenceID(uint8_t id) {
    sequence_id = id;
}

void AccessUnit::insertPosdescriptor(uint32_t value) {
    std::pair<std::string, uint8_t> valor(std::to_string(value), 32);
    descriptors[0].insertValue(valor);
}

void AccessUnit::insertRcompDescriptor(uint8_t value) {
    std::pair<std::string, uint8_t> valor(std::to_string(value), 8);
    descriptors[1].insertValue(valor);
}

void AccessUnit::insertFlagsDescriptor(uint8_t value) {
    std::pair<std::string, uint8_t> valor(std::to_string(value), 8);
    descriptors[2].insertValue(valor);
}

void AccessUnit::insertRlenDescriptor(uint8_t value) {
    std::pair<std::string, uint8_t> valor(std::to_string(value), 8);
    descriptors[3].insertValue(valor);
}

void AccessUnit::insertPairDescriptor(std::string value, uint8_t size) {
    std::pair<std::string, uint8_t> valor(value, size);
    descriptors[4].insertValue(valor);
}

void AccessUnit::insertMmposDescriptor(std::string value) {
    std::pair<std::string, uint8_t> valor(value, 16);
    descriptors[5].insertValue(valor);
}

void AccessUnit::insertMmtypeDescriptor(std::string value) {
    std::pair<std::string, uint8_t> valor(value, 8);
    descriptors[6].insertValue(valor);
}

void AccessUnit::insertSclipsDescriptor(std::string value, uint8_t size) {
    std::pair<std::string, uint8_t> valor(value, size);
    descriptors[7].insertValue(valor);
}

std::vector<std::pair<std::string, uint8_t > > AccessUnit::getPosDescriptorValues() {
    return descriptors[0].getPayload();
}

std::vector<std::pair<std::string, uint8_t > > AccessUnit::getRcompDescriptorValues() {
    return descriptors[1].getPayload();
}

std::vector<std::pair<std::string, uint8_t > > AccessUnit::getFlagsDescriptorValues() {
    return descriptors[2].getPayload();
}

std::vector<std::pair<std::string, uint8_t > > AccessUnit::getRLenDescriptorValues() {
    return descriptors[3].getPayload();
}

std::vector<std::pair<std::string, uint8_t > > AccessUnit::getPairDescriptorValues() {
    return descriptors[4].getPayload();
}

std::vector<std::pair<std::string, uint8_t > >  AccessUnit::getMmposDescriptorValues() {
    return descriptors[5].getPayload();
}

std::vector<std::pair<std::string, uint8_t > > AccessUnit::getMmtypeDescriptorValues() {
    return descriptors[6].getPayload();
}

void AccessUnit::updateReads() {
    ++reads_count;
}

uint32_t AccessUnit::getReadsCount() {
    return reads_count;
}

void AccessUnit::write() {
    std::cout << "Type : ";
    if (AU_type == 1) std::cout << "P ";
    else if (AU_type == 2) std::cout << "N ";
    else if (AU_type == 3) std::cout << "M ";
    else if (AU_type == 4) std::cout << "I ";
    std::cout << "ID = " << access_unit_id << " " << AU_start_position << " " << AU_end_position << " number of reads = " << reads_count << std::endl;
}