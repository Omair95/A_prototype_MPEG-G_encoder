#include "Utils.h"
#include "AccessUnit_P.h"
#include "AccessUnit_N.h"
#include "AccessUnit_M.h"
#include "AccessUnit_I.h"
#include "AccessUnit_HM.h"
#include "AccessUnit_U.h"
#include "FileManager.h"
#include <time.h>
#include <algorithm>

/*! \file */

Utils u;
FileManager f;
int au_id = -1;

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
        u.convertToMpeggRecord(a, it->second.first);
        f.writeMpeggToFile(a);

        if (a.class_type == 1) {
            // update the number of reads in the access unit
            AU_P->updateReads();

            // get pos descriptor
            if (firstP) {
                firstP = false;
                antPosP = a.mapping_pos[0];
                AU_P->insertPosdescriptor(u.int_to_hex(0));
                AU_P->setStartPosition(a.mapping_pos[0]);
                f.insertPosValue(u.int_to_hex(0), 1);
            } else {
                AU_P->insertPosdescriptor(u.int_to_hex(a.mapping_pos[0] - antPosP));
                f.insertPosValue(u.int_to_hex(a.mapping_pos[0] - antPosP), 1);
                antPosP = a.mapping_pos[0];
            }

            // get rcomp descriptor
            std::string rcomp = u.getRcompDescriptor(it->second.first);
            AU_P->insertRcompDescriptor(rcomp);
            f.insertRcompValue(rcomp, 1);

            // get flags descriptor
            std::string flags = u.getFlagDescriptor(it->second.first);
            AU_P->insertFlagsDescriptor(flags);
            f.insertFlagsValue(flags, 1);

            // get rlen descriptor
            std::string rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_P*> (AU_P)->insertRlenDescriptor(rlen);
            f.insertRlenValue(rlen, 1);

            // get pair descriptor
            std::string pair = u.getPairDescriptor(it->second.first);
            static_cast<AccessUnit_P*> (AU_P)->insertPairDescriptor(pair);
            f.insertPairValue(pair, 1);

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
                f.insertPosValue(u.int_to_hex(0), 2);
            } else {
                AU_N->insertPosdescriptor(u.int_to_hex(a.mapping_pos[0] - antPosN));
                f.insertPosValue(u.int_to_hex(a.mapping_pos[0] - antPosN), 2);
                antPosN = a.mapping_pos[0];
            }

            // get rcomp descriptor
            std::string rcomp = u.getRcompDescriptor(it->second.first);
            AU_N->insertRcompDescriptor(rcomp);
            f.insertRcompValue(rcomp, 2);

            // get flags descriptor
            std::string flags = u.getFlagDescriptor(it->second.first);
            AU_N->insertFlagsDescriptor(flags);
            f.insertFlagsValue(flags, 2);

            // get mmpos descriptor
            std::vector<std::pair<int, std::string> > mmpos = u.getmmposDescriptor(it->second.first);

            for (int i = 0; i < mmpos.size(); ++i) {
                static_cast<AccessUnit_N*> (AU_N)->insertMmposDescriptor(u.int_to_hex(mmpos[i].first));
                f.insertMmposValue(u.int_to_hex(mmpos[i].first), 2);
            }

            // get rlen descriptor
            std::string rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_N*> (AU_N)->insertRlenDescriptor(rlen);
            f.insertRlenValue(rlen, 2);

            // get pair descriptor
            std::string pair = u.getPairDescriptor(it->second.first);
            static_cast<AccessUnit_N*> (AU_N)->insertPairDescriptor(pair);
            f.insertPairValue(pair, 2);

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
                f.insertPosValue(u.int_to_hex(0), 3);
            } else {
                AU_M->insertPosdescriptor(u.int_to_hex(a.mapping_pos[0] - antPosM));
                f.insertPosValue(u.int_to_hex(a.mapping_pos[0] - antPosM), 3);
                antPosM = a.mapping_pos[0];
            }

            // get rcomp descriptor
            std::string rcomp = u.getRcompDescriptor(it->second.first);
            AU_M->insertRcompDescriptor(rcomp);
            f.insertRcompValue(rcomp, 3);

            // get flags descriptor
            std::string flags = u.getFlagDescriptor(it->second.first);
            AU_M->insertFlagsDescriptor(flags);
            f.insertFlagsValue(flags, 3);

            // get mmpos descriptor
            std::vector<std::pair<int, std::string> > mmpos = u.getmmposDescriptor(it->second.first);

            for (int i = 0; i < mmpos.size(); ++i) {
                static_cast<AccessUnit_M*> (AU_M)->insertMmposDescriptor(u.int_to_hex(mmpos[i].first));
                f.insertMmposValue(u.int_to_hex(mmpos[i].first), 3);
            }

            // get mmtype descriptor
            std::vector<std::string> mmtype = u.getmmtypeDescriptor(mmpos);

            for (int i = 0; i < mmtype.size(); ++i) {
                static_cast<AccessUnit_M*> (AU_M)->insertMmtypeDescriptor(mmtype[i]);
                f.insertMmtypeValue(mmtype[i], 3);
            }

            // get rlen descriptor
            std::string rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_M*> (AU_M)->insertRlenDescriptor(rlen);
            f.insertRlenValue(rlen, 3);

            // get pair descriptor
            std::string pair = u.getPairDescriptor(it->second.first);
            static_cast<AccessUnit_M*> (AU_M)->insertPairDescriptor(pair);
            f.insertPairValue(pair, 3);

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
                f.insertPosValue(u.int_to_hex(0), 4);
            } else {
                AU_I->insertPosdescriptor(u.int_to_hex(a.mapping_pos[0] - antPosI));
                f.insertPosValue(u.int_to_hex(a.mapping_pos[0] - antPosI), 4);
                antPosI = a.mapping_pos[0];
            }

            // get rcomp descriptor
            std::string rcomp = u.getRcompDescriptor(it->second.first);
            AU_I->insertRcompDescriptor(rcomp);
            f.insertRcompValue(rcomp, 4);

            // get flags descriptor
            std::string flags = u.getFlagDescriptor(it->second.first);
            AU_I->insertFlagsDescriptor(flags);
            f.insertFlagsValue(flags, 4);

            // get mmpos descriptor
            std::vector<std::pair<int, std::string> > mmpos = u.getmmposDescriptor(it->second.first);

            for (int i = 0; i < mmpos.size(); ++i) {
                static_cast<AccessUnit_I*> (AU_I)->insertMmposDescriptor(u.int_to_hex(mmpos[i].first));
                f.insertMmposValue(u.int_to_hex(mmpos[i].first), 4);
            }

            // get mmtype descriptor
            std::vector<std::string> mmtype = u.getmmtypeDescriptor(mmpos);

            for (int i = 0; i < mmtype.size(); ++i) {
                static_cast<AccessUnit_I*> (AU_I)->insertMmtypeDescriptor(mmtype[i]);
                f.insertMmtypeValue(mmtype[i], 4);
            }

            // get rlen descriptor
            std::string rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_I*> (AU_I)->insertRlenDescriptor(rlen);
            f.insertRlenValue(rlen, 4);

            // get pair descriptor
            std::string pair = u.getPairDescriptor(it->second.first);
            static_cast<AccessUnit_I*> (AU_I)->insertPairDescriptor(pair);
            f.insertPairValue(pair, 4);

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
                f.insertPosValue(u.int_to_hex(0), 5);
            } else {
                AU_HM->insertPosdescriptor(u.int_to_hex(a.mapping_pos[0] - antPosHM));
                f.insertPosValue(u.int_to_hex(a.mapping_pos[0] - antPosHM), 5);
                antPosHM = a.mapping_pos[0];
            }

            // get rcomp descriptor
            std::string rcomp = u.getRcompDescriptor(it->second.first);
            AU_HM->insertRcompDescriptor(rcomp);
            f.insertRcompValue(rcomp, 5);

            // get flags descriptor
            std::string flags = u.getFlagDescriptor(it->second.first);
            AU_HM->insertFlagsDescriptor(flags);
            f.insertFlagsValue(flags, 5);

            // get rlen descriptor
            std::string rlen = u.getRlenDescriptor(it->second.first);
            static_cast<AccessUnit_HM*> (AU_HM)->insertRlenDescriptor(rlen);
            f.insertRlenValue(rlen, 5);

            // get pair descriptor
            std::string pair = u.getPairDescriptor(it->second.first);
            static_cast<AccessUnit_HM*> (AU_HM)->insertPairDescriptor(pair);
            f.insertPairValue(pair, 5);

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
            f.insertRlenValue(rlen, 6);

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

    generateByteStream();

    std::cout << accessUnits.size() << std::endl;

    for (int i = 0; i < accessUnits.size(); ++i) {
        accessUnits[i].write();
    }

    f.closeFiles();

    return 0;
}

// 23020 bam.pos