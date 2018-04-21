#include <fstream>
#include "MpeggRecord.h"

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

    FileManager();

    ~FileManager();

    void insertPosValue(std::string value, int classType);

    void insertRcompValue(std::string value, int classType);

    void insertFlagsValue(std::string value, int classType);

    void insertMmposValue(std::string value, int classType);

    void insertMmtypeValue(std::string value, int classType);

    void insertRlenValue(std::string value, int classType);

    void insertPairValue(std::string value, int classType);

    void writeMpeggToFile(MpeggRecord& result);

    void closeFiles();
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_FILEMANAGER_H
