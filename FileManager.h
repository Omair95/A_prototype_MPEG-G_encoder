#include <fstream>

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

    FileManager() {
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

    ~FileManager() { }

    void insertPosValue(std::string value, int classType) {
        if (classType == 1) {
            posDescriptorClassP << value << " ";
        } else if (classType == 2) {
            posDescriptorClassN << value << " ";
        } else if (classType == 3) {
            posDescriptorClassM << value << " ";
        } else if (classType == 4) {
            posDescriptorClassI << value << " ";
        } else if (classType == 5) {
            posDescriptorClassHM << value << " ";
        }
    }

    void insertRcompValue(std::string value, int classType) {
        if (classType == 1) {
            rcompDescriptorClassP << value << " ";
        } else if (classType == 2) {
            rcompDescriptorClassN << value << " ";
        } else if (classType == 3) {
            rcompDescriptorClassM << value << " ";
        } else if (classType == 4) {
            rcompDescriptorClassI << value << " ";
        } else if (classType == 5) {
            rcompDescriptorClassHM << value << " ";
        }
    }

    void insertFlagsValue(std::string value, int classType) {
        if (classType == 1) {
            flagsDescriptorClassP << value << " ";
        } else if (classType == 2) {
            flagsDescriptorClassN << value << " ";
        } else if (classType == 3) {
            flagsDescriptorClassM << value << " ";
        } else if (classType == 4) {
            flagsDescriptorClassI << value << " ";
        } else if (classType == 5) {
            flagsDescriptorClassHM << value << " ";
        }
    }

    void insertMmposValue(std::string value, int classType) {
         if (classType == 2) {
            mmposDescriptorClassN << value << " ";
        } else if (classType == 3) {
            mmposDescriptorClassM << value << " ";
        } else if (classType == 4) {
            mmposDescriptorClassI << value << " ";
        }
    }

    void insertMmtypeValue(std::string value, int classType) {
         if (classType == 3) {
            mmtypeDescriptorClassM << value << " ";
        } else if (classType == 4) {
            mmtypeDescriptorClassI << value << " ";
        }
    }

    void insertRlenValue(std::string value, int classType) {
        if (classType == 1) {
            rlenDescriptorClassP << value << " ";
        } else if (classType == 2) {
            rlenDescriptorClassN << value << " ";
        } else if (classType == 3) {
            rlenDescriptorClassM << value << " ";
        } else if (classType == 4) {
            rlenDescriptorClassI << value << " ";
        } else if (classType == 5) {
            rlenDescriptorClassHM << value << " ";
        } else if (classType == 6) {
            rlenDescriptorClassU << value << " ";
        }
    }

    void insertPairValue(std::string value, int classType) {
        if (classType == 1) {
            pairDescriptorClassP << value << " ";
        } else if (classType == 2) {
            pairDescriptorClassN << value << " ";
        } else if (classType == 3) {
            pairDescriptorClassM << value << " ";
        } else if (classType == 4) {
            pairDescriptorClassI << value << " ";
        } else if (classType == 5) {
            pairDescriptorClassHM << value << " ";
        }
    }

    void writeMpeggToFile(MpeggRecord& result) {
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
    }

    void closeFiles() {
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
};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_FILEMANAGER_H
