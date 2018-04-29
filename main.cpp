#include "Utils.h"
#include "AccessUnit_P.h"
#include "AccessUnit_N.h"
#include "AccessUnit_M.h"
#include "AccessUnit_I.h"
#include "AccessUnit_HM.h"
#include "AccessUnit_U.h"
#include "FileManager.h"
#include <boost/endian/conversion.hpp>

/*! \file main.cpp */

/** Auxiliary class to be used that contains several useful methods for the main class
 * */
Utils u;

/** Identifier of the access units
 * */
int au_id = -1;

// Bam file to read
std::string fileName = "9827_2#49";

/** Auxiliary class to be used that allows to create and write into files
 * */
FileManager f(fileName);

/** Size of each access unit
 * */
#define ACCESS_UNIT_SIZE 100000

/**
 * \brief This is the main function of the program. It detects the type of data class that a read belongs to
 *  and generates all the asociated descriptors
 * \param void
 * \return void
 * */
void generateByteStream() {
    AccessUnit *AU_P, *AU_N, *AU_M, *AU_I, *AU_HM, *AU_U;
    AU_P = new AccessUnit_P(++au_id);
    AU_N = new AccessUnit_N(++au_id);
    AU_M = new AccessUnit_M(++au_id);
    AU_I = new AccessUnit_I(++au_id);
    AU_HM = new AccessUnit_HM(++au_id);
    AU_U = new AccessUnit_U(++au_id);
    std::multimap<int, std::pair<BamAlignmentRecord, BamAlignmentRecord> > reads;
    u.getAllreads(reads);

    auto it = reads.begin();
    const auto end = reads.end();
    bool firstP = true, firstN = true, firstM = true;
    bool firstI = true, firstHM = true;
    int antPosP = 0, antPosN = 0, antPosM = 0;
    int antPosI = 0, antPosHM = 0;

    while (it != end) {
        MpeggRecord record;
        u.convertToMpeggRecord(record, it->second.first);
        f.writeMpeggToFile(record);

        if (record.class_type == 1) {
            // update the number of reads in the access unit
            AU_P->updateReads();

            // get pos descriptor
            if (firstP) {
                firstP = false;
                antPosP = record.mapping_pos[0];
                AU_P->insertPosdescriptor(0);
                AU_P->setStartPosition(record.mapping_pos[0]);
                f.insertPosValue(0, 1);
            } else {
                AU_P->insertPosdescriptor(record.mapping_pos[0] - antPosP);
                f.insertPosValue(record.mapping_pos[0] - antPosP, 1);
                antPosP = record.mapping_pos[0];
            }

            // get rcomp descriptor
            uint8_t rcomp = u.getRcompDescriptor(it->second.first);
            AU_P->insertRcompDescriptor(rcomp);
            f.insertRcompValue(rcomp, 1);

            // get flags descriptor
            uint8_t flags = u.getFlagDescriptor(it->second.first);
            AU_P->insertFlagsDescriptor(flags);
            f.insertFlagsValue(flags, 1);

            // get rlen descriptor
            uint8_t rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_P*> (AU_P)->insertRlenDescriptor(rlen);
            f.insertRlenValue(rlen, 1);

            // get pair descriptor
            std::string pair = u.getPairDescriptor(it->second.first);

            static_cast<AccessUnit_P*> (AU_P)->insertPairDescriptor(pair);
            f.insertPairValue(pair, 1);

            // create a new access unit in case if the current one is full
            if (AU_P->getReadsCount() == ACCESS_UNIT_SIZE) {
                AU_P->setEndPosition(record.mapping_pos[0]);
                u.insertAccessUnit(*AU_P);
                AU_P = new AccessUnit_P(++au_id);
                AU_P->setStartPosition(record.mapping_pos[0]);
                firstP = true;
            }

            // erase the read after using it
            reads.erase(it++);
            u.removeFirstRead();

        } else if (record.class_type == 2) {
            // update the number of reads in the access unit
            AU_N->updateReads();

            // get pos descriptor
            if (firstN) {
                firstN = false;
                antPosN = record.mapping_pos[0];
                AU_N->insertPosdescriptor(0);
                AU_N->setStartPosition(record.mapping_pos[0]);
                f.insertPosValue(0, 2);
            } else {
                AU_N->insertPosdescriptor(record.mapping_pos[0] - antPosN);
                f.insertPosValue(record.mapping_pos[0] - antPosN, 2);
                antPosN = record.mapping_pos[0];
            }

            // get rcomp descriptor
            uint8_t rcomp = u.getRcompDescriptor(it->second.first);
            AU_N->insertRcompDescriptor(rcomp);
            f.insertRcompValue(rcomp, 2);

            // get flags descriptor
            uint8_t flags = u.getFlagDescriptor(it->second.first);
            AU_N->insertFlagsDescriptor(flags);
            f.insertFlagsValue(flags, 2);

            // get mmpos descriptor
            std::vector<std::pair<uint16_t, std::string> > mmpos = u.getmmposDescriptor(it->second.first);

            for (int i = 0; i < mmpos.size(); ++i) {
                static_cast<AccessUnit_N*> (AU_N)->insertMmposDescriptor(mmpos[i].first);
                f.insertMmposValue(mmpos[i].first, 2);
            }

            // get rlen descriptor
            uint8_t rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_N*> (AU_N)->insertRlenDescriptor(rlen);
            f.insertRlenValue(rlen, 2);

            // get pair descriptor
            std::string pair = u.getPairDescriptor(it->second.first);
            static_cast<AccessUnit_N*> (AU_N)->insertPairDescriptor(pair);
            f.insertPairValue(pair, 2);

            // create a new access unit in case if the current one is full
            if (AU_N->getReadsCount() == ACCESS_UNIT_SIZE) {
                // create a new accessUnit
                AU_N->setEndPosition(record.mapping_pos[0]);
                u.insertAccessUnit(*AU_N);
                AU_N = new AccessUnit_N(++au_id);
                AU_N->setStartPosition(record.mapping_pos[0]);
                firstN = true;
            }

            // erase the read after using it
            reads.erase(it++);
            u.removeFirstRead();

        } else if (record.class_type == 3) {
            // update the number of reads in the access unit
            AU_M->updateReads();

            // get pos descriptor
            if (firstM) {
                firstM = false;
                antPosM = record.mapping_pos[0];
                AU_M->insertPosdescriptor(0);
                AU_M->setStartPosition(record.mapping_pos[0]);
                f.insertPosValue(0, 3);
            } else {
                AU_M->insertPosdescriptor(record.mapping_pos[0] - antPosM);
                f.insertPosValue(record.mapping_pos[0] - antPosM, 3);
                antPosM = record.mapping_pos[0];
            }

            // get rcomp descriptor
            uint8_t rcomp = u.getRcompDescriptor(it->second.first);
            AU_M->insertRcompDescriptor(rcomp);
            f.insertRcompValue(rcomp, 3);

            // get flags descriptor
            uint8_t flags = u.getFlagDescriptor(it->second.first);
            AU_M->insertFlagsDescriptor(flags);
            f.insertFlagsValue(flags, 3);

            // get mmpos descriptor
            std::vector<std::pair<uint16_t, std::string> > mmpos = u.getmmposDescriptor(it->second.first);

            for (int i = 0; i < mmpos.size(); ++i) {
                static_cast<AccessUnit_M*> (AU_M)->insertMmposDescriptor(mmpos[i].first);
                f.insertMmposValue(mmpos[i].first, 3);
            }

            // get mmtype descriptor
            std::vector<uint8_t> mmtype = u.getmmtypeDescriptor(mmpos);

            for (int i = 0; i < mmtype.size(); ++i) {
                static_cast<AccessUnit_M*> (AU_M)->insertMmtypeDescriptor(mmtype[i]);
                f.insertMmtypeValue(mmtype[i], 3);
            }

            // get rlen descriptor
            uint8_t rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_M*> (AU_M)->insertRlenDescriptor(rlen);
            f.insertRlenValue(rlen, 3);

            // get pair descriptor
            std::string pair = u.getPairDescriptor(it->second.first);
            static_cast<AccessUnit_M*> (AU_M)->insertPairDescriptor(pair);
            f.insertPairValue(pair, 3);

            // create a new access unit in case if the current one is full
            if (AU_M->getReadsCount() == ACCESS_UNIT_SIZE) {
                // create a new accessUnit
                AU_M->setEndPosition(record.mapping_pos[0]);
                u.insertAccessUnit(*AU_M);
                AU_M = new AccessUnit_M(++au_id);
                AU_M->setStartPosition(record.mapping_pos[0]);
                firstM = true;
            }

            // erase read after using it
            reads.erase(it++);
            u.removeFirstRead();

        } else if (record.class_type == 4) {
            // update the number of reads in the access unit
            AU_I->updateReads();

            // get pos descriptor
            if (firstI) {
                firstI = false;
                antPosI = record.mapping_pos[0];
                AU_I->insertPosdescriptor(0);
                AU_I->setStartPosition(record.mapping_pos[0]);
                f.insertPosValue(0, 4);
            } else {
                AU_I->insertPosdescriptor(record.mapping_pos[0] - antPosI);
                f.insertPosValue(record.mapping_pos[0] - antPosI, 4);
                antPosI = record.mapping_pos[0];
            }

            // get rcomp descriptor
            uint8_t rcomp = u.getRcompDescriptor(it->second.first);
            AU_I->insertRcompDescriptor(rcomp);
            f.insertRcompValue(rcomp, 4);

            // get flags descriptor
            uint8_t flags = u.getFlagDescriptor(it->second.first);
            AU_I->insertFlagsDescriptor(flags);
            f.insertFlagsValue(flags, 4);

            // get mmpos descriptor
            std::vector<std::pair<uint16_t, std::string> > mmpos = u.getmmposDescriptor(it->second.first);

            for (int i = 0; i < mmpos.size(); ++i) {
                static_cast<AccessUnit_I*> (AU_I)->insertMmposDescriptor(mmpos[i].first);
                f.insertMmposValue(mmpos[i].first, 4);
            }

            // get mmtype descriptor
            std::vector<uint8_t> mmtype = u.getmmtypeDescriptor(mmpos);

            for (int i = 0; i < mmtype.size(); ++i) {
                static_cast<AccessUnit_I*> (AU_I)->insertMmtypeDescriptor(mmtype[i]);
                f.insertMmtypeValue(mmtype[i], 4);
            }

            // get rlen descriptor
            uint8_t rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_I*> (AU_I)->insertRlenDescriptor(rlen);
            f.insertRlenValue(rlen, 4);

            // get pair descriptor
            std::string pair = u.getPairDescriptor(it->second.first);
            static_cast<AccessUnit_I*> (AU_I)->insertPairDescriptor(pair);
            f.insertPairValue(pair, 4);

            // get clips descriptor
            std::string cigar = u.getCigar(it->second.first.cigar);
            if (cigar.find('S') != std::string::npos) {
                std::cout << it->second.first.qName << " " << cigar << " ";
                std::vector<std::string> clips = u.getClipsDescriptor(record);
                for (int i = 0; i < clips.size(); ++i) {
                    std::cout << clips[i] << " ";
                }
                std::cout << std::endl;
            }

            // create a new access unit in case if the current one is full
            if (AU_I->getReadsCount() == ACCESS_UNIT_SIZE) {
                // create a new accessUnit
                AU_I->setEndPosition(record.mapping_pos[0]);
                u.insertAccessUnit(*AU_I);
                AU_I = new AccessUnit_I(++au_id);
                AU_I->setStartPosition(record.mapping_pos[0]);
                firstI = true;
            }

            // erase the read after using it
            reads.erase(it++);
            u.removeFirstRead();

        } else if (record.class_type == 5) {
            // update the number of reads in the access unit
            AU_HM->updateReads();

            // get pos descriptor
            if (firstHM) {
                firstHM = false;
                antPosHM = record.mapping_pos[0];
                AU_HM->insertPosdescriptor(0);
                AU_HM->setStartPosition(record.mapping_pos[0]);
                f.insertPosValue(0, 5);
            } else {
                AU_HM->insertPosdescriptor(record.mapping_pos[0] - antPosHM);
                f.insertPosValue(record.mapping_pos[0] - antPosHM, 5);
                antPosHM = record.mapping_pos[0];
            }

            // get rcomp descriptor
            uint8_t rcomp = u.getRcompDescriptor(it->second.first);
            AU_HM->insertRcompDescriptor(rcomp);
            f.insertRcompValue(rcomp, 5);

            // get flags descriptor
            uint8_t flags = u.getFlagDescriptor(it->second.first);
            AU_HM->insertFlagsDescriptor(flags);
            f.insertFlagsValue(flags, 5);

            // get rlen descriptor
            uint8_t rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_HM*> (AU_HM)->insertRlenDescriptor(rlen);
            f.insertRlenValue(rlen, 5);

            // get pair descriptor
            std::string pair = u.getPairDescriptor(it->second.first);
            static_cast<AccessUnit_HM*> (AU_HM)->insertPairDescriptor(pair);
            f.insertPairValue(pair, 5);

            // create a new access unit in case if the current one is full
            if (AU_HM->getReadsCount() == ACCESS_UNIT_SIZE) {
                // create a new accessUnit
                AU_HM->setEndPosition(record.mapping_pos[0]);
                u.insertAccessUnit(*AU_HM);
                AU_HM = new AccessUnit_HM(++au_id);
                AU_HM->setStartPosition(record.mapping_pos[0]);
                firstHM = true;
            }

            // erase the read after using it
            reads.erase(it++);
            u.removeFirstRead();

        } else if (record.class_type == 6) {
            // update the number of reads in the access unit
            AU_U->updateReads();

            // get rlen descriptor
            uint8_t rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_U*> (AU_U)->insertRlenDescriptor(rlen);
            f.insertRlenValue(rlen, 6);

            // create a new access unit in case if the current one is full
            if (AU_U->getReadsCount() == ACCESS_UNIT_SIZE) {
                // create a new accessUnit
                AU_U->setEndPosition(record.mapping_pos[0]);
                u.insertAccessUnit(*AU_U);
                AU_U = new AccessUnit_U(++au_id);
                AU_U->setStartPosition(record.mapping_pos[0]);
            }

            // erase the read after using it
            reads.erase(it++);
            u.removeFirstRead();

        } else ++it;

    }

    // add all incomplete access units
    u.insertAccessUnit(*AU_P);
    u.insertAccessUnit(*AU_N);
    u.insertAccessUnit(*AU_M);
    u.insertAccessUnit(*AU_I);
    u.insertAccessUnit(*AU_HM);
    u.insertAccessUnit(*AU_U);
}

int main () {
    std::string filePath = "../../TestFiles/" + fileName + ".bam";
    BamFileIn bamFileIn(toCString(filePath));
    BamHeader header;
    readHeader(header, bamFileIn);

    int count = 1;
    BamAlignmentRecord record;
    while (!atEnd(bamFileIn) and count <= 1000) {
        readRecord(record, bamFileIn);

        if (record.beginPos <= record.pNext) {
            u.insertRead(record, record);
        } else {
            u.updateRecord(record, record.pNext);
        }

        ++count;
    }

    generateByteStream();

    std::vector<AccessUnit> au;
    u.getAllAccessUnits(au);

    for (int i = 0; i < au.size(); ++i) {
        au[i].write();
    }

    f.closeFiles();

    return 0;
}
