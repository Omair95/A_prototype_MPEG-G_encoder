#include <fstream>
#include <boost/endian/conversion.hpp>
#include "Utils.h"
using namespace seqan;

/**
 * @file FileManager.h
 * @Author Omair Iqbal
 * @date 04/2018
 * @class FileManager
 * @brief Class to be used to create and write files that contain the output Mpegg format and
 *        all the associated descriptors
 * @contact Omair95@protonmail.com
*/

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_FILEMANAGER_H
#define A_PROTOTYPE_MPEG_G_ENCODER_FILEMANAGER_H

class FileManager {

private:
    std::ofstream mpeggRecordFile;

    std::ofstream posDescriptorClassP;
    std::ofstream rcompDescriptorClassP;
    std::ofstream flagsDescriptorClassP;
    std::ofstream rlenDescriptorClassP;
    std::ofstream pairDescriptorClassP;

    std::ofstream posDescriptorClassN;
    std::ofstream rcompDescriptorClassN;
    std::ofstream flagsDescriptorClassN;
    std::ofstream mmposDescriptorClassN;
    std::ofstream rlenDescriptorClassN;
    std::ofstream pairDescriptorClassN;

    std::ofstream posDescriptorClassM;
    std::ofstream rcompDescriptorClassM;
    std::ofstream flagsDescriptorClassM;
    std::ofstream mmposDescriptorClassM;
    std::ofstream mmtypeDescriptorClassM;
    std::ofstream rlenDescriptorClassM;
    std::ofstream pairDescriptorClassM;

    std::ofstream posDescriptorClassI;
    std::ofstream rcompDescriptorClassI;
    std::ofstream flagsDescriptorClassI;
    std::ofstream mmposDescriptorClassI;
    std::ofstream mmtypeDescriptorClassI;
    std::ofstream clipsDescriptorClassI;
    std::ofstream rlenDescriptorClassI;
    std::ofstream pairDescriptorClassI;
    
public:
    
    /** @brief Constructor method
     * @param void
     * @return void
     * */
    explicit FileManager(std::string FileName);

    /** @brief Destructor method
     * @param void
     * @return void
     * */
    ~FileManager();

    /** @brief Writes the mpegg record of the respective read to a file
     *  @param result mpegg record format of the read
     *  @return void
     * */
    void writeMpeggToFile(MpeggRecord& result);

    /** @brief Close all created files
     * @param void
     * @return void
     * */
    void closeFiles();

    /** @brief Write pos descriptor value to the respective file
     *         according to the class type
     *  @param value unsigned integer representing the value of the pos descriptor
     *  @param classType class type
     *   @return void
     * */
    void writePosDescriptor(uint32_t value, uint8_t classType);

    /** @brief Write rcomp descriptor value to the respective file
     *         according to the class type
     *  @param value unsigned integer representing the value of the rcomp descriptor
     *  @param classType class type
     *   @return void
     * */
    void writeRcompDescriptor(uint8_t value, uint8_t classType);

    /** @brief Write flags descriptor value to the respective file
     *         according to the class type
     *  @param value unsigned integer representing the value of the flags descriptor
     *  @param classType class type
     *   @return void
     * */
    void writeFlagsDescriptor(uint8_t value, uint8_t classType);

    /** @brief Write rlen descriptor value to the respective file
     *         according to the class type
     *  @param value unsigned integer representing the value of the rlen descriptor
     *  @param classType class type
     *   @return void
     * */
    void writeRlenDescriptor(uint8_t value, uint8_t classType);

    /** @brief Write 8 bit pair descriptor value to the respective file
     *         according to the class type
     *  @param value unsigned integer representing the value of the pos descriptor
     *  @param classType class type
     *   @return void
     * */
    void write8bitPairDescriptor(uint8_t value, uint8_t classType);

    /** @brief Write 16 bit pair descriptor value to the respective file
     *         according to the class type
     *  @param value unsigned integer representing the value of the pos descriptor
     *  @param classType class type
     *   @return void
     * */
    void write16bitPairDescriptor(uint16_t value, uint8_t classType);

    /** @brief Write 32 bit pair descriptor value to the respective file
     *         according to the class type
     *  @param value unsigned integer representing the value of the pos descriptor
     *  @param classType class type
     *   @return void
     * */
    void write32bitPairDescriptor(uint32_t value, uint8_t classType);

    /** @brief Write mmpos descriptor value to the respective file
     *         according to the class type
     *  @param value unsigned integer representing the value of the pos descriptor
     *  @param classType class type
     *   @return void
     * */
    void writeMmposDescriptorValue(uint16_t value, uint8_t classType, bool lastPos);

    /** @brief Write mmtype descriptor value to the respective file
     *         according to the class type
     *  @param value unsigned integer representing the value of the pos descriptor
     *  @param classType class type
     *   @return void
     * */
    void writeMmtypeDescriptor(uint8_t value, uint8_t classType);

    /** @brief Write 8 bit soft clip descriptor value to the respective file
     *         according to the class type
     *  @param value unsigned integer representing the value of the pos descriptor
     *  @param classType class type
     *   @return void
     * */
    void write8bitSoftclipDescriptor(uint8_t value, uint8_t classType);

    /** @brief Write 32 bit soft clip descriptor value to the respective file
     *         according to the class type
     *  @param value unsigned integer representing the value of the pos descriptor
     *  @param classType class type
     *   @return void
     * */
    void write32bitSoftclipDescriptor(uint32_t value, uint8_t classType);

    /** @brief Write access units descriptors to files
     *  @param u utils class that containts all the access units to be written
     *   @return void
     * */
    void writeAccessUnits(Utils& u);
};

#endif //A_PROTOTYPE_MPEG_G_ENCODER_FILEMANAGER_H