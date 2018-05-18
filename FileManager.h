#include <fstream>
#include <boost/endian/conversion.hpp>
#include "MpeggRecord.h"
#include "Utils.h"
#include <seqan/bam_io.h>
using namespace seqan;

/*! \file FileManager.h */

/**
 * \class FileManager
 * \brief Class to be used to create and write files that contain the output Mpegg format and
 *        all the associated descriptors
 *
 * \author Omair Iqbal
 *
 * \date 04/2018
 *
 * Contact: Omair95@protonmail.com
 * */

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

    std::ofstream posDescriptorClassHM;
    std::ofstream rcompDescriptorClassHM;
    std::ofstream flagsDescriptorClassHM;
    std::ofstream rlenDescriptorClassHM;
    std::ofstream pairDescriptorClassHM;

    std::ofstream rlenDescriptorClassU;

public:
    /** \brief Constructor method
     * \param void
     * \return void
     * */
    explicit FileManager(std::string FileName);

    /** \brief Destructor method
     * \param void
     * \return void
     * */
    ~FileManager();

    /** \brief Writes the pos descriptor value to the respective file
     *         according to the class type
     *  \param value value of the pos descriptor
     *  \param classType class type
     *   \return void
     * */
    void insertPosValue(uint32_t value, uint8_t classType);

    /** \brief calculates and writes the rcomp descriptor value  of the read
     *          to the respective file according to the class type
     *  \param value value of the rcomp descriptor
     *  \param classType class type
     *  \return Rcomp descriptor value of the read
     * */
    uint8_t insertRcompValue(BamAlignmentRecord& record, BamAlignmentRecord& second, uint8_t classType);

    /** \brief Writes the flags descriptor value to the respective file
     *         according to the class type
     *  \param value value of the flags descriptor
     *  \param classType class type
     *   \return Flags descriptor value of the read
     * */
    uint8_t insertFlagsValue(BamAlignmentRecord& record, uint8_t classType);

    /** \brief Writes the rlen descriptor value to the respective file
     *         according to the class type
     *  \param value value of the rlen descriptor
     *  \param classType class type
     *   \return Rlen descriptor value of the read
     * */
    uint8_t insertRlenValue(BamAlignmentRecord& record, uint8_t classType);


    /** \brief Writes the pair descriptor value to the respective file
     *         according to the class type
     *  \param value value of the rlen descriptor
     *  \param classType class type
     *   \return Rlen descriptor value of the read
     * */
    uint16_t insertPairValue(BamAlignmentRecord& record, BamAlignmentRecord& record2, uint8_t classType);


    void insertMmposValue(uint16_t pos, uint8_t classType, bool lastPos);

    /** \brief Writes the mmtype descriptor value to the respective file
     *         according to the class type
     *  \param value value of the mmtype descriptor
     *  \param classType class type
     *   \return void
     * */
    std::vector<uint8_t> insertmmtypeDescriptor(std::vector<std::pair<uint16_t, std::string> >& mmpos, uint8_t classType);

    std::vector<std::string> insertClipsDescriptor(MpeggRecord& record, uint32_t id);

    /** \brief Writes the mpegg record of the respective read to a file
     *  \param result mpegg record format of the read
     *  \return void
     * */
    void writeMpeggToFile(MpeggRecord& result);

    /** \brief Closes all the created files
     * \param void
     * \return void
     * */
    void closeFiles();

    void write8bitRcomp(uint8_t value, uint8_t classType);

    void write16bit(uint16_t value, uint8_t classType);

    void write32bit(uint32_t value, uint8_t classType);
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_FILEMANAGER_H
