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
        std::ofstream mpeggRecordFile("../Files/9827_2#49.mpegg", std::ofstream::out | std::ofstream::trunc);

        std::ofstream posDescriptorClassP("../Files/9827_2#49.mpegg.pos", std::ofstream::out | std::ofstream::trunc);
        std::ofstream rcompDescriptorClassP("../Files/9827_2#49.mpegg.rcomp", std::ofstream::out | std::ofstream::trunc);
        std::ofstream flagsDescriptorClassP("../Files/9827_2#49.mpegg.flags", std::ofstream::out | std::ofstream::trunc);
        std::ofstream rlenDescriptorClassP("../Files/9827_2#49.mpegg.rlen", std::ofstream::out | std::ofstream::trunc);
        std::ofstream pairDescriptorClassP("../Files/9827_2#49.mpegg.pair", std::ofstream::out | std::ofstream::trunc);

        std::ofstream posDescriptorClassN("../Files/9827_2#49.mpegg.npos", std::ofstream::out | std::ofstream::trunc);
        std::ofstream rcompDescriptorClassN("../Files/9827_2#49.mpegg.nrcomp", std::ofstream::out | std::ofstream::trunc);
        std::ofstream flagsDescriptorClassN("../Files/9827_2#49.mpegg.nflags", std::ofstream::out | std::ofstream::trunc);
        std::ofstream mmposDescriptorClassN("../Files/9827_2#49.mpegg.nmmpos", std::ofstream::out | std::ofstream::trunc);
        std::ofstream rlenDescriptorClassN("../Files/9827_2#49.mpegg.nrlen", std::ofstream::out | std::ofstream::trunc);
        std::ofstream pairDescriptorClassN("../Files/9827_2#49.mpegg.npair", std::ofstream::out | std::ofstream::trunc);

        std::ofstream posDescriptorClassM("../Files/9827_2#49.mpegg.mpos", std::ofstream::out | std::ofstream::trunc);
        std::ofstream rcompDescriptorClassM("../Files/9827_2#49.mpegg.mrcomp", std::ofstream::out | std::ofstream::trunc);
        std::ofstream flagsDescriptorClassM("../Files/9827_2#49.mpegg.mflags", std::ofstream::out | std::ofstream::trunc);
        std::ofstream mmposDescriptorClassM("../Files/9827_2#49.mpegg.mmmpos", std::ofstream::out | std::ofstream::trunc);
        std::ofstream mmtypeDescriptorClassM("../Files/9827_2#49.mpegg.mmmtype", std::ofstream::out | std::ofstream::trunc);
        std::ofstream rlenDescriptorClassM("../Files/9827_2#49.mpegg.mrlen", std::ofstream::out | std::ofstream::trunc);
        std::ofstream pairDescriptorClassM("../Files/9827_2#49.mpegg.mpair", std::ofstream::out | std::ofstream::trunc);

        std::ofstream posDescriptorClassI("../Files/9827_2#49.mpegg.ipos", std::ofstream::out | std::ofstream::trunc);
        std::ofstream rcompDescriptorClassI("../Files/9827_2#49.mpegg.ircomp", std::ofstream::out | std::ofstream::trunc);
        std::ofstream flagsDescriptorClassI("../Files/9827_2#49.mpegg.iflags", std::ofstream::out | std::ofstream::trunc);
        std::ofstream mmposDescriptorClassI("../Files/9827_2#49.mpegg.immpos", std::ofstream::out | std::ofstream::trunc);
        std::ofstream mmtypeDescriptorClassI("../Files/9827_2#49.mpegg.immtype", std::ofstream::out | std::ofstream::trunc);
        std::ofstream rlenDescriptorClassI("../Files/9827_2#49.mpegg.irlen", std::ofstream::out | std::ofstream::trunc);
        std::ofstream pairDescriptorClassI("../Files/9827_2#49.mpegg.ipair", std::ofstream::out | std::ofstream::trunc);

        std::ofstream posDescriptorClassHM("../Files/9827_2#49.mpegg.hmpos", std::ofstream::out | std::ofstream::trunc);
        std::ofstream rcompDescriptorClassHM("../Files/9827_2#49.mpegg.hmrcomp", std::ofstream::out | std::ofstream::trunc);
        std::ofstream flagsDescriptorClassHM("../Files/9827_2#49.mpegg.hmflags", std::ofstream::out | std::ofstream::trunc);
        std::ofstream rlenDescriptorClassHM("../Files/9827_2#49.mpegg.hmrlen", std::ofstream::out | std::ofstream::trunc);
        std::ofstream pairDescriptorClassHM("../Files/9827_2#49.mpegg.hmpair", std::ofstream::out | std::ofstream::trunc);

        std::ofstream rlenDescriptorClassU("../Files/9827_2#49.mpegg.urlen", std::ofstream::out | std::ofstream::trunc);
    }

    ~FileManager() { }

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
