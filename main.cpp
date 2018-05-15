#include "Utils.h"
#include "AccessUnit_P.h"
#include "AccessUnit_N.h"
#include "AccessUnit_M.h"
#include "AccessUnit_I.h"
#include "AccessUnit_HM.h"
#include "AccessUnit_U.h"
#include "FileManager.h"

/*! \file main.cpp */

int au_id = -1;

// Bam file to read
std::string fileName = "9827_2#49";

/** Auxiliary class to be used that allows to create and write into files
 * */
FileManager f(fileName);

/* Number of references sequences
 * */
std::map<int, std::pair<int, int> > references;

/**
 * */
Utils u;

/** Size of each access unit
 * */
#define ACCESS_UNIT_SIZE 100000

/**
 * \brief This is the main function of the program. It detects the type of data class that a read belongs to
 *  and generates all the associated descriptors
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
    int count = 0;
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
            uint8_t rcomp = f.insertRcompValue(it->second.first, it->second.second, 1);
            AU_P->insertRcompDescriptor(rcomp);


            // get flags descriptor
            uint8_t flags = f.insertFlagsValue(it->second.first, 1);
            AU_P->insertFlagsDescriptor(flags);

            // get rlen descriptor
            uint8_t rlen = f.insertRlenValue(it->second.first, 1);
            static_cast<AccessUnit_P*> (AU_P)->insertRlenDescriptor(rlen);

            // get pair descriptor
            uint16_t pair = f.insertPairValue(it->second.first, it->second.second, 1);
            static_cast<AccessUnit_P*> (AU_P)->insertPairDescriptor(std::to_string(pair));

            std::cout << count << " " << it->second.first.qName << " " << it->second.first.beginPos << " " << it->second.first.pNext << " " << pair << std::endl;
            ++count;

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

        } else ++it; /*
        else if (record.class_type == 2) {
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
            uint8_t rcomp = f.insertRcompValue(it->second.first, it->second.second, 2);
            AU_N->insertRcompDescriptor(rcomp);

            // get flags descriptor
            uint8_t flags = f.insertFlagsValue(it->second.first, 2);
            AU_N->insertFlagsDescriptor(flags);

            // get rlen descriptor
            uint8_t rlen = f.insertRlenValue(it->second.first, 2);
            static_cast<AccessUnit_N*> (AU_N)->insertRlenDescriptor(rlen);

            // get pair descriptor
            uint16_t pair = f.insertPairValue(it->second.first, 2);
            static_cast<AccessUnit_N*> (AU_N)->insertPairDescriptor(std::to_string(pair));

            // get mmpos descriptor
            std::vector<std::pair<uint16_t, std::string> > mmpos = u.getMmposValues(it->second.first);
            for (int i = 0; i < mmpos.size(); ++i) {
                static_cast<AccessUnit_N*> (AU_N)->insertMmposDescriptor(mmpos[i].first);
                if (i != (mmpos.size() - 1)) f.insertMmposValue(mmpos[i].first, 2, false);
                else f.insertMmposValue(mmpos[i].first, 2, true);
            }

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
            uint8_t rcomp = f.insertRcompValue(it->second.first, it->second.second, 3);
            AU_M->insertRcompDescriptor(rcomp);

            // get flags descriptor
            uint8_t flags = f.insertFlagsValue(it->second.first, 3);
            AU_M->insertFlagsDescriptor(flags);

            // get mmpos descriptor
            std::vector<std::pair<uint16_t, std::string> > mmpos = u.getMmposValues(it->second.first);
            for (int i = 0; i < mmpos.size(); ++i) {
                static_cast<AccessUnit_M*> (AU_M)->insertMmposDescriptor(mmpos[i].first);
                if (i != (mmpos.size() - 1)) f.insertMmposValue(mmpos[i].first, 3, false);
                else f.insertMmposValue(mmpos[i].first, 3, true);

            }

            // get mmtype descriptor
            std::vector<uint8_t> mmtype = f.insertmmtypeDescriptor(mmpos, 3);

            for (int i = 0; i < mmtype.size(); ++i) static_cast<AccessUnit_M*> (AU_M)->insertMmtypeDescriptor(mmtype[i]);

            // get rlen descriptor
            uint8_t rlen = f.insertRlenValue(it->second.first, 3);
            static_cast<AccessUnit_M*> (AU_M)->insertRlenDescriptor(rlen);

            // get pair descriptor
            uint16_t pair = f.insertPairValue(it->second.first, 3);
            static_cast<AccessUnit_M*> (AU_M)->insertPairDescriptor(std::to_string(pair));

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

        }
        else if (record.class_type == 4) {
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
            uint8_t rcomp = f.insertRcompValue(it->second.first, 4);
            AU_I->insertRcompDescriptor(rcomp);

            // get flags descriptor
            uint8_t flags = f.insertFlagsValue(it->second.first, 4);
            AU_I->insertFlagsDescriptor(flags);

            // get mmpos descriptor
            std::vector<std::pair<uint16_t, std::string> > mmpos = u.getMmposValues(it->second.first);

            for (int i = 0; i < mmpos.size(); ++i) {
                static_cast<AccessUnit_I*> (AU_I)->insertMmposDescriptor(mmpos[i].first);
                if (i != (mmpos.size() - 1)) f.insertMmposValue(mmpos[i].first, 4, false);
                else f.insertMmposValue(mmpos[i].first, 4, true);
            }

            // get mmtype descriptor
            std::vector<uint8_t> mmtype = f.insertmmtypeDescriptor(mmpos, 4);

            for (int i = 0; i < mmtype.size(); ++i) static_cast<AccessUnit_I*> (AU_I)->insertMmtypeDescriptor(mmtype[i]);

            // get rlen descriptor
            uint8_t rlen = f.insertRlenValue(it->second.first, 4);
            static_cast<AccessUnit_I*> (AU_I)->insertRlenDescriptor(rlen);

            // get pair descriptor
            uint16_t pair = f.insertPairValue(it->second.first, 4);
            static_cast<AccessUnit_I*> (AU_I)->insertPairDescriptor(std::to_string(pair));

            // get clips descriptor
            std::string read1_cigar = u.getCigar(it->second.first.cigar);
            std::string read2_cigar = u.getCigar(it->second.second.cigar);

            if (read1_cigar.find('S') != std::string::npos or read2_cigar.find('S') != std::string::npos) {
                std::vector<std::string> clips = f.insertClipsDescriptor(record, AU_I->getReadsCount() - 1);
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
            uint8_t rcomp = f.insertRcompValue(it->second.first, 5);
            AU_HM->insertRcompDescriptor(rcomp);

            // get flags descriptor
            uint8_t flags = f.insertFlagsValue(it->second.first, 5);
            AU_HM->insertFlagsDescriptor(flags);

            // get rlen descriptor
            uint8_t rlen = f.insertRlenValue(it->second.first, 5);
            static_cast<AccessUnit_HM*> (AU_HM)->insertRlenDescriptor(rlen);

            // get pair descriptor
            uint16_t pair = f.insertPairValue(it->second.first, 5);
            static_cast<AccessUnit_HM*> (AU_HM)->insertPairDescriptor(std::to_string(pair));

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
            uint8_t rlen = f.insertRlenValue(it->second.first, 6);
            static_cast<AccessUnit_U*> (AU_U)->insertRlenDescriptor(rlen);

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

        }*/
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

    long long count = 1;
    BamAlignmentRecord record;

    while (!atEnd(bamFileIn) and count < 10000) { // 8559058 total reads encoded
        readRecord(record, bamFileIn);

        if (references.find(record.rID) == references.end()) {
            references.insert(std::make_pair(record.rID, std::make_pair(record.beginPos, record.beginPos)));
        } else {
            auto it = references.find(record.rID);
            int begin = it->second.first;
            references.erase(record.rID);
            references.insert(std::make_pair(record.rID, std::make_pair(begin, record.beginPos)));
        }

        if (record.tLen > 1500 or record.tLen < -1500) {
            u.insertRead(record, record);
        } else if (record.beginPos <= record.pNext) {
            u.insertRead(record, record);
        } else {
            u.updateRecord(record, record.pNext);
        }
        ++count;
    }

    // 0 10000 249240518
    // 1 10267 237429082


    generateByteStream();
    std::vector<AccessUnit> au;
    u.getAllAccessUnits(au);

    for (int i = 0; i < au.size(); ++i) {
        au[i].write();
    }
    f.closeFiles();

    return 0;
}
