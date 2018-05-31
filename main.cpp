#include "AccessUnit_P.h"
#include "AccessUnit_N.h"
#include "AccessUnit_M.h"
#include "AccessUnit_I.h"
#include "FileManager.h"
#include <algorithm>

/**
 * @file main.cpp
 * @Author Omair Iqbal
 * @date 04/2018
 * @class main
 * @brief main class
 * @contact Omair95@protonmail.com
*/

/** Access Unit identifier
 * */
int au_id = -1;

// Bam file to read
std::string fileName = "9827_2#49";

// Auxiliary class to be used that allows to create and write into files
FileManager f(fileName);

// Auxiliary functions
Utils u;

/** Number of references sequences with their start and end positions
 * */
std::map<int, std::pair<int, int> > references;

// Size of each access unit
#define ACCESS_UNIT_SIZE 1000000 // 13716

/** List of reads with their asociated tags
 * */
std::multimap<int, std::pair<MpeggRecord, std::vector<std::string> > > tags_read;

/** @brief Inserts the asociated tags to the reads according to the positions
 * @param positions positions in the reference sequences delimitating the tags
 * */
void insertTagsToReads(std::vector<std::map<int, std::vector<std::string> > >& positions) {
    std::multimap<int, std::pair<BamAlignmentRecord, BamAlignmentRecord> > reads;
    u.getAllreads(reads);

    for (auto &read : reads) {
        MpeggRecord record;
        u.convertToMpeggRecord(record, read.second.first);
        f.writeMpeggToFile(record);
        bool found = false;
        auto position = positions[record.seq_Id].begin();

        while (not found and position != positions[record.seq_Id].end()) {
            if (read.first < position->first) {
                std::vector<std::string> tags;
                tags.push_back(position->second[0]);
                tags_read.insert(std::make_pair(read.first, std::make_pair(record, tags)));
                found = true;
            } else {
                ++position;
            }
        }
        if (not found) {
            std::vector<std::string> tags;
            tags.emplace_back("Public");
            tags_read.insert(std::make_pair(read.first, std::make_pair(record, tags)));
        }
    }
}

/** @brief Merge the tags from the use case 1 and use case 2
 *  @param useCase1 matrix containing the positions for each reference sequence for the use case 1
 *  @param useCase2 matrix containing the positions for each reference sequence for the use case 2
 *  @param result the resulted matrix which contains the merged positions for the both use cases
 **/
void mergeTags(std::vector<std::map<int, std::vector<std::string> > >& useCase1, std::vector<std::map<int, std::vector<std::string> > >& useCase2, std::vector<std::map<int, std::vector<std::string> > >& result) {
    for (int i = 0; i < useCase1.size(); ++i) {
        useCase1[i].insert(useCase2[i].begin(), useCase2[i].end());
        auto it = useCase1[i].begin();
        auto it2 = useCase2[i].begin();

        while (it != useCase1[i].end()) {
            if (it->first <= it2->first) {
                if (it->second[0] != it2->second[0]) {
                    it->second.emplace_back(it2->second[0]);
                }
                ++it;
            } else if (it2->first >= it->first and it2->first <= it++->first) {
                ++it;
            } else {
                ++it2;
            }
        }
    }
    result = useCase1;
}

/** @brief Writes the especified regions of the reference sequence to the corresponding access units which are written into files
 **/
void dispatcher() {
    AccessUnit *AU_P, *AU_N, *AU_M, *AU_I;
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
    std::vector<std::string> antTagsP, antTagsN, antTagsM, antTagsI;
    antTagsP = antTagsN = antTagsM = antTagsI = tagsReads_it->second.second;

    while (tagsReads_it != end) {

        if (tagsReads_it->second.first.class_type == 1) {
            // if we found a new tag region then create new access Unit
            if (antTagsP != tagsReads_it->second.second) {
                AU_P->setEndPosition(antPosP);
                AU_P->setSequenceID(tagsReads_it->second.first.seq_Id);
                u.insertAccessUnit(*AU_P);
                AU_P = new AccessUnit_P(++au_id);
                AU_P->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                firstP = true;
            }

            // update the number of reads in the access unit
            AU_P->updateReads();

            // get pos descriptor
            if (firstP) {
                firstP = false;
                antPosP = tagsReads_it->second.first.mapping_pos[0];
                AU_P->insertPosdescriptor(0);
                AU_P->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
            } else {
                AU_P->insertPosdescriptor(tagsReads_it->second.first.mapping_pos[0] - antPosP);
                antPosP = tagsReads_it->second.first.mapping_pos[0];
            }

            // get rcomp descriptor
            std::pair<uint8_t, uint8_t> rcomp = u.getRcompDescriptor(reads_it->second.first, reads_it->second.second);
            AU_P->insertRcompDescriptor(rcomp.first);
            if (rcomp.second != 255) AU_P->insertRcompDescriptor(rcomp.second);

            // get flags descriptor
            uint8_t flag = u.getFlagDescriptor(reads_it->second.first);
            AU_P->insertFlagsDescriptor(flag);

            // get rlen descriptor
            uint8_t rlen = u.getRlenDescriptor(reads_it->second.first);
            AU_P->insertRlenDescriptor(rlen);

            // get pair descriptor
            std::string pair = u.getPairDescriptor(reads_it->second.first, reads_it->second.second);
            AU_P->insertPairDescriptor(pair);

            antTagsP = tagsReads_it->second.second;

            // create a new access unit in case if the current one is full
            if (AU_P->getReadsCount() == ACCESS_UNIT_SIZE) {
                AU_P->setEndPosition(tagsReads_it->second.first.mapping_pos[0]);
                u.insertAccessUnit(*AU_P);
                AU_P = new AccessUnit_P(++au_id);
                AU_P->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                AU_P->setSequenceID(tagsReads_it->second.first.seq_Id);
                firstP = true;
            }

            // erase the read after using it
            reads.erase(reads_it++);
            tags_read.erase(tagsReads_it++);
            u.removeFirstRead();

        }  /* else if (tagsReads_it->second.first.class_type == 2) {
            // if we found a new tag region then create new access Unit
            if (antTagsN != tagsReads_it->second.second) {
                AU_N->setEndPosition(antPosN);
                u.insertAccessUnit(*AU_N);
                AU_N = new AccessUnit_N(++au_id);
                AU_N->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                AU_N->setSequenceID(tagsReads_it->second.first.seq_Id);
                firstN = true;
            }

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

            antTagsN = tagsReads_it->second.second;

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

            // if we found a new tag region then create new access Unit
            if (antTagsM != tagsReads_it->second.second) {
                AU_M->setEndPosition(antPosM);
                u.insertAccessUnit(*AU_M);
                AU_M = new AccessUnit_M(++au_id);
                AU_M->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                AU_M->setSequenceID(tagsReads_it->second.first.seq_Id);
                firstM = true;
            }

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

            antTagsM = tagsReads_it->second.second;

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

            // if we found a new tag region then create new access Unit
            if (antTagsI != tagsReads_it->second.second) {
                AU_I->setEndPosition(antPosI);
                u.insertAccessUnit(*AU_I);
                AU_I = new AccessUnit_I(++au_id);
                AU_I->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                AU_I->setSequenceID(tagsReads_it->second.first.seq_Id);
                firstI = true;
            }

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
                static_cast<AccessUnit_I*> (AU_I)->insertSclipsDescriptor(clips);
            }

            antTagsI = tagsReads_it->second.second;

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
        } */ else {
            ++reads_it;
            ++tagsReads_it;
        }
    }

    // add all incomplete access units
    AU_P->setEndPosition(antPosP);
    u.insertAccessUnit(*AU_P);

    AU_N->setEndPosition(antPosN);
    u.insertAccessUnit(*AU_N);

    AU_M->setEndPosition(antPosM);
    u.insertAccessUnit(*AU_M);

    AU_I->setEndPosition(antPosI);
    u.insertAccessUnit(*AU_I);
}

int main () {
    // full path of the file to be read
    std::string filePath = "../../TestFiles/" + fileName + ".bam";
    BamFileIn bamFileIn(toCString(filePath));
    BamHeader header;
    readHeader(header, bamFileIn);

    BamAlignmentRecord record;
    std::cout << "Pairing reads ... " << std::endl;

    // loop that reads the file and pairs the mate reads
    int count = 1;
    while (!atEnd(bamFileIn) and count <= 10000) {
        readRecord(record, bamFileIn);

        if (references.find(record.rID) == references.end()) {
            references.insert(std::make_pair(record.rID, std::make_pair(record.beginPos, record.beginPos)));
        } else {
            auto it = references.find(record.rID);
            int begin = it->second.first;
            references.erase(record.rID);
            references.insert(std::make_pair(record.rID, std::make_pair(begin, record.beginPos)));
        }

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

    // reference sequence found with their start and end positions
    for (auto it = references.begin(); it != references.end(); it++) {
        std::cout << "Reference ID = " << it->first << " " << "Start : " << it->second.first << " " << "End : " << it->second.second << std::endl;
    }
    std::cout << std::endl;

    // Use cases are shown
    std::cout << "PROTECTION    : 1 " << std::endl;
    std::cout << "RANDOM ACCESS : 2 " << std::endl;
    std::cout << "NONE          : 3 " << std::endl;
    std::cout << "Enter use case : ";

    int useCase;
    std::vector<std::map<int, std::vector<std::string> > > useCase1Positions(references.size()), useCase2Positions(references.size());
    bool useCase1 = false, useCase2 = false;
    while (std::cin >> useCase and useCase != 3) {

        if (useCase == 1) {

            // for each reference sequence write the end position and name of the tag
            for (auto &reference : references) {
                std::cout << "Use case : Protection " << std::endl;
                std::cout << "Enter positions and tags for reference sequence" << " : " << reference.first << " (" << reference.second.first << " - " << reference.second.second << ")" << std::endl;
                std::cout << "Insert 0 and END to finish" << std::endl;
                int pos = -1;
                std::string tag;
                while (std::cin >> pos >> tag and pos != 0 and tag != "END") {
                    if (tag == "END") std::cout << "Calculating positions ... " << std::endl;
                    std::vector<std::string> tags;
                    tags.emplace_back(tag);
                    useCase1Positions[reference.first].insert(std::make_pair(pos, tags));
                }
            }
            useCase1 = true;
            std::cout << std::endl;
        } else if (useCase == 2) {
            std::cout << "Use case: Random access " << std::endl;
            std::cout << "Insert Tags (insert END to finish) : " << std::endl;

            // write the list of tags
            std::string tag;
            std::vector<std::string> tags;
            while (std::cin >> tag and tag != "END") {
                tags.emplace_back(tag);
            }

            std::cout << "Calculating positions ... " << std::endl;

            int j = 0;
            for (auto &reference : references) {
                for (int i = reference.second.first; i < reference.second.second; ++i) {
                    if (i == (((reference.second.second - reference.second.first) / tags.size()) * (j+1)) +
                                     reference.second.first){
                        std::vector<std::string> singleTag;
                        singleTag.emplace_back(tags[j]);
                        useCase2Positions[reference.first].insert(std::make_pair(i, singleTag));
                        ++j;
                    }
                }
            }
            useCase2 = true;
            std::cout << std::endl;
        } else if (useCase == 3) {
            // end the use cases loop
            break;
        }

        std::cout << "PROTECTION    : 1 " << std::endl;
        std::cout << "RANDOM ACCESS : 2 " << std::endl;
        std::cout << "NONE          : 3 " << std::endl;
        std::cout << "Enter use case : ";
    }

    // if two use cases have been introduced then we have the option to merge the positions
    if (useCase1 and useCase2) {
        std::cout << "Merge use cases? Y/N ";
        char c; std::cin >> c;
        if (c == 'Y') {
            std::cout << "Merging tags ..." << std::endl;
            std::vector<std::map<int, std::vector<std::string> > > result(references.size());
            mergeTags(useCase1Positions, useCase2Positions, result);

            std::cout << "Inserting tags ..." << std::endl;
            insertTagsToReads(result);
        } else if (c == 'N') {
            std::cout << "Encode use case 1 or 2? ";
            int useCase; std::cin >> useCase;
            std::cout << "Inserting tags ..." << std::endl;
            if (useCase == 1) insertTagsToReads(useCase1Positions);
            else if (useCase == 2) insertTagsToReads(useCase2Positions);
        }
    } else {
        if (useCase1 or useCase2) std::cout << "Inserting tags ..." << std::endl;
        if (useCase1) {
            insertTagsToReads(useCase1Positions);
        }
        else {
            insertTagsToReads(useCase2Positions);
        }
    }

    std::cout << "Dispatching and encoding..." << std::endl;

    // start the dispatcher
    dispatcher();

    // write via command line the access units encoded without the payload
    std::cout << "List of access Units encoded" << std::endl;

    f.writeAccessUnits(u);
    f.closeFiles();

    return 0;
}