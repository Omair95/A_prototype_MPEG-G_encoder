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
    descriptors[0].insertValue(std::to_string(value));
}

void AccessUnit::insertRcompDescriptor(uint8_t value) {
    descriptors[1].insertValue(std::to_string(value));
}

void AccessUnit::insertFlagsDescriptor(uint8_t value) {
    descriptors[2].insertValue(std::to_string(value));
}

void AccessUnit::insertRlenDescriptor(uint8_t value) {
    descriptors[3].insertValue(std::to_string(value));
}

void AccessUnit::insertPairDescriptor(std::string value) {
    descriptors[4].insertValue(value);
}

std::vector<std::string> AccessUnit::getPosDescriptorValues() {
    return descriptors[0].getPayload();
}

std::vector<std::string> AccessUnit::getRcompDescriptorValues() {
    return descriptors[1].getPayload();
}

std::vector<std::string> AccessUnit::getFlagsDescriptorValues() {
    return descriptors[2].getPayload();
}

std::vector<std::string> AccessUnit::getRLenDescriptorValues() {
    return descriptors[3].getPayload();
}

std::vector<std::string> AccessUnit::getPairDescriptorValues() {
    return descriptors[4].getPayload();
}

std::vector<std::string> AccessUnit::getMmposDescriptorValues() {
    return descriptors[5].getPayload();
}

std::vector<std::string> AccessUnit::getMmtypeDescriptorValues() {
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