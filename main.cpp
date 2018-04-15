#include "Utils.h"
#include "MpeggRecord.h"
#include "AccessUnit_P.h"
#include "AccessUnit_N.h"
#include "AccessUnit_M.h"
#include "AccessUnit_I.h"
#include "AccessUnit_HM.h"
#include "AccessUnit_U.h"
#include "FileManager.h"
#include <time.h>
#include <algorithm>
#include <boost/endian/conversion.hpp>

/*! \file */

Utils u;
FileManager f;
int au_id = -1;
uint64_t record_id = 0;

void convertToMpeggRecord(MpeggRecord& result, BamAlignmentRecord& record) {
    result.global_Id = ++record_id;
    result.read_name = toCString(record.qName);
    result.class_type = u.getClassType(record);
    result.seq_Id = (result.class_type == 6) ? 0 : record.rID;
    result.read1_first = (record.flag & 64) ? 1 : 0;
    result.flags = record.flag;
    result.number_of_segments = (record.flag & 8 or (record.flag & 1 and record.flag & 128)) ? 1 : 2;
    result.number_of_alignments = 1; // just put 1 for now

    CharString seq = record.seq;
    const char *s1 = toCString(seq);
    std::string sequence(s1);
    std::string qv = toCString(record.qual);

    result.read_len.push_back(sequence.size());
    result.quality_values.push_back(qv);
    result.sequence.push_back(sequence);

    BamAlignmentRecord second;
    if (result.number_of_segments > 1) {
        second = u.getSecondRecord(record);
        CharString seq = second.seq;
        const char *s1 = toCString(seq);
        std::string sequence(s1);
        std::string qv = toCString(record.qual);

        result.read_len.push_back(sequence.size());
        result.quality_values.push_back(qv);
        result.sequence.push_back(sequence);
    }

    result.mapping_pos.push_back(record.beginPos);

    result.cigar_size.resize(result.number_of_segments);
    for (int i = 0; i < result.number_of_segments; ++i) result.cigar_size[i].resize(1);

    result.cigar_size.at(0).push_back(u.getCigar(record.cigar).size());
    if (result.number_of_segments > 1) result.cigar_size.at(1).push_back(u.getCigar(second.cigar).size());

    result.ecigar_string.resize(result.number_of_segments);
    for (int i = 0; i < result.number_of_segments; ++i) result.ecigar_string[i].resize(1);

    result.ecigar_string.at(0).push_back("POR HACER");
    if (result.number_of_segments > 1) result.ecigar_string.at(1).push_back("POR HACER");

    result.reverse_comp.resize(result.number_of_segments);
    for (int i = 0; i < result.number_of_segments; ++i) result.reverse_comp[i].resize(1);

    result.reverse_comp.at(0).push_back((record.flag & 16) ? 0: 1);
    if (result.number_of_segments > 1) result.reverse_comp.at(1).push_back((second.flag & 16) ? 0: 1);
}

/*

void generateByteStream() {
    AccessUnit *AU_P, *AU_N, *AU_M, *AU_I, *AU_HM, *AU_U;
    AU_P = new AccessUnit_P(++au_id);
    AU_N = new AccessUnit_N(++au_id);
    AU_M = new AccessUnit_M(++au_id);
    AU_I = new AccessUnit_I(++au_id);
    AU_HM = new AccessUnit_HM(++au_id);
    AU_U = new AccessUnit_U(++au_id);

    auto it = reads.begin();
    const auto end = reads.end();
    bool firstP = true, firstN = true, firstM = true;
    bool firstI = true, firstHM = true;
    int antPosP = 0, antPosN = 0, antPosM = 0;
    int antPosI = 0, antPosHM = 0;

    while (it != end) {
        MpeggRecord a;
        convertToMpeggRecord(a, it->second.first);
        writeMpeggToFile(a);

        if (a.class_type == 1) {
            // update the number of reads in the access unit
            AU_P->updateReads();

            // get pos descriptor
            if (firstP) {
                firstP = false;
                antPosP = a.mapping_pos[0];
                AU_P->insertPosdescriptor(u.int_to_hex(0));
                AU_P->setStartPosition(a.mapping_pos[0]);
                posDescriptorClassP << u.int_to_hex(0);
            } else {
                AU_P->insertPosdescriptor(u.int_to_hex(a.mapping_pos[0] - antPosP));
                posDescriptorClassP << " " << u.int_to_hex(a.mapping_pos[0] - antPosP);
                antPosP = a.mapping_pos[0];
            }

            // get rcomp descriptor
            std::string rcomp = u.getRcompDescriptor(it->second.first);
            AU_P->insertRcompDescriptor(rcomp);
            rcompDescriptorClassP << rcomp << " ";

            // get flags descriptor
            std::string flags = u.getFlagDescriptor(it->second.first);
            AU_P->insertFlagsDescriptor(flags);
            flagsDescriptorClassP << flags << " ";

            // get rlen descriptor
            std::string rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_P*> (AU_P)->insertRlenDescriptor(rlen);
            rlenDescriptorClassP << rlen << " ";

            // get pair descriptor
            std::string pair = u.getPairDescriptor(it->second.first);
            static_cast<AccessUnit_P*> (AU_P)->insertPairDescriptor(pair);
            pairDescriptorClassP << pair << " ";

            // create a new access unit in case if the current one is full
            if (AU_P->getReadsCount() == 100000) {
                AU_P->setEndPosition(a.mapping_pos[0]);
                accessUnits.push_back(*AU_P);
                AU_P = new AccessUnit_P(++au_id);
                AU_P->setStartPosition(a.mapping_pos[0]);
            }

            // erase the read after using it
            reads.erase(it++);

        } else if (a.class_type == 2) {
            // update the number of reads in the access unit
            AU_N->updateReads();

            // get pos descriptor
            if (firstN) {
                firstN = false;
                antPosN = a.mapping_pos[0];
                AU_N->insertPosdescriptor(u.int_to_hex(0));
                AU_N->setStartPosition(a.mapping_pos[0]);
                posDescriptorClassN << u.int_to_hex(0) << " ";
            } else {
                AU_N->insertPosdescriptor(u.int_to_hex(a.mapping_pos[0] - antPosN));
                posDescriptorClassN << u.int_to_hex(a.mapping_pos[0] - antPosN) << " ";
                antPosN = a.mapping_pos[0];
            }

            // get rcomp descriptor
            std::string rcomp = u.getRcompDescriptor(it->second.first);
            AU_N->insertRcompDescriptor(rcomp);
            rcompDescriptorClassN << rcomp << " ";

            // get flags descriptor
            std::string flags = u.getFlagDescriptor(it->second.first);
            AU_N->insertFlagsDescriptor(flags);
            flagsDescriptorClassN << flags << " ";

            // get mmpos descriptor
            std::vector<std::pair<int, std::string> > mmpos = u.getmmposDescriptor(it->second.first);

            for (int i = 0; i < mmpos.size(); ++i) {
                static_cast<AccessUnit_N*> (AU_N)->insertMmposDescriptor(u.int_to_hex(mmpos[i].first));
                mmposDescriptorClassN << u.int_to_hex(mmpos[i].first) << " ";
            }

            // get rlen descriptor
            std::string rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_N*> (AU_N)->insertRlenDescriptor(rlen);
            rlenDescriptorClassN << rlen << " ";

            // get pair descriptor
            std::string pair = u.getPairDescriptor(it->second.first);
            static_cast<AccessUnit_N*> (AU_N)->insertPairDescriptor(pair);
            pairDescriptorClassN << pair << " ";

            // create a new access unit in case if the current one is full
            if (AU_N->getReadsCount() == 100000) {
                // create a new accessUnit
                AU_N->setEndPosition(a.mapping_pos[0]);
                accessUnits.push_back(*AU_N);
                AU_N = new AccessUnit_N(++au_id);
                AU_N->setStartPosition(a.mapping_pos[0]);
            }

            // erase the read after using it
            reads.erase(it++);

        } else if (a.class_type == 3) {
            // update the number of reads in the access unit
            AU_M->updateReads();

            // get pos descriptor
            if (firstM) {
                firstM = false;
                antPosM = a.mapping_pos[0];
                AU_M->insertPosdescriptor(u.int_to_hex(0));
                AU_M->setStartPosition(a.mapping_pos[0]);
                posDescriptorClassM << u.int_to_hex(0) << " ";
            } else {
                AU_M->insertPosdescriptor(u.int_to_hex(a.mapping_pos[0] - antPosM));
                posDescriptorClassM << u.int_to_hex(a.mapping_pos[0] - antPosM) << " ";
                antPosM = a.mapping_pos[0];
            }

            // get rcomp descriptor
            std::string rcomp = u.getRcompDescriptor(it->second.first);
            AU_M->insertRcompDescriptor(rcomp);
            rcompDescriptorClassM << rcomp << " ";

            // get flags descriptor
            std::string flags = u.getFlagDescriptor(it->second.first);
            AU_M->insertFlagsDescriptor(flags);
            flagsDescriptorClassM << flags << " ";

            // get mmpos descriptor
            std::vector<std::pair<int, std::string> > mmpos = u.getmmposDescriptor(it->second.first);

            for (int i = 0; i < mmpos.size(); ++i) {
                static_cast<AccessUnit_M*> (AU_M)->insertMmposDescriptor(u.int_to_hex(mmpos[i].first));
                mmposDescriptorClassM << u.int_to_hex(mmpos[i].first) << " ";
            }

            // get mmtype descriptor
            std::vector<std::string> mmtype = u.getmmtypeDescriptor(mmpos);

            for (int i = 0; i < mmtype.size(); ++i) {
                static_cast<AccessUnit_M*> (AU_M)->insertMmtypeDescriptor(mmtype[i]);
                mmtypeDescriptorClassM << mmtype[i] << " ";
            }

            // get rlen descriptor
            std::string rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_M*> (AU_M)->insertRlenDescriptor(rlen);
            rlenDescriptorClassM << rlen << " ";

            // get pair descriptor
            std::string pair = u.getPairDescriptor(it->second.first);
            static_cast<AccessUnit_M*> (AU_M)->insertPairDescriptor(pair);
            pairDescriptorClassM << pair << " ";

            // create a new access unit in case if the current one is full
            if (AU_M->getReadsCount() == 100000) {
                // create a new accessUnit
                AU_M->setEndPosition(a.mapping_pos[0]);
                accessUnits.push_back(*AU_M);
                AU_M = new AccessUnit_M(++au_id);
                AU_M->setStartPosition(a.mapping_pos[0]);
            }

            // erase read after using it
            reads.erase(it++);

        } else if (a.class_type == 4) {
            // update the number of reads in the access unit
            AU_I->updateReads();

            // get pos descriptor
            if (firstI) {
                firstI = false;
                antPosI = a.mapping_pos[0];
                AU_I->insertPosdescriptor(u.int_to_hex(0));
                AU_I->setStartPosition(a.mapping_pos[0]);
                posDescriptorClassI << u.int_to_hex(0) << " ";
            } else {
                AU_I->insertPosdescriptor(u.int_to_hex(a.mapping_pos[0] - antPosI));
                posDescriptorClassI << u.int_to_hex(a.mapping_pos[0] - antPosI) << " ";
                antPosI = a.mapping_pos[0];
            }

            // get rcomp descriptor
            std::string rcomp = u.getRcompDescriptor(it->second.first);
            AU_I->insertRcompDescriptor(rcomp);
            rcompDescriptorClassI << rcomp << " ";

            // get flags descriptor
            std::string flags = u.getFlagDescriptor(it->second.first);
            AU_I->insertFlagsDescriptor(flags);
            flagsDescriptorClassI << flags << " ";

            // get mmpos descriptor
            std::vector<std::pair<int, std::string> > mmpos = u.getmmposDescriptor(it->second.first);

            for (int i = 0; i < mmpos.size(); ++i) {
                static_cast<AccessUnit_I*> (AU_I)->insertMmposDescriptor(u.int_to_hex(mmpos[i].first));
                mmposDescriptorClassI << u.int_to_hex(mmpos[i].first) << " ";
            }

            // get mmtype descriptor
            std::vector<std::string> mmtype = u.getmmtypeDescriptor(mmpos);

            for (int i = 0; i < mmtype.size(); ++i) {
                static_cast<AccessUnit_I*> (AU_I)->insertMmtypeDescriptor(mmtype[i]);
                mmtypeDescriptorClassI << mmtype[i] << " ";
            }

            // get rlen descriptor
            std::string rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_I*> (AU_I)->insertRlenDescriptor(rlen);
            rlenDescriptorClassI << rlen << " ";

            // get pair descriptor
            std::string pair = u.getPairDescriptor(it->second.first);
            static_cast<AccessUnit_I*> (AU_I)->insertPairDescriptor(pair);
            pairDescriptorClassI << pair << " ";

            // create a new access unit in case if the current one is full
            if (AU_I->getReadsCount() == 100000) {
                // create a new accessUnit
                AU_I->setEndPosition(a.mapping_pos[0]);
                accessUnits.push_back(*AU_I);
                AU_I = new AccessUnit_I(++au_id);
                AU_I->setStartPosition(a.mapping_pos[0]);
            }

            // erase the read after using it
            reads.erase(it++);

        } else if (a.class_type == 5) {
            // update the number of reads in the access unit
            AU_HM->updateReads();

            // get pos descriptor
            if (firstHM) {
                firstHM = false;
                antPosHM = a.mapping_pos[0];
                AU_HM->insertPosdescriptor(u.int_to_hex(0));
                AU_HM->setStartPosition(a.mapping_pos[0]);
                posDescriptorClassHM << u.int_to_hex(0) << " ";
            } else {
                AU_HM->insertPosdescriptor(u.int_to_hex(a.mapping_pos[0] - antPosHM));
                posDescriptorClassHM << u.int_to_hex(a.mapping_pos[0] - antPosHM) << " ";
                antPosHM = a.mapping_pos[0];
            }

            // get rcomp descriptor
            std::string rcomp = u.getRcompDescriptor(it->second.first);
            AU_HM->insertRcompDescriptor(rcomp);
            rcompDescriptorClassHM << rcomp << " ";

            // get flags descriptor
            std::string flags = u.getFlagDescriptor(it->second.first);
            AU_HM->insertFlagsDescriptor(flags);
            flagsDescriptorClassHM << flags << " ";

            // get rlen descriptor
            std::string rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_HM*> (AU_HM)->insertRlenDescriptor(rlen);
            rlenDescriptorClassHM << rlen << " ";

            // get pair descriptor
            std::string pair = u.getPairDescriptor(it->second.first);
            static_cast<AccessUnit_HM*> (AU_HM)->insertPairDescriptor(pair);
            pairDescriptorClassHM << pair << " ";

            // create a new access unit in case if the current one is full
            if (AU_HM->getReadsCount() == 100000) {
                // create a new accessUnit
                AU_HM->setEndPosition(a.mapping_pos[0]);
                accessUnits.push_back(*AU_HM);
                AU_HM = new AccessUnit_HM(++au_id);
                AU_HM->setStartPosition(a.mapping_pos[0]);
            }

            // erase the read after using it
            reads.erase(it++);

        } else if (a.class_type == 6) {
            // update the number of reads in the access unit
            AU_U->updateReads();

            // get rlen descriptor
            std::string rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_U*> (AU_U)->insertRlenDescriptor(rlen);
            rlenDescriptorClassU << rlen << " ";

            // create a new access unit in case if the current one is full
            if (AU_U->getReadsCount() == 100000) {
                // create a new accessUnit
                AU_U->setEndPosition(a.mapping_pos[0]);
                accessUnits.push_back(*AU_U);
                AU_U = new AccessUnit_U(++au_id);
                AU_U->setStartPosition(a.mapping_pos[0]);
            }

            // erase the read after using it
            reads.erase(it++);

        } else ++it;
    }

    // add all incomplete access units
    accessUnits.push_back(*AU_P);
    accessUnits.push_back(*AU_N);
    accessUnits.push_back(*AU_M);
    accessUnits.push_back(*AU_I);
    accessUnits.push_back(*AU_HM);
    accessUnits.push_back(*AU_U);
}

*/

int main () {
    CharString fileName = "/home/omair/TFG/Files/9827_2#49.sam";
    BamFileIn bamFileIn(toCString(fileName));

    BamHeader header;
    readHeader(header, bamFileIn);

    int count = 1;
    BamAlignmentRecord record;
    while (!atEnd(bamFileIn) and count <= 1000) {
        readRecord(record, bamFileIn);

        if (record.beginPos <= record.pNext) {
            reads.insert(std::make_pair(record.beginPos, std::make_pair(record, record)));
        } else {
            u.updateRecord(record, record.pNext);
        }
        ++count;
    }

    // generateByteStream();

    std::cout << accessUnits.size() << std::endl;

    for (int i = 0; i < accessUnits.size(); ++i) {
        accessUnits[i].write();
    }

    // u.closeFiles();

    return 0;
}

// 23020 bam.pos