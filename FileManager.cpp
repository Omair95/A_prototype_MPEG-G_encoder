#include "FileManager.h"

FileManager::FileManager() {
    mpeggRecordFile.open("../Files/9827_2#49.mpegg", std::ofstream::out | std::ofstream::trunc);

    posDescriptorClassP.open("../Files/9827_2#49.mpegg.pos", std::ofstream::out | std::ofstream::trunc);
    rcompDescriptorClassP.open("../Files/9827_2#49.mpegg.rcomp", std::ofstream::out | std::ofstream::trunc);
    flagsDescriptorClassP.open("../Files/9827_2#49.mpegg.flags", std::ofstream::out | std::ofstream::trunc);
    rlenDescriptorClassP.open("../Files/9827_2#49.mpegg.rlen", std::ofstream::out | std::ofstream::trunc);
    pairDescriptorClassP.open("../Files/9827_2#49.mpegg.pair", std::ofstream::out | std::ofstream::trunc);

    posDescriptorClassN.open("../Files/9827_2#49.mpegg.npos", std::ofstream::out | std::ofstream::trunc);
    rcompDescriptorClassN.open("../Files/9827_2#49.mpegg.nrcomp", std::ofstream::out | std::ofstream::trunc);
    flagsDescriptorClassN.open("../Files/9827_2#49.mpegg.nflags", std::ofstream::out | std::ofstream::trunc);
    mmposDescriptorClassN.open("../Files/9827_2#49.mpegg.nmmpos", std::ofstream::out | std::ofstream::trunc);
    rlenDescriptorClassN.open("../Files/9827_2#49.mpegg.nrlen", std::ofstream::out | std::ofstream::trunc);
    pairDescriptorClassN.open("../Files/9827_2#49.mpegg.npair", std::ofstream::out | std::ofstream::trunc);

    posDescriptorClassM.open("../Files/9827_2#49.mpegg.mpos", std::ofstream::out | std::ofstream::trunc);
    rcompDescriptorClassM.open("../Files/9827_2#49.mpegg.mrcomp", std::ofstream::out | std::ofstream::trunc);
    flagsDescriptorClassM.open("../Files/9827_2#49.mpegg.mflags", std::ofstream::out | std::ofstream::trunc);
    mmposDescriptorClassM.open("../Files/9827_2#49.mpegg.mmmpos", std::ofstream::out | std::ofstream::trunc);
    mmtypeDescriptorClassM.open("../Files/9827_2#49.mpegg.mmmtype", std::ofstream::out | std::ofstream::trunc);
    rlenDescriptorClassM.open("../Files/9827_2#49.mpegg.mrlen", std::ofstream::out | std::ofstream::trunc);
    pairDescriptorClassM.open("../Files/9827_2#49.mpegg.mpair", std::ofstream::out | std::ofstream::trunc);

    posDescriptorClassI.open("../Files/9827_2#49.mpegg.ipos", std::ofstream::out | std::ofstream::trunc);
    rcompDescriptorClassI.open("../Files/9827_2#49.mpegg.ircomp", std::ofstream::out | std::ofstream::trunc);
    flagsDescriptorClassI.open("../Files/9827_2#49.mpegg.iflags", std::ofstream::out | std::ofstream::trunc);
    mmposDescriptorClassI.open("../Files/9827_2#49.mpegg.immpos", std::ofstream::out | std::ofstream::trunc);
    mmtypeDescriptorClassI.open("../Files/9827_2#49.mpegg.immtype", std::ofstream::out | std::ofstream::trunc);
    rlenDescriptorClassI.open("../Files/9827_2#49.mpegg.irlen", std::ofstream::out | std::ofstream::trunc);
    pairDescriptorClassI.open("../Files/9827_2#49.mpegg.ipair", std::ofstream::out | std::ofstream::trunc);

    posDescriptorClassHM.open("../Files/9827_2#49.mpegg.hmpos", std::ofstream::out | std::ofstream::trunc);
    rcompDescriptorClassHM.open("../Files/9827_2#49.mpegg.hmrcomp", std::ofstream::out | std::ofstream::trunc);
    flagsDescriptorClassHM.open("../Files/9827_2#49.mpegg.hmflags", std::ofstream::out | std::ofstream::trunc);
    rlenDescriptorClassHM.open("../Files/9827_2#49.mpegg.hmrlen", std::ofstream::out | std::ofstream::trunc);
    pairDescriptorClassHM.open("../Files/9827_2#49.mpegg.hmpair", std::ofstream::out | std::ofstream::trunc);

    rlenDescriptorClassU.open("../Files/9827_2#49.mpegg.urlen", std::ofstream::out | std::ofstream::trunc);
}

FileManager::~FileManager() { }

void FileManager::insertPosValue(uint32_t value, int classType) {
    uint32_t littleEndianValue = boost::endian::native_to_little(value);
    if (classType == 1) {
        posDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 2) {
        posDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 3) {
        posDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 4) {
        posDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 5) {
        posDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    }
}

void FileManager::insertRcompValue(uint8_t value, int classType) {
    uint8_t littleEndianValue = boost::endian::native_to_little(value);
    if (classType == 1) {
        rcompDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 2) {
        rcompDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 3) {
        rcompDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 4) {
        rcompDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 5) {
        rcompDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    }
}

void FileManager::insertFlagsValue(uint8_t value, int classType) {
    uint8_t littleEndianValue = boost::endian::native_to_little(value);
    if (classType == 1) {
        flagsDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 2) {
        flagsDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 3) {
        flagsDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 4) {
        flagsDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 5) {
        flagsDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    }
}

void FileManager::insertMmposValue(uint16_t value, int classType) {
    uint16_t littleEndianValue = boost::endian::native_to_little(value);
    if (classType == 2) {
        mmposDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 3) {
        mmposDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 4) {
        mmposDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    }
}

void FileManager::insertMmtypeValue(uint8_t value, int classType) {
    uint8_t littleEndianValue = boost::endian::native_to_little(value);
    if (classType == 3) {
        mmtypeDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 4) {
        mmtypeDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    }
}

void FileManager::insertRlenValue(uint8_t value, int classType) {
    uint8_t littleEndianValue = boost::endian::native_to_little(value);
    if (classType == 1) {
        rlenDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 2) {
        rlenDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 3) {
        rlenDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 4) {
        rlenDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 5) {
        rlenDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 6) {
        rlenDescriptorClassU.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    }
}

void FileManager::insertPairValue(std::string value, int classType) {
    auto intValue = Utils::hex_to_int(value);
    auto littleEndianValue = boost::endian::native_to_little(intValue);
    if (classType == 1) {
        pairDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 2) {
        pairDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 3) {
        pairDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 4) {
        pairDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    } else if (classType == 5) {
        pairDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
    }
}

void FileManager::writeMpeggToFile(MpeggRecord& result) {
    mpeggRecordFile << result.global_Id << " ";
    mpeggRecordFile << result.read_name << " ";
    mpeggRecordFile << unsigned(result.class_type) << " ";
    mpeggRecordFile << result.seq_Id << " ";
    mpeggRecordFile << unsigned(result.read1_first) << " ";
    mpeggRecordFile << unsigned(result.flags) << " ";
    mpeggRecordFile << unsigned(result.number_of_segments) << " ";
    mpeggRecordFile << result.number_of_alignments << " ";

    for (int i = 0; i < result.number_of_segments; ++i) {
        mpeggRecordFile << result.read_len[i] << " ";
        mpeggRecordFile << result.quality_values[i] << " ";
        mpeggRecordFile << result.sequence[i] << " ";
    }

    for (int i = 0; i < result.number_of_alignments; ++i) {
        mpeggRecordFile << result.mapping_pos[i] << " ";

        for (int j = 0; j < result.number_of_segments; ++j) {
            mpeggRecordFile << result.cigar_size[j][i+1] << " ";
            mpeggRecordFile << result.ecigar_string[j][i+1] << " ";
            mpeggRecordFile << unsigned(result.reverse_comp[j][i+1]) << " ";
        }
    }
    mpeggRecordFile << std::endl;
    mpeggRecordFile << std::endl;
}

void FileManager::closeFiles() {
    mpeggRecordFile.close();
    posDescriptorClassP.close();
    rcompDescriptorClassP.close();
    flagsDescriptorClassP.close();
    rlenDescriptorClassP.close();
    pairDescriptorClassP.close();

    posDescriptorClassN.close();
    rcompDescriptorClassN.close();
    flagsDescriptorClassN.close();
    mmposDescriptorClassN.close();
    rlenDescriptorClassN.close();
    pairDescriptorClassN.close();

    posDescriptorClassM.close();
    rcompDescriptorClassM.close();
    flagsDescriptorClassM.close();
    mmposDescriptorClassM.close();
    mmtypeDescriptorClassM.close();
    rlenDescriptorClassM.close();
    pairDescriptorClassM.close();

    posDescriptorClassI.close();
    rcompDescriptorClassI.close();
    flagsDescriptorClassI.close();
    mmposDescriptorClassI.close();
    mmtypeDescriptorClassI.close();
    pairDescriptorClassI.close();
    rlenDescriptorClassI.close();

    posDescriptorClassHM.close();
    rcompDescriptorClassHM.close();
    flagsDescriptorClassHM.close();
    rlenDescriptorClassHM.close();
    pairDescriptorClassHM.close();

    rlenDescriptorClassU.close();

}
