#include "AccessUnit.h"

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_P_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_P_H

class AccessUnit_P : public AccessUnit {

public:
    AccessUnit_P(uint32_t id);

    ~AccessUnit_P();

    void insertRlenDescriptor(std::string value);

    void insertPairDescriptor(std::string value);

};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_P_H
