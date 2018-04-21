#include "AccessUnit.h"

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_M_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_M_H

class AccessUnit_M : public AccessUnit {

public:
    AccessUnit_M(uint32_t id);

    ~AccessUnit_M();

    void insertMmposDescriptor(std::string value);

    void insertMmtypeDescriptor(std::string value);

    void insertRlenDescriptor(std::string value);

    void insertPairDescriptor(std::string value);
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_M_H
