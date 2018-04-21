#include "AccessUnit.h"

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_I_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_I_H

class AccessUnit_I : public AccessUnit {

public:
    AccessUnit_I(uint32_t id);

    ~AccessUnit_I();

    void insertMmposDescriptor(std::string value);

    void insertMmtypeDescriptor(std::string value);

    void insertRlenDescriptor(std::string value);

    void insertPairDescriptor(std::string value);
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_I_H
