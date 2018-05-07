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

    posDescriptorClassHM.open("../Files/" + fileName + ".mpegg.hmpos", std::ofstream::out | std::ofstream::trunc);
    rcompDescriptorClassHM.open("../Files/" + fileName + ".mpegg.hmrcomp", std::ofstream::out | std::ofstream::trunc);
    flagsDescriptorClassHM.open("../Files/" + fileName + ".mpegg.hmflags", std::ofstream::out | std::ofstream::trunc);
    rlenDescriptorClassHM.open("../Files/" + fileName + ".mpegg.hmrlen", std::ofstream::out | std::ofstream::trunc);
    pairDescriptorClassHM.open("../Files/" + fileName + ".mpegg.hmpair", std::ofstream::out | std::ofstream::trunc);

    rlenDescriptorClassU.open("../Files/" + fileName + ".mpegg.urlen", std::ofstream::out | std::ofstream::trunc);
}

FileManager::~FileManager() { }

void FileManager::insertPosValue(uint32_t value, uint8_t classType) {
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

uint8_t FileManager::insertRcompValue(BamAlignmentRecord& record, uint8_t classType) {
    uint8_t result = 0;
    bool pair1 = record.flag & 16;

    if (record.flag & 8) {
        if (pair1) result = 0;
        else result = 1;
    }
    else {
        bool pair2 = record.flag & 32;
        bool firstRead = record.flag & 64;

        if (firstRead) {
            if (not pair1 and not pair2) result = 3;  // both reads on forward
            else if (pair1 and pair2) result = 0;     // both reads on reverse
            else if (not pair1 and pair2) result = 1; // first read on forward, 2nd on reverse
            else if (pair1 and not pair2) result = 2; // first read on reverse, 2nd on forward
        } else {
            if (not pair1 and not pair2) result = 3;  // both reads on forward
            else if (pair1 and pair2) result = 0;     // both reads on reverse
            else if (not pair1 and pair2) result = 2; // second on forward and 1st on reverse
            else if (pair1 and not pair2) result = 1; // second on reverse and 1st on forward
        }
    }

    uint8_t littleEndianValue = boost::endian::native_to_little(result);

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
    return result;
}

uint8_t FileManager::insertFlagsValue(BamAlignmentRecord& record, uint8_t classType) {
    uint8_t result = 0;
    if (record.flag & 1024) result = 1;
    if (record.flag & 512) result += 2;
    if (not(record.flag & 8)) result += 4;

    uint8_t littleEndianValue = boost::endian::native_to_little(result);

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
    return result;
}

uint8_t FileManager::insertRlenValue(BamAlignmentRecord& record, uint8_t classType) {
    uint8_t result;
    CharString seq = record.seq;
    const char *s1 = toCString(seq);
    std::string str(s1);
    result = str.size();

    uint8_t littleEndianValue = boost::endian::native_to_little(result);
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
    return result;
}

uint16_t FileManager::insertPairValue(BamAlignmentRecord& record, uint8_t classType) {
    bool firstRead = record.flag & 64;
    uint16_t result;

    if (record.flag & 8) {
        if (firstRead) result = 0x8001; // read2 unpaired
        else result = 0x7fff;           // read1 unpaired
        uint16_t littleEndianResult = boost::endian::native_to_little(result);

        if (classType == 1) {
            pairDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
        } else if (classType == 2) {
            pairDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
        } else if (classType == 3) {
            pairDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
        } else if (classType == 4) {
            pairDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
        } else if (classType == 5) {
            pairDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
        }
        return result;
    }

    if (firstRead) {
        if (record.rID == record.rNextId) { // read2 in pair is on the same reference but coded separately
            result = 0x7ffd;
            uint32_t distance = Utils::reads_distance(record);
            uint16_t littleEndianResult = boost::endian::native_to_little(result);
            uint32_t littleEndiandistance = boost::endian::native_to_little(distance);

            if (classType == 1) {
                pairDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndiandistance), sizeof(littleEndiandistance));
            } else if (classType == 2) {
                pairDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndiandistance), sizeof(littleEndiandistance));
            } else if (classType == 3) {
                pairDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndiandistance), sizeof(littleEndiandistance));
            } else if (classType == 4) {
                pairDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndiandistance), sizeof(littleEndiandistance));
            } else if (classType == 5) {
                pairDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndiandistance), sizeof(littleEndiandistance));
            }

        } else {  // read2 is on a diferent reference
            result = 0x7ffe;
            uint16_t referenceID = record.rNextId;
            uint32_t pos = record.pNext;

            uint16_t littleEndianResult = boost::endian::native_to_little(result);
            uint16_t littleEndianRiD = boost::endian::native_to_little(referenceID);
            uint32_t littleEndianPos = boost::endian::native_to_little(pos);

            if (classType == 1) {
                pairDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianRiD), sizeof(littleEndianRiD));
                pairDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianPos), sizeof(littleEndianPos));
            } else if (classType == 2) {
                pairDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianRiD), sizeof(littleEndianRiD));
                pairDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianPos), sizeof(littleEndianPos));
            } else if (classType == 3) {
                pairDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianRiD), sizeof(littleEndianRiD));
                pairDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianPos), sizeof(littleEndianPos));
            } else if (classType == 4) {
                pairDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianRiD), sizeof(littleEndianRiD));
                pairDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianPos), sizeof(littleEndianPos));
            } else if (classType == 5) {
                pairDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndianRiD), sizeof(littleEndianRiD));
                pairDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndianPos), sizeof(littleEndianPos));
            }
        }
    } else {
        if (record.rID == record.rNextId) { // read1 in pair is on the same reference but coded separately
            result = 0x8003;
            uint32_t distance = Utils::reads_distance(record);
            uint16_t littleEndianResult = boost::endian::native_to_little(result);
            uint32_t littleEndiandistance = boost::endian::native_to_little(distance);

            if (classType == 1) {
                pairDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndiandistance), sizeof(littleEndiandistance));
            } else if (classType == 2) {
                pairDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndiandistance), sizeof(littleEndiandistance));
            } else if (classType == 3) {
                pairDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndiandistance), sizeof(littleEndiandistance));
            } else if (classType == 4) {
                pairDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndiandistance), sizeof(littleEndiandistance));
            } else if (classType == 5) {
                pairDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndiandistance), sizeof(littleEndiandistance));
            }

        } else {    // read1 is on a diferente reference
            result = 0x8002;
            uint16_t referenceID = record.rNextId;
            uint32_t pos = record.pNext;

            uint16_t littleEndianResult = boost::endian::native_to_little(result);
            uint16_t littleEndianRiD = boost::endian::native_to_little(referenceID);
            uint32_t littleEndianPos = boost::endian::native_to_little(pos);

            if (classType == 1) {
                pairDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianRiD), sizeof(littleEndianRiD));
                pairDescriptorClassP.write(reinterpret_cast<const char *>(&littleEndianPos), sizeof(littleEndianPos));
            } else if (classType == 2) {
                pairDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianRiD), sizeof(littleEndianRiD));
                pairDescriptorClassN.write(reinterpret_cast<const char *>(&littleEndianPos), sizeof(littleEndianPos));
            } else if (classType == 3) {
                pairDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianRiD), sizeof(littleEndianRiD));
                pairDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianPos), sizeof(littleEndianPos));
            } else if (classType == 4) {
                pairDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianRiD), sizeof(littleEndianRiD));
                pairDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianPos), sizeof(littleEndianPos));
            } else if (classType == 5) {
                pairDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndianResult), sizeof(littleEndianResult));
                pairDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndianRiD), sizeof(littleEndianRiD));
                pairDescriptorClassHM.write(reinterpret_cast<const char *>(&littleEndianPos), sizeof(littleEndianPos));
            }
        }
    }
    return result;
}

void FileManager::insertMmposValue(uint16_t pos, uint8_t classType, bool lastPos) {
    uint16_t littleEndianValue = boost::endian::native_to_little(pos);
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

std::vector<std::string> FileManager::insertClipsDescriptor(MpeggRecord& record, uint32_t id) {
    std::vector <std::string> clips_descriptor;
    std::string read1_cigar = record.ecigar_string[0][1], read2_cigar;
    std::string read1_sequence = record.sequence[0], read2_sequence;
    auto n = std::count(read1_cigar.begin(), read1_cigar.end(), '(');

    for (int i = 0; i < n; ++i) {
        std::string result;

        // get and write id to file
        uint32_t idclip = id;
        uint32_t littleEndianID = boost::endian::native_to_little(idclip);
        clipsDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianID), sizeof(littleEndianID));
        result += std::to_string(idclip);

        // get and write pos of the soft clip
        uint8_t flagPos;
        int pos;
        if (i == 0) {
            pos = read1_cigar.find('(');
            if (pos == 0) flagPos = 0x00;
            else {
                flagPos = 0x01;
                pos = read1_cigar.rfind('(');
            }
        } else {
            flagPos = 0x01;
            pos = read1_cigar.rfind('(');
        }
        uint8_t littleEndianFlagPos = boost::endian::native_to_little(flagPos);
        clipsDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianFlagPos), sizeof(littleEndianFlagPos));
        result += std::to_string(flagPos);

        // get and write base sequences
        if (flagPos == 0x00) {
            int a = 0, b = 0, f;

            if (isdigit(read1_cigar[pos + 1]) and
                isdigit(read1_cigar[pos + 2])) {
                a = read1_cigar[pos+1] - '0';
                b = read1_cigar[pos+2] - '0';
                f = a * 10 + b;
            }
            else if (isdigit(read1_cigar[pos + 1])) {
                a = read1_cigar[pos+1] - '0';
                f = a;
            }

            for (int i = 0; i < f; ++i) {
                uint8_t c = read1_sequence[i];
                clipsDescriptorClassI.write(reinterpret_cast<const char *>(&c), sizeof(c));
            }
            result += read1_sequence.substr(0, f);  // M bases

            if (n > 1) {
                uint8_t terminator = 0xfe;
                uint8_t littleEndianTerminator = boost::endian::native_to_little(terminator);
                clipsDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianTerminator), sizeof(littleEndianTerminator));
                result += std::to_string(terminator);
            }
        } else if (flagPos == 0x01) {
            int a = 0, b = 0, f = 0;

            if (isdigit(read1_cigar[pos + 1]) and
                isdigit(read1_cigar[pos + 2])) {
                a = read1_cigar[pos + 1] - '0';
                b = read1_cigar[pos + 2] - '0';
                f = a * 10 + b;
            }
            else if (isdigit(read1_cigar[pos + 1])) {
                a = read1_cigar[pos + 1] - '0';
                f = a;
            }

            for (int i = read1_sequence.size() - 1 - f; i < read1_sequence.size() - 1; ++i) {
                uint8_t c = read1_sequence[i];
                clipsDescriptorClassI.write(reinterpret_cast<const char *>(&c), sizeof(c));
            }
            result += read1_sequence.substr(read1_sequence.size() - f, read1_sequence.size() - 1);
        }
        clips_descriptor.push_back(result);
    }

    if (record.number_of_segments == 1) {
        uint8_t terminator = 0xff;
        uint8_t littleEndianTerminator = boost::endian::native_to_little(terminator);
        clipsDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianTerminator), sizeof(littleEndianTerminator));
        return clips_descriptor;
    }


    read2_cigar = record.ecigar_string[1][1];
    read2_sequence = record.sequence[1];

    auto k = std::count(read2_cigar.begin(), read2_cigar.end(), '(');

    if (k == 0) {
        uint8_t terminator = 0xff;
        clipsDescriptorClassI.write(reinterpret_cast<const char *>(&terminator), sizeof(terminator));
        return clips_descriptor;
    } else if (n != 0) {
        uint8_t terminator = 0xfe;
        clipsDescriptorClassI.write(reinterpret_cast<const char *>(&terminator), sizeof(terminator));
    }

    for (int i = 0; i < k; ++i) {
        std::string result;

        // get and write id to file
        uint32_t idclip = id;
        uint32_t littleEndianID = boost::endian::native_to_little(idclip);
        clipsDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianID), sizeof(littleEndianID));
        result += std::to_string(idclip);

        // get and write pos of the soft clip
        uint8_t flagPos;
        int pos;
        if (i == 0) {
            pos = read2_cigar.find('(');
            if (pos == 0) flagPos = 0x02;
            else {
                flagPos = 0x03;
                pos = read2_cigar.rfind('(');
            }
        } else {
            flagPos = 0x03;
            pos = read2_cigar.rfind('(');
        }
        uint8_t littleEndianFlagPos = boost::endian::native_to_little(flagPos);
        clipsDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianFlagPos), sizeof(littleEndianFlagPos));
        result += std::to_string(flagPos);

        // get and write base sequences
        if (flagPos == 0x02) {
            int a = 0, b = 0, f;
            if (isdigit(read2_cigar[pos + 1]) and
                isdigit(read2_cigar[pos + 2])) {
                a = read2_cigar[pos + 1] - '0';
                b = read2_cigar[pos + 2] - '0';
                f = a * 10 + b;
            }
            else if (isdigit(read2_cigar[pos + 1])) {
                a = read2_cigar[pos + 1] - '0';
                f = a;
            }

            for (int i = 0; i < f; ++i) {
                uint8_t c = read1_sequence[i];
                clipsDescriptorClassI.write(reinterpret_cast<const char *>(&c), sizeof(c));
            }
            result += read1_sequence.substr(0, f);  // M bases
            uint8_t terminator;

            if (k > 1) terminator = 0xfe;
            else terminator = 0xff;
            clipsDescriptorClassI.write(reinterpret_cast<const char *>(&terminator), sizeof(terminator));
            result += std::to_string(terminator);

        } else if (flagPos == 0x03) {
            int a = 0, b = 0, f;

            if (isdigit(read2_cigar[pos + 1]) and isdigit(read2_cigar[pos + 2])) {
                a = read2_cigar[pos + 1] - '0';
                b = read2_cigar[pos + 2] - '0';
                f = a * 10 + b;
            }
            else if (isdigit(read2_cigar[pos + 1])) {
                a = read2_cigar[pos + 1] - '0';
                f = a;
            }

            for (int i = read2_sequence.size() - 1 - f; i < read2_sequence.size() - 1; ++i) {
                uint8_t c = read2_sequence[i];
                clipsDescriptorClassI.write(reinterpret_cast<const char *>(&c), sizeof(c));
            }
            result += read2_sequence.substr(read2_sequence.size() - f, read2_sequence.size() - 1);

            uint8_t terminator = 0xff;
            clipsDescriptorClassI.write(reinterpret_cast<const char *>(&terminator), sizeof(terminator));
            result += std::to_string(terminator);
        }
        clips_descriptor.push_back(result);
    }

    return  clips_descriptor;
}

std::vector<uint8_t> FileManager::insertmmtypeDescriptor(std::vector<std::pair<uint16_t, std::string> >& mmpos, uint8_t classType) {
    std::vector<uint8_t > result;
    for (int i = 0; i < mmpos.size(); ++i) {
        if (mmpos[i].second[0] == 'I') {
            if (mmpos[i].second[1] == 'A') result.push_back(6);
            else if (mmpos[i].second[1] == 'C') result.push_back(7);
            else if (mmpos[i].second[1] == 'G') result.push_back(8);
            else if (mmpos[i].second[1] == 'T') result.push_back(9);
        }
        else if (mmpos[i].second[0] == 'D') result.push_back(5);
        else if (mmpos[i].second[0] == 'A') result.push_back(0);
        else if (mmpos[i].second[0] == 'C') result.push_back(1);
        else if (mmpos[i].second[0] == 'G') result.push_back(2);
        else if (mmpos[i].second[0] == 'T') result.push_back(3);
    }

    for (int i = 0; i < result.size(); ++i) {
        uint8_t littleEndianValue = boost::endian::native_to_little(result[i]);
        if (classType == 3) {
            mmtypeDescriptorClassM.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
        } else if (classType == 4) {
            mmtypeDescriptorClassI.write(reinterpret_cast<const char *>(&littleEndianValue), sizeof(littleEndianValue));
        }
    }
    return result;
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
