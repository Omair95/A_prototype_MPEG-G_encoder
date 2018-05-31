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

    /** @brief Writes the pos descriptor value to the respective file
     *         according to the class type
     *  @param value unsigned integer representing the value of the pos descriptor
     *  @param classType class type
     *   @return void
     * */
    void insertPosValue(uint32_t value, uint8_t classType);

    /** @brief calculates, returns and writes the rcomp descriptor value of the read
     *          to the respective file according to the class type
     *  @param reocord first read from the read pair
     *  @param second second read from the read pair
     *  @param classType classType of the read pair
     *  @return Rcomp descriptor value of the read
     * */
    uint8_t insertRcompValue(BamAlignmentRecord& record, BamAlignmentRecord& second, uint8_t classType);

    /** @brief Return and writes the flags descriptor value to the respective file
     *         according to the class type of the read
     *  @param record read from the read pair
     *  @param classType class type
     *   @return Flags descriptor value of the read
     * */
    uint8_t insertFlagsValue(BamAlignmentRecord& record, uint8_t classType);

    /** @brief Returns and writes the rlen descriptor value to the respective file
     *         according to the class type
     *  @param record read from the read pair
     *  @param classType class type
     *   @return Rlen descriptor value of the read
     * */
    uint8_t insertRlenValue(BamAlignmentRecord& record, uint8_t classType);
    
    /** @brief Returns and writes the pair descriptor value to the respective file
     *         according to the class type
     *  @param record first read from the read pair
     *  @param record2 second read from the read pair
     *  @param classType class type
     *   @return pair descriptor value of the read pair
     * */
    std::vector<std::string> insertPairValue(BamAlignmentRecord& record, BamAlignmentRecord& record2, uint8_t classType);

    /** @brief Writes the mmpos descriptor values to the respective file
     *         according to the class type
     *  @param pos position of the mismatch
     *  @param classType class type
     *  @param lastPos check if its the last position of the mismatch
     *   @return void
     * */
    void insertMmposValue(uint16_t pos, uint8_t classType, bool lastPos);

    /** @brief Returns and writes the mmtype descriptor value to the respective file
     *         according to the class type
     *  @param mmpos all mismatches of the read 
     *  @param classType class type
     *   @return a vector containing the type of mismatches according to the alphabet 1
     * */
    std::vector<uint8_t> insertmmtypeDescriptor(std::vector<std::pair<uint16_t, std::string> >& mmpos, uint8_t classType);

    /** @brief Returns and writes the sclips descriptor value to the respective file
     *         according to the class type
     *  @param record the genomic record of the read
     *  @param id identifier of the record
     *   @return a vector containing all the soft clips descriptor values
     * */
    std::vector<std::string> insertClipsDescriptor(MpeggRecord& record, uint32_t id);

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

    /** @brief Write an unsigned 8 bit integer rcomp descriptor value to the respective file according to the class type 
     * @param value unsigned integer value to write
     * @param classType class type of the read
     * @return void
     * */
    void write8bitRcomp(uint8_t value, uint8_t classType);

    /** @brief Write an unsigned 8 bit integer pair descriptor value to the respective file according to the class type 
     * @param value unsigned integer value to write
     * @param classType class type of the read
     * @return void
     * */
    void write8bitPair(uint8_t value, uint8_t classType);

    /** @brief Write an unsigned 16 bit integer pair descriptor value to the respective file according to the class type 
     * @param value unsigned integer value to write
     * @param classType class type of the read
     * @return void
     * */
    void write16bitPair(uint16_t value, uint8_t classType);

    /** @brief Write an unsigned 32 bit integer pair descriptor value to the respective file according to the class type 
     * @param value unsigned integer value to write
     * @param classType class type of the read
     * @return void
     * */
    void write32bitPair(uint32_t value, uint8_t classType);

    void writePosDescriptor(uint32_t value, uint8_t classType);

    void writeRcompDescriptor(uint8_t value, uint8_t classType);

    void writeFlagsDescriptor(uint8_t value, uint8_t classType);

    void writeRlenDescriptor(uint8_t value, uint8_t classType);

    void write8bitPairDescriptor(uint8_t value, uint8_t classType);

    void write16bitPairDescriptor(uint16_t value, uint8_t classType);

    void write32bitPairDescriptor(uint32_t value, uint8_t classType);

    void writeMmposDescriptorValue(uint16_t value, uint8_t classType, bool lastPos);

    void writeMmtypeDescriptor(uint8_t value, uint8_t classType);

    void writeAccessUnits(Utils& u);
};

#endif //A_PROTOTYPE_MPEG_G_ENCODER_FILEMANAGER_H