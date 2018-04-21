#include "AccessUnit.h"

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_N_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_N_H

class AccessUnit_N : public AccessUnit {

private:
    uint16_t mm_threshold;
    uint16_t mm_count;

public:
    AccessUnit_N(uint32_t id);

    ~AccessUnit_N();

    void insertMmposDescriptor(std::string value);

    void insertRlenDescriptor(std::string value);

    void insertPairDescriptor(std::string value);

};

#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_N_H
