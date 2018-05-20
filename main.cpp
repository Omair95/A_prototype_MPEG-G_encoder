#include "Utils.h"
#include "AccessUnit_P.h"
#include "AccessUnit_N.h"
#include "AccessUnit_M.h"
#include "AccessUnit_I.h"
#include "FileManager.h"

/*! \file main.cpp */

int au_id = -1;

// Bam file to read
std::string fileName = "9827_2#49";

// Auxiliary class to be used that allows to create and write into files
FileManager f(fileName);

// Auxiliary functions
Utils u;

// Size of each access unit
#define ACCESS_UNIT_SIZE 1000000 // 13716

std::multimap<int, std::pair<MpeggRecord, std::vector<std::string> > > tags_read;

void insertTagsToReads() {
    std::multimap<int, std::pair<BamAlignmentRecord, BamAlignmentRecord> > reads;
    u.getAllreads(reads);

    for (auto it = reads.begin(); it != reads.end(); ++it) {
        MpeggRecord record;
        u.convertToMpeggRecord(record, it->second.first);
        f.writeMpeggToFile(record);
        if (it->first < 1000000) {
            std::vector<std::string> tags;
            tags.emplace_back("Doctor");
            tags_read.insert(std::make_pair(it->first, std::make_pair(record, tags)));
        } else {
            std::vector<std::string> tags;
            tags.emplace_back("Student");
            tags_read.insert(std::make_pair(it->first, std::make_pair(record, tags)));
        }
    }
}


/**
 * \brief This is the main function of the program. It detects the type of data class that a read belongs to
 *  and generates all the associated descriptors
 * \param void
 * \return void
 * */
void dispatcher() {
    AccessUnit *AU_P, *AU_N, *AU_M, *AU_I, *AU_HM, *AU_U;
    AU_P = new AccessUnit_P(++au_id);
    AU_N = new AccessUnit_N(++au_id);
    AU_M = new AccessUnit_M(++au_id);
    AU_I = new AccessUnit_I(++au_id);

    std::multimap<int, std::pair<BamAlignmentRecord, BamAlignmentRecord> > reads;
    u.getAllreads(reads);

    auto reads_it = reads.begin();
    auto tagsReads_it = tags_read.begin();
    auto end = tags_read.end();

    bool firstP = true, firstN = true, firstM = true, firstI = true;
    int antPosP = 0, antPosN = 0, antPosM = 0, antPosI = 0;
    int count = 0;

    while (tagsReads_it != end) {

        if (tagsReads_it->second.first.class_type == 1) {
            // update the number of reads in the access unit
            AU_P->updateReads();

            // get pos descriptor
            if (firstP) {
                firstP = false;
                antPosP = tagsReads_it->second.first.mapping_pos[0];
                AU_P->insertPosdescriptor(0);
                AU_P->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                f.insertPosValue(0, 1);
            } else {
                AU_P->insertPosdescriptor(tagsReads_it->second.first.mapping_pos[0] - antPosP);
                f.insertPosValue(tagsReads_it->second.first.mapping_pos[0] - antPosP, 1);
                antPosP = tagsReads_it->second.first.mapping_pos[0];
            }

            // get rcomp descriptor
            uint8_t rcomp = f.insertRcompValue(reads_it->second.first, reads_it->second.second, 1);
            AU_P->insertRcompDescriptor(rcomp);

            // get flags descriptor
            uint8_t flags = f.insertFlagsValue(reads_it->second.first, 1);
            AU_P->insertFlagsDescriptor(flags);

            // get rlen descriptor
            uint8_t rlen = f.insertRlenValue(reads_it->second.first, 1);
            static_cast<AccessUnit_P*> (AU_P)->insertRlenDescriptor(rlen);

            // get pair descriptor
            uint16_t pair = f.insertPairValue(reads_it->second.first, reads_it->second.second, 1);
            static_cast<AccessUnit_P*> (AU_P)->insertPairDescriptor(pair, reads_it->second.first.rID, u.reads_distance(reads_it->second.first));

            // create a new access unit in case if the current one is full
            if (AU_P->getReadsCount() == ACCESS_UNIT_SIZE) {
                AU_P->setEndPosition(tagsReads_it->second.first.mapping_pos[0]);
                u.insertAccessUnit(*AU_P);
                AU_P = new AccessUnit_P(++au_id);
                AU_P->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                firstP = true;
            }

            // erase the read after using it
            reads.erase(reads_it++);
            tags_read.erase(tagsReads_it++);
            u.removeFirstRead();

        }  else if (tagsReads_it->second.first.class_type == 2) {
            // update the number of reads in the access unit
            AU_N->updateReads();

            // get pos descriptor
            if (firstN) {
                firstN = false;
                antPosN = tagsReads_it->second.first.mapping_pos[0];
                AU_N->insertPosdescriptor(0);
                AU_N->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                f.insertPosValue(0, 2);
            } else {
                AU_N->insertPosdescriptor(tagsReads_it->second.first.mapping_pos[0] - antPosN);
                f.insertPosValue(tagsReads_it->second.first.mapping_pos[0] - antPosN, 2);

                antPosN = tagsReads_it->second.first.mapping_pos[0];
            }

            // get rcomp descriptor
            uint8_t rcomp = f.insertRcompValue(reads_it->second.first, reads_it->second.second, 2);
            AU_N->insertRcompDescriptor(rcomp);

            // get flags descriptor
            uint8_t flags = f.insertFlagsValue(reads_it->second.first, 2);
            AU_N->insertFlagsDescriptor(flags);

            // get rlen descriptor
            uint8_t rlen = f.insertRlenValue(reads_it->second.first, 2);
            static_cast<AccessUnit_N*> (AU_N)->insertRlenDescriptor(rlen);

            // get pair descriptor
            uint16_t pair = f.insertPairValue(reads_it->second.first, reads_it->second.second, 2);
            static_cast<AccessUnit_N*> (AU_N)->insertPairDescriptor(pair, reads_it->second.first.rID, u.reads_distance(reads_it->second.first));

            // get mmpos descriptor
            std::vector<std::pair<uint16_t, std::string> > mmpos = u.getMmposValues(reads_it->second.first);
            for (int i = 0; i < mmpos.size(); ++i) {
                static_cast<AccessUnit_N*> (AU_N)->insertMmposDescriptor(mmpos[i].first);
                if (i != (mmpos.size() - 1)) f.insertMmposValue(mmpos[i].first, 2, false);
                else f.insertMmposValue(mmpos[i].first, 2, true);
            }

            // create a new access unit in case if the current one is full
            if (AU_N->getReadsCount() == ACCESS_UNIT_SIZE) {
                // create a new accessUnit
                AU_N->setEndPosition(tagsReads_it->second.first.mapping_pos[0]);
                u.insertAccessUnit(*AU_N);
                AU_N = new AccessUnit_N(++au_id);
                AU_N->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                firstN = true;
            }

            // erase the read after using it
            reads.erase(reads_it++);
            tags_read.erase(tagsReads_it++);
            u.removeFirstRead();

        } else if (tagsReads_it->second.first.class_type == 3) {
            // update the number of reads in the access unit
            AU_M->updateReads();

            // get pos descriptor
            if (firstM) {
                firstM = false;
                antPosM = tagsReads_it->second.first.mapping_pos[0];
                AU_M->insertPosdescriptor(0);
                AU_M->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                f.insertPosValue(0, 3);
            } else {
                AU_M->insertPosdescriptor(tagsReads_it->second.first.mapping_pos[0] - antPosM);
                f.insertPosValue(tagsReads_it->second.first.mapping_pos[0] - antPosM, 3);


                antPosM = tagsReads_it->second.first.mapping_pos[0];
            }

            // get rcomp descriptor
            uint8_t rcomp = f.insertRcompValue(reads_it->second.first, reads_it->second.second, 3);
            AU_M->insertRcompDescriptor(rcomp);

            // get flags descriptor
            uint8_t flags = f.insertFlagsValue(reads_it->second.first, 3);
            AU_M->insertFlagsDescriptor(flags);

            // get mmpos descriptor
            std::vector<std::pair<uint16_t, std::string> > mmpos = u.getMmposValues(reads_it->second.first);
            for (int i = 0; i < mmpos.size(); ++i) {
                static_cast<AccessUnit_M*> (AU_M)->insertMmposDescriptor(mmpos[i].first);
                if (i != (mmpos.size() - 1)) f.insertMmposValue(mmpos[i].first, 3, false);
                else f.insertMmposValue(mmpos[i].first, 3, true);

            }

            // get mmtype descriptor
            std::vector<uint8_t> mmtype = f.insertmmtypeDescriptor(mmpos, 3);

            for (int i = 0; i < mmtype.size(); ++i) static_cast<AccessUnit_M*> (AU_M)->insertMmtypeDescriptor(mmtype[i]);

            // get rlen descriptor
            uint8_t rlen = f.insertRlenValue(reads_it->second.first, 3);
            static_cast<AccessUnit_M*> (AU_M)->insertRlenDescriptor(rlen);

            // get pair descriptor
            uint16_t pair = f.insertPairValue(reads_it->second.first, reads_it->second.second, 3);
            static_cast<AccessUnit_M*> (AU_M)->insertPairDescriptor(pair, reads_it->second.first.rID, u.reads_distance(reads_it->second.first));

            // create a new access unit in case if the current one is full
            if (AU_M->getReadsCount() == ACCESS_UNIT_SIZE) {
                // create a new accessUnit
                AU_M->setEndPosition(tagsReads_it->second.first.mapping_pos[0]);
                u.insertAccessUnit(*AU_M);
                AU_M = new AccessUnit_M(++au_id);
                AU_M->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                firstM = true;
            }

            // erase read after using it
            reads.erase(reads_it++);
            tags_read.erase(tagsReads_it++);
            u.removeFirstRead();

        }
        else if (tagsReads_it->second.first.class_type == 4) {
            // update the number of reads in the access unit
            AU_I->updateReads();

            // get pos descriptor
            if (firstI) {
                firstI = false;
                antPosI = tagsReads_it->second.first.mapping_pos[0];
                AU_I->insertPosdescriptor(0);
                AU_I->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                f.insertPosValue(0, 4);
            } else {
                AU_I->insertPosdescriptor(tagsReads_it->second.first.mapping_pos[0] - antPosI);
                f.insertPosValue(tagsReads_it->second.first.mapping_pos[0] - antPosI, 4);
                antPosI = tagsReads_it->second.first.mapping_pos[0];
            }

            // get rcomp descriptor
            uint8_t rcomp = f.insertRcompValue(reads_it->second.first, reads_it->second.second, 4);
            AU_I->insertRcompDescriptor(rcomp);

            // get flags descriptor
            uint8_t flags = f.insertFlagsValue(reads_it->second.first, 4);
            AU_I->insertFlagsDescriptor(flags);

            // get mmpos descriptor
            std::vector<std::pair<uint16_t, std::string> > mmpos = u.getMmposValues(reads_it->second.first);

            for (int i = 0; i < mmpos.size(); ++i) {
                static_cast<AccessUnit_I*> (AU_I)->insertMmposDescriptor(mmpos[i].first);
                if (i != (mmpos.size() - 1)) f.insertMmposValue(mmpos[i].first, 4, false);
                else f.insertMmposValue(mmpos[i].first, 4, true);
            }

            // get mmtype descriptor
            std::vector<uint8_t> mmtype = f.insertmmtypeDescriptor(mmpos, 4);

            for (int i = 0; i < mmtype.size(); ++i) static_cast<AccessUnit_I*> (AU_I)->insertMmtypeDescriptor(mmtype[i]);

            // get rlen descriptor
            uint8_t rlen = f.insertRlenValue(reads_it->second.first, 4);
            static_cast<AccessUnit_I*> (AU_I)->insertRlenDescriptor(rlen);

            // get pair descriptor
            uint16_t pair = f.insertPairValue(reads_it->second.first, reads_it->second.second, 4);
            static_cast<AccessUnit_I*> (AU_I)->insertPairDescriptor(pair, reads_it->second.first.rID, u.reads_distance(reads_it->second.first));

            // get clips descriptor
            std::string read1_cigar = u.getCigar(reads_it->second.first.cigar);
            std::string read2_cigar = u.getCigar(reads_it->second.second.cigar);

            if (read1_cigar.find('S') != std::string::npos or read2_cigar.find('S') != std::string::npos) {
                std::vector<std::string> clips = f.insertClipsDescriptor(tagsReads_it->second.first, AU_I->getReadsCount() - 1);
            }

            // create a new access unit in case if the current one is full
            if (AU_I->getReadsCount() == ACCESS_UNIT_SIZE) {
                // create a new accessUnit
                AU_I->setEndPosition(tagsReads_it->second.first.mapping_pos[0]);
                u.insertAccessUnit(*AU_I);
                AU_I = new AccessUnit_I(++au_id);
                AU_I->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                firstI = true;
            }

            // erase the read after using it
            reads.erase(reads_it++);
            tags_read.erase(tagsReads_it++);
            u.removeFirstRead();
        } else {
            ++reads_it;
            ++tagsReads_it;
        }
    }

    // add all incomplete access units
    u.insertAccessUnit(*AU_P);
    u.insertAccessUnit(*AU_N);
    u.insertAccessUnit(*AU_M);
    u.insertAccessUnit(*AU_I);
}

int main () {
    std::string filePath = "../../TestFiles/" + fileName + ".bam";
    BamFileIn bamFileIn(toCString(filePath));
    BamHeader header;
    readHeader(header, bamFileIn);

    int useCase = 1; // protection

    int count = 1;
    BamAlignmentRecord record;

    while (!atEnd(bamFileIn) and count <= 100000) { // 8559058 total reads encoded
        readRecord(record, bamFileIn);

        if (record.tLen > 2150 or record.tLen < -2150) {
            u.insertRead(record, record);
        } else if (record.beginPos == record.pNext) {
            u.updateRecord(record, record.pNext);
        } else if (record.beginPos <= record.pNext) {
            u.insertRead(record, record);
        } else {
            u.updateRecord(record, record.pNext);
        }
        ++count;
    }

    // 0 10000 249240518
    // 1 10267 237429082

    if (useCase == 1) {
        insertTagsToReads();

    } else if (useCase == 2) {

    }

    dispatcher();

    std::vector<AccessUnit> au;
    u.getAllAccessUnits(au);

    for (int i = 0; i < au.size(); ++i) {
        au[i].write();
    }
    f.closeFiles();

    return 0;
}