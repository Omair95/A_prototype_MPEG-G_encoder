#include "AccessUnit.h"

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_HM_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_HM_H

class AccessUnit_HM : public AccessUnit {

public:
    AccessUnit_HM(uint32_t id);

    ~AccessUnit_HM();

    void insertRlenDescriptor(std::string value);

    void insertPairDescriptor(std::string value);
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_HM_H
