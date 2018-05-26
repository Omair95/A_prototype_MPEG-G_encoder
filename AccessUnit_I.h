#include "AccessUnit.h"

/**
 * @file AccessUnit_I.h
 * @Author Omair Iqbal
 * @date 04/2018
 * @class AccessUnit_I
 * @brief Access unit of type I: reads containing at least one insertion, deletion or clipped base, and
 *        optionally unknown bases or substitutions
 * @contact Omair95@protonmail.com
*/

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_I_H
#define A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_I_H

class AccessUnit_I : public AccessUnit {

public:
    /** @brief Constructor method
    * @param id identifier of the access unit of type I (4)
    * @return void
    * */
    explicit AccessUnit_I(uint32_t id);

    /** @brief Destructor method
    * @param void
    * @return void
    * */
    ~AccessUnit_I();

    /** @brief Insert a single mmpos descriptor value inside the corresponding block in the access unit's payload
    * @param value unsigned integer representing the value of the mmpos descriptor
    * @return void
    * */
    void insertMmposDescriptor(uint16_t value);

    /** @brief Insert the mmtype descriptor value inside the corresponding block in the access unit's payload
    * @param value unsigned integer representing the value of the mmtype descriptor
    * @return void
    * */
    void insertMmtypeDescriptor(uint8_t value);
    
    /** @brief Insert the sclips descriptor value inside the corresponding block in the access unit's payload
    * @param sclips vector containing all values of the soft clips descriptor of the read
    * @return void
    * */
    void insertSclipsDescriptor(std::vector<std::string>& sclips);
};

#endif //A_PROTOTYPE_MPEG_G_ENCODER_ACCESSUNIT_I_H