#include "FileManager.h"

FileManager::FileManager(std::string fileName) {
    mpeggRecordFile.open("../Files/" + fileName + ".mpegg", std::ofstream::out | std::ofstream::trunc);

    posDescriptorClassP.open("../Files/" + fileName + ".mpegg.pos", std::ofstream::out | std::ofstream::trunc);
    rcompDescriptorClassP.open("../Files/" + fileName + ".mpegg.rcomp", std::ofstream::out | std::ofstream::trunc);
    flagsDescriptorClassP.open("../Files/" + fileName + ".mpegg.flags", std::ofstream::out | std::ofstream::trunc);
    rlenDescriptorClassP.open("../Files/" + fileName + ".mpegg.rlen", std::ofstream::out | std::ofstream::trunc);
    pairDescriptorClassP.open("../Files/" + fileName + ".mpegg.pair", std::ofstream::out | std::ofstream::trunc);

    posDescriptorClassN.open("../Files/" + fileName + ".mpegg.npos", std::ofstream::out | std::ofstream::trunc);
    rcompDescriptorClassN.open("../Files/" + fileName + ".mpegg.nrcomp", std::ofstream::out | std::ofstream::trunc);
    flagsDescriptorClassN.open("../Files/" + fileName + ".mpegg.nflags", std::ofstream::out | std::ofstream::trunc);
    mmposDescriptorClassN.open("../Files/" + fileName + ".mpegg.nmmpos", std::ofstream::out | std::ofstream::trunc);
    rlenDescriptorClassN.open("../Files/" + fileName + ".mpegg.nrlen", std::ofstream::out | std::ofstream::trunc);
    pairDescriptorClassN.open("../Files/" + fileName + ".mpegg.npair", std::ofstream::out | std::ofstream::trunc);

    posDescriptorClassM.open("../Files/" + fileName + ".mpegg.mpos", std::ofstream::out | std::ofstream::trunc);
    rcompDescriptorClassM.open("../Files/" + fileName + ".mpegg.mrcomp", std::ofstream::out | std::ofstream::trunc);
    flagsDescriptorClassM.open("../Files/" + fileName + ".mpegg.mflags", std::ofstream::out | std::ofstream::trunc);
    mmposDescriptorClassM.open("../Files/"+ fileName + ".mpegg.mmmpos", std::ofstream::out | std::ofstream::trunc);
    mmtypeDescriptorClassM.open("../Files/" + fileName + ".mpegg.mmmtype", std::ofstream::out | std::ofstream::trunc);
    rlenDescriptorClassM.open("../Files/" + fileName + ".mpegg.mrlen", std::ofstream::out | std::ofstream::trunc);
    pairDescriptorClassM.open("../Files/" + fileName + ".mpegg.mpair", std::ofstream::out | std::ofstream::trunc);

    posDescriptorClassI.open("../Files/" + fileName + ".mpegg.ipos", std::ofstream::out | std::ofstream::trunc);
    rcompDescriptorClassI.open("../Files/" + fileName + ".mpegg.ircomp", std::ofstream::out | std::ofstream::trunc);
    flagsDescriptorClassI.open("../Files/" + fileName + ".mpegg.iflags", std::ofstream::out | std::ofstream::trunc);
    mmposDescriptorClassI.open("../Files/" + fileName + ".mpegg.immpos", std::ofstream::out | std::ofstream::trunc);
    mmtypeDescriptorClassI.open("../Files/" + fileName + ".mpegg.immtype", std::ofstream::out | std::ofstream::trunc);
    clipsDescriptorClassI.open("../Files/" + fileName + ".mpegg.iclips", std::ofstream::out | std::ofstream::trunc);
    rlenDescriptorClassI.open("../Files/" + fileName + ".mpegg.irlen", std::ofstream::out | std::ofstream::trunc);
    pairDescriptorClassI.open("../Files/" + fileName + ".mpegg.ipair", std::ofstream::out | std::ofstream::trunc);
}

FileManager::~FileManager() = default;

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
}

void FileManager::writePosDescriptor(uint32_t value, uint8_t classType) {
    uint32_t littleEndianResult = boost::endian::native_to_little(value);

    if (classType == 1) {
        posDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 2) {
        posDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 3) {
        posDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 4) {
        posDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    }
}

void FileManager::writeRcompDescriptor(uint8_t value, uint8_t classType) {
    uint8_t littleEndianResult = boost::endian::native_to_little(value);

    if (classType == 1) {
        rcompDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 2) {
        rcompDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 3) {
        rcompDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 4) {
        rcompDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    }
}

void FileManager::writeFlagsDescriptor(uint8_t value, uint8_t classType) {
    uint8_t littleEndianResult = boost::endian::native_to_little(value);

    if (classType == 1) {
        flagsDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 2) {
        flagsDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 3) {
        flagsDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 4) {
        flagsDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    }
}

void FileManager::writeRlenDescriptor(uint8_t value, uint8_t classType) {
    uint8_t littleEndianResult = boost::endian::native_to_little(value);

    if (classType == 1) {
        rlenDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 2) {
        rlenDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 3) {
        rlenDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 4) {
        rlenDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    }
}

void FileManager::write8bitPairDescriptor(uint8_t value, uint8_t classType) {
    uint8_t littleEndianResult = boost::endian::native_to_little(value);

    if (classType == 1) {
        pairDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 2) {
        pairDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 3) {
        pairDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 4) {
        pairDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    }
}

void FileManager::write16bitPairDescriptor(uint16_t value, uint8_t classType) {
    uint16_t littleEndianResult = boost::endian::native_to_little(value);

    if (classType == 1) {
        pairDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 2) {
        pairDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 3) {
        pairDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 4) {
        pairDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    }
}

void FileManager::write32bitPairDescriptor(uint32_t value, uint8_t classType) {
    uint32_t littleEndianResult = boost::endian::native_to_little(value);

    if (classType == 1) {
        pairDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 2) {
        pairDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 3) {
        pairDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 4) {
        pairDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    }
}

void FileManager::writeMmposDescriptorValue(uint16_t value, uint8_t classType, bool lastPos) {
    uint16_t littleEndianValue = boost::endian::native_to_little(value);
    uint16_t endPos = boost::endian::native_to_little(0x03e9);
    if (classType == 2) {
        mmposDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
        if (lastPos) mmposDescriptorClassN.write(reinterpret_cast<const char *>(&endPos), sizeof(endPos));
    } else if (classType == 3) {
        mmposDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
        if (lastPos) mmposDescriptorClassM.write(reinterpret_cast<const char *>(&endPos), sizeof(endPos));
    } else if (classType == 4) {
        mmposDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
        if (lastPos) mmposDescriptorClassI.write(reinterpret_cast<const char *>(&endPos), sizeof(endPos));
    }
}

void FileManager::writeMmtypeDescriptor(uint8_t value, uint8_t classType) {
    uint8_t littleEndianResult = boost::endian::native_to_little(value);

    if (classType == 3) {
        mmtypeDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    } else if (classType == 4) {
        mmtypeDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    }
}

void FileManager::write8bitSoftclipDescriptor(uint8_t value, uint8_t classType) {
    uint8_t littleEndianResult = boost::endian::native_to_little(value);

    if (classType == 4) {
        clipsDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    }
}

void FileManager::write32bitSoftclipDescriptor(uint32_t value, uint8_t classType) {
    uint32_t littleEndianResult = boost::endian::native_to_little(value);

    if (classType == 4) {
        clipsDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
    }
}

void FileManager::writeAccessUnit(AccessUnit au) {

    au.write();

    // write pos descriptor to file
    std::vector<std::pair<std::string, uint8_t > > pos = au.getPosDescriptorValues();
    for (int j = 0; j < pos.size(); ++j) {
        uint32_t value = static_cast<uint32_t>(atoi(pos[j].first.c_str()));
        writePosDescriptor(value, au.getType());
    }

    // write rcomp descriptor to file
    std::vector<std::pair<std::string, uint8_t > > rcomp = au.getRcompDescriptorValues();
    for (int j = 0; j < rcomp.size(); ++j) {
        uint8_t value = static_cast<uint8_t>(atoi(rcomp[j].first.c_str()));
        writeRcompDescriptor(value, au.getType());
    }

    // write pair descriptor to file
    std::vector<std::pair<std::string, uint8_t > > flags = au.getFlagsDescriptorValues();
    for (int j = 0; j < flags.size(); ++j) {
        uint8_t value = static_cast<uint8_t>(atoi(flags[j].first.c_str()));
        writeFlagsDescriptor(value, au.getType());
    }

    // write rlen descriptor to file
    std::vector<std::pair<std::string, uint8_t > > rlen = au.getRLenDescriptorValues();
    for (int j = 0; j < rlen.size(); ++j) {
        uint8_t value = static_cast<uint8_t>(atoi(flags[j].first.c_str()));
        writeRlenDescriptor(value, au.getType());
    }

    // write pair descriptor to file
    std::vector<std::pair<std::string, uint8_t > > pair = au.getPairDescriptorValues();
    for (int j = 0; j < pair.size(); ++j) {
        if (pair[j].second == 8) {
            uint8_t value = atoi(pair[j].first.c_str());
            write8bitPairDescriptor(value, au.getType());

        } else if (pair[j].second == 16) {
            uint16_t value;
            std::stringstream ss;
            ss << std::hex << pair[j].first;
            ss >> value;
            write16bitPairDescriptor(value, au.getType());

        } else if (pair[j].second == 32) {
            uint32_t value = atoi(pair[j].first.c_str());
            write32bitPairDescriptor(value, au.getType());

        } else if (pair[j].second == 64) {
            uint16_t value = atoi(pair[j].first.c_str());
            write16bitPairDescriptor(value, au.getType());
        }
    }

    if (au.getType() == 2) {
        // write mmpos descriptor to file
        std::vector<std::pair<std::string, uint8_t > > mmpos = au.getMmposDescriptorValues();

        for (int j = 0; j < mmpos.size(); ++j) {
            uint16_t value = atoi(mmpos[j].first.c_str());
            if (j != (mmpos.size() - 1)) writeMmposDescriptorValue(value, au.getType(), false);
            else writeMmposDescriptorValue(value, au.getType(), true);
        }

    } else if (au.getType() == 3) {
        // write mmpos descriptor to file
        std::vector<std::pair<std::string, uint8_t > > mmpos = au.getMmposDescriptorValues();

        for (int j = 0; j < mmpos.size(); ++j) {
            uint16_t value = atoi(mmpos[j].first.c_str());
            if (j != (mmpos.size() - 1)) writeMmposDescriptorValue(value, au.getType(), false);
            else writeMmposDescriptorValue(value, au.getType(), true);
        }

        // write mmtype descriptor to file
        std::vector<std::pair<std::string, uint8_t > > mmtype = au.getMmtypeDescriptorValues();
        for(int j = 0; j < mmtype.size(); ++j) {
            uint8_t value = atoi(mmtype[j].first.c_str());

            writeMmtypeDescriptor(value, au.getType());
        }
    } else if (au.getType() == 4) {
        // write mmpos descriptor to file
        std::vector<std::pair<std::string, uint8_t> > mmpos = au.getMmposDescriptorValues();

        for (int j = 0; j < mmpos.size(); ++j) {
            uint16_t value = atoi(mmpos[j].first.c_str());
            if (j != (mmpos.size() - 1)) writeMmposDescriptorValue(value, au.getType(), false);
            else writeMmposDescriptorValue(value, au.getType(), true);
        }

        // write mmtype descriptor to file
        std::vector<std::pair<std::string, uint8_t > > mmtype = au.getMmtypeDescriptorValues();
        for(int j = 0; j < mmtype.size(); ++j) {
            uint8_t value = atoi(mmtype[j].first.c_str());

            writeMmtypeDescriptor(value, au.getType());
        }

        // write sclips to file
        std::vector<std::pair<std::string, uint8_t> > sclips = au.getMmtypeDescriptorValues();

        for (int j = 0; j < sclips.size(); ++j) {
            if (sclips[j].second == 8) {
                uint8_t value = atoi(sclips[j].first.c_str());
                write8bitSoftclipDescriptor(value, au.getType());

            } else if (sclips[j].second == 32) {
                uint32_t value = atoi(sclips[j].first.c_str());
                write32bitSoftclipDescriptor(value, au.getType());
            }
        }

    }
}