#include <fstream>
#include "MpeggRecord.h"

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
    FileManager();

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
    void insertPosValue(uint32_t value, int classType);

    /** \brief Writes the rcomp descriptor value to the respective file
     *         according to the class type
     *  \param value value of the rcomp descriptor
     *  \param classType class type
     * */
    void insertRcompValue(uint8_t value, int classType);

    /** \brief Writes the flags descriptor value to the respective file
     *         according to the class type
     *  \param value value of the flags descriptor
     *  \param classType class type
     *   \return void
     * */
    void insertFlagsValue(uint8_t value, int classType);

    /** \brief Writes the mmpos descriptor value to the respective file
     *         according to the class type
     *  \param value value of the mmpos descriptor
     *  \param classType class type
     *   \return void
     * */
    void insertMmposValue(uint16_t value, int classType);

    /** \brief Writes the mmtype descriptor value to the respective file
     *         according to the class type
     *  \param value value of the mmtype descriptor
     *  \param classType class type
     *   \return void
     * */
    void insertMmtypeValue(uint8_t value, int classType);

    /** \brief Writes the rlen descriptor value to the respective file
     *         according to the class type
     *  \param value value of the rlen descriptor
     *  \param classType class type
     *   \return void
     * */
    void insertRlenValue(uint8_t value, int classType);

    /** \brief Writes the pair descriptor value to the respective file
     *         according to the class type
     *  \param value value of the pair descriptor
     *  \param classType class type
     *  \return void
     * */
    void insertPairValue(std::string value, int classType);

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
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_FILEMANAGER_H
