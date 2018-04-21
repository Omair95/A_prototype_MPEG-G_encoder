#include "AccessUnit.h"

AccessUnit::AccessUnit() { }

AccessUnit::~AccessUnit() { }

void AccessUnit::setStartPosition(uint32_t start) {
    AU_start_position = start;
}

void AccessUnit::setEndPosition(uint32_t endpos) {
    AU_end_position = endpos;
}

void AccessUnit::insertPosdescriptor(std::string value) {
    descriptors[0].insertValue(value);
}

void AccessUnit::insertRcompDescriptor(std::string value) {
    descriptors[1].insertValue(value);
}

void AccessUnit::insertFlagsDescriptor(std::string value) {
    descriptors[2].insertValue(value);
}

void AccessUnit::getPosDescriptorValues() {
    descriptors[0].write();
}

void AccessUnit::getRcompDescriptorValues() {
    descriptors[1].write();
}

void AccessUnit::getFlagsDescriptorValues() {
    descriptors[2].write();
}

void AccessUnit::updateReads() {
    ++reads_count;
}

uint32_t AccessUnit::getReadsCount() {
    return reads_count;
}

void AccessUnit::write() {
    std::cout << "Type : " << unsigned(AU_type) << " ";
    std::cout << "ID = " << access_unit_id << " " << AU_start_position << " " << AU_end_position << " number of reads = " << reads_count << std::endl;
}
