#include <fstream>
#include "MpeggRecord.h"

/*! \file FileManager.h */

/**
 * \class FileManager
 * \brief Class to be used to create and write files that contain the output Mpegg format and
 *        all the asociated descriptors
 *
 * \author $Author: Omair Iqbal $
 *
 * \date $Date: 04/2018 $
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
     *  \param value is the value of the pos descriptor and \param classType is the
     *   class type
     *   \return void
     * */
    void insertPosValue(std::string value, int classType);

    /** \brief Writes the rcomp descriptor value to the respective file
     *         according to the class type
     *  \param value is the value of the rcomp descriptor and \param classType is the
     *   class type
     * */
    void insertRcompValue(std::string value, int classType);

    /** \brief Writes the flags descriptor value to the respective file
     *         according to the class type
     *  \param value is the value of the flags descriptor and \param classType is the
     *   class type
     *   \return void
     * */
    void insertFlagsValue(std::string value, int classType);

    /** \brief Writes the mmpos descriptor value to the respective file
     *         according to the class type
     *  \param value is the value of the mmpos descriptor and \param classType is the
     *   class type
     *   \return void
     * */
    void insertMmposValue(std::string value, int classType);

    /** \brief Writes the mmtype descriptor value to the respective file
     *         according to the class type
     *  \param value is the value of the mmtype descriptor and \param classType is the
     *   class type
     *   \return void
     * */
    void insertMmtypeValue(std::string value, int classType);

    /** \brief Writes the rlen descriptor value to the respective file
     *         according to the class type
     *  \param value is the value of the rlen descriptor and \param classType is the
     *   class type
     *   \return void
     * */
    void insertRlenValue(std::string value, int classType);

    /** \brief Writes the pair descriptor value to the respective file
     *         according to the class type
     *  \param value is the value of the pair descriptor and \param classType is the
     *   class type
     *  \return void
     * */
    void insertPairValue(std::string value, int classType);

    /** \brief Writes the mpegg record of the respective read to a file
     *  \param result is the mpegg record of the read
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
