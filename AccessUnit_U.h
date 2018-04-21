#include "AccessUnit.h"

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_U_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_U_H

class AccessUnit_U : public AccessUnit {

public:
    AccessUnit_U(uint32_t id);

    ~AccessUnit_U();

    void insertRlenDescriptor(std::string value);
};

#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_U_H
