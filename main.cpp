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

/// Access Unit identifier
int au_id = -1;

// Bam file to read
std::string fileName;

// Auxiliary functions
Utils u;

// Number of references sequences with their start and end positions
std::map<int, std::pair<int, int> > references;

// Size of each access unit
int sizeAU = 0;
#define ACCESS_UNIT_SIZE sizeAU

// Reads in mpegg record format with their asociated tags
std::multimap<int, std::pair<MpeggRecord, std::vector<std::string> > > tags_read;

/** @brief Inserts the associated tags to the reads according to the positions
 * @param positions positions in the reference sequences delimitating the tags
 * @param f file manager that allows to write te mpegg record into files
 * */
void insertTagsToReads(std::vector<std::map<int, std::vector<std::string> > >& positions, FileManager& f) {
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
                for(int i = 0; i < position->second.size(); ++i) tags.push_back(position->second[i]);
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
 *  @return void
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

/** @brief Writes the especified regions of the reference sequence to the corresponding access units
 *  @param f file manager that allow to write the encoded Access Units into files
 *  @return void
 **/
void dispatcher(FileManager& f) {
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
                f.writeAccessUnit(*AU_P);
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
            std::vector<std::pair<std::string, uint8_t> > pair = u.getPairDescriptor(reads_it->second.first, reads_it->second.second);

            for (int i = 0; i < pair.size(); ++i) {
                AU_P->insertPairDescriptor(pair[i].first, pair[i].second);
            }

            antTagsP = tagsReads_it->second.second;

            // create a new access unit in case if the current one is full
            if (AU_P->getReadsCount() == ACCESS_UNIT_SIZE) {
                AU_P->setEndPosition(tagsReads_it->second.first.mapping_pos[0]);
                u.insertAccessUnit(*AU_P);
                f.writeAccessUnit(*AU_P);
                AU_P = new AccessUnit_P(++au_id);
                AU_P->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                AU_P->setSequenceID(tagsReads_it->second.first.seq_Id);
                firstP = true;
            }

            // erase the read after using it
            reads.erase(reads_it++);
            tags_read.erase(tagsReads_it++);
            u.removeFirstRead();

        }  else if (tagsReads_it->second.first.class_type == 2) {
            // if we found a new tag region then create new access Unit
            if (antTagsN != tagsReads_it->second.second) {
                AU_N->setEndPosition(antPosN);
                u.insertAccessUnit(*AU_N);
                f.writeAccessUnit(*AU_N);
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
            } else {
                AU_N->insertPosdescriptor(tagsReads_it->second.first.mapping_pos[0] - antPosN);
                antPosN = tagsReads_it->second.first.mapping_pos[0];
            }

            // get rcomp descriptor
            std::pair<uint8_t, uint8_t> rcomp = u.getRcompDescriptor(reads_it->second.first, reads_it->second.second);
            AU_N->insertRcompDescriptor(rcomp.first);
            if (rcomp.second != 255) AU_N->insertRcompDescriptor(rcomp.second);

            // get flags descriptor
            uint8_t flag = u.getFlagDescriptor(reads_it->second.first);
            AU_N->insertFlagsDescriptor(flag);

            // get rlen descriptor
            uint8_t rlen = u.getRlenDescriptor(reads_it->second.first);
            AU_N->insertRlenDescriptor(rlen);

            // get pair descriptor
            std::vector<std::pair<std::string, uint8_t> > pair = u.getPairDescriptor(reads_it->second.first, reads_it->second.second);

            for (int i = 0; i < pair.size(); ++i) {
                AU_N->insertPairDescriptor(pair[i].first, pair[i].second);
            }

            // get mmpos descriptor
            std::vector<std::pair<uint16_t, std::string> > mmpos = u.getMmposValues(reads_it->second.first);
            for (int i = 0; i < mmpos.size(); ++i) {
                AU_N->insertMmposDescriptor(std::to_string(mmpos[i].first));
            }

            antTagsN = tagsReads_it->second.second;

            // create a new access unit in case if the current one is full
            if (AU_N->getReadsCount() == ACCESS_UNIT_SIZE) {
                // create a new accessUnit
                AU_N->setEndPosition(tagsReads_it->second.first.mapping_pos[0]);
                u.insertAccessUnit(*AU_N);
                f.writeAccessUnit(*AU_N);
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
                f.writeAccessUnit(*AU_M);
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
            } else {
                AU_M->insertPosdescriptor(tagsReads_it->second.first.mapping_pos[0] - antPosM);
                antPosM = tagsReads_it->second.first.mapping_pos[0];
            }

            // get rcomp descriptor
            std::pair<uint8_t, uint8_t> rcomp = u.getRcompDescriptor(reads_it->second.first, reads_it->second.second);
            AU_M->insertRcompDescriptor(rcomp.first);
            if (rcomp.second != 255) AU_M->insertRcompDescriptor(rcomp.second);

            // get flags descriptor
            uint8_t flag = u.getFlagDescriptor(reads_it->second.first);
            AU_M->insertFlagsDescriptor(flag);

            // get rlen descriptor
            uint8_t rlen = u.getRlenDescriptor(reads_it->second.first);
            AU_M->insertRlenDescriptor(rlen);

            // get pair descriptor
            std::vector<std::pair<std::string, uint8_t> > pair = u.getPairDescriptor(reads_it->second.first, reads_it->second.second);

            for (int i = 0; i < pair.size(); ++i) {
                AU_M->insertPairDescriptor(pair[i].first, pair[i].second);
            }

            // get mmpos descriptor
            std::vector<std::pair<uint16_t, std::string> > mmpos = u.getMmposValues(reads_it->second.first);
            for (int i = 0; i < mmpos.size(); ++i) AU_M->insertMmposDescriptor(std::to_string(mmpos[i].first));

            // get mmtype descriptor
            std::vector<uint8_t> mmtype = u.getMmtypeDescriptor(mmpos);
            for (int i = 0; i < mmtype.size(); ++i) AU_M->insertMmtypeDescriptor(std::to_string(mmtype[i]));

            antTagsM = tagsReads_it->second.second;

            // create a new access unit in case if the current one is full
            if (AU_M->getReadsCount() == ACCESS_UNIT_SIZE) {
                // create a new accessUnit
                AU_M->setEndPosition(tagsReads_it->second.first.mapping_pos[0]);
                u.insertAccessUnit(*AU_M);
                f.writeAccessUnit(*AU_M);
                AU_M = new AccessUnit_M(++au_id);
                AU_M->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                firstM = true;
            }

            // erase read after using it
            reads.erase(reads_it++);
            tags_read.erase(tagsReads_it++);
            u.removeFirstRead();

        } else if (tagsReads_it->second.first.class_type == 4) {

            // if we found a new tag region then create new access Unit
            if (antTagsI != tagsReads_it->second.second) {
                AU_I->setEndPosition(antPosI);
                u.insertAccessUnit(*AU_I);
                f.writeAccessUnit(*AU_I);
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
            } else {
                AU_I->insertPosdescriptor(tagsReads_it->second.first.mapping_pos[0] - antPosI);
                antPosI = tagsReads_it->second.first.mapping_pos[0];
            }

            // get rcomp descriptor
            std::pair<uint8_t, uint8_t> rcomp = u.getRcompDescriptor(reads_it->second.first, reads_it->second.second);
            AU_I->insertRcompDescriptor(rcomp.first);
            if (rcomp.second != 255) AU_I->insertRcompDescriptor(rcomp.second);

            // get flags descriptor
            uint8_t flag = u.getFlagDescriptor(reads_it->second.first);
            AU_I->insertFlagsDescriptor(flag);

            // get rlen descriptor
            uint8_t rlen = u.getRlenDescriptor(reads_it->second.first);
            AU_I->insertRlenDescriptor(rlen);

            // get pair descriptor
            std::vector<std::pair<std::string, uint8_t> > pair = u.getPairDescriptor(reads_it->second.first, reads_it->second.second);

            for (int i = 0; i < pair.size(); ++i) {
                AU_I->insertPairDescriptor(pair[i].first, pair[i].second);
            }

            // get mmpos descriptor
            std::vector<std::pair<uint16_t, std::string> > mmpos = u.getMmposValues(reads_it->second.first);
            for (int i = 0; i < mmpos.size(); ++i) AU_I->insertMmposDescriptor(std::to_string(mmpos[i].first));

            // get mmtype descriptor
            std::vector<uint8_t> mmtype = u.getMmtypeDescriptor(mmpos);
            for (int i = 0; i < mmtype.size(); ++i) AU_I->insertMmtypeDescriptor(std::to_string(mmtype[i]));

            // get clips descriptor
            std::string read1_cigar = u.getCigar(reads_it->second.first.cigar);
            std::string read2_cigar = u.getCigar(reads_it->second.second.cigar);

            if (read1_cigar.find('S') != std::string::npos or read2_cigar.find('S') != std::string::npos) {
                std::vector<std::pair<std::string, uint8_t> > clips = u.getClipsDescriptor(tagsReads_it->second.first, AU_I->getReadsCount() - 1);
                for (int i = 0; i < clips.size(); ++i) {
                    AU_I->insertSclipsDescriptor(clips[i].first, clips[i].second);
                }
            }

            antTagsI = tagsReads_it->second.second;

            // create a new access unit in case if the current one is full
            if (AU_I->getReadsCount() == ACCESS_UNIT_SIZE) {
                // create a new accessUnit
                AU_I->setEndPosition(tagsReads_it->second.first.mapping_pos[0]);
                u.insertAccessUnit(*AU_I);
                f.writeAccessUnit(*AU_I);
                AU_I = new AccessUnit_I(++au_id);
                AU_I->setStartPosition(tagsReads_it->second.first.mapping_pos[0]);
                firstI = true;
            }

            // erase the read after using it
            reads.erase(reads_it++);
            tags_read.erase(tagsReads_it++);
            u.removeFirstRead();
        }  else {
            ++reads_it;
            ++tagsReads_it;
        }
    }

    // add all incomplete access units
    AU_P->setEndPosition(antPosP);
    u.insertAccessUnit(*AU_P);
    f.writeAccessUnit(*AU_P);

    AU_N->setEndPosition(antPosN);
    u.insertAccessUnit(*AU_N);
    f.writeAccessUnit(*AU_N);

    AU_M->setEndPosition(antPosM);
    u.insertAccessUnit(*AU_M);
    f.writeAccessUnit(*AU_M);

    AU_I->setEndPosition(antPosI);
    u.insertAccessUnit(*AU_I);
    f.writeAccessUnit(*AU_I);
}

/** @brief Print usage message
 * @param void
 * @return void
 * */
void usage_message() {
    std::cout << "A prototype MPEG-G encoder "  << "(https://github.com/Omair95/A_prototype_MPEG-G_encoder)" << std::endl;
    std::cout << "Usage: " << "A_prototype_MPEG_G_encoder " "FILE_PATH" << " " << "ACCESS_UNIT_SIZE" << std::endl;
    std::cout << "FILE_PATH:" << std::endl;
    std::cout << "Full path of the file to be processed" << std::endl;
    std::cout << "ACCESS_UNIT_SIZE: " << std::endl;
    std::cout << "Size of each access unit " << std::endl;
    std::cout << "Contact: Omair95@protonmail.com" << std::endl;
}

/** @brief Main function
 * @param argc number of parameters
 * @param argv parameter passed
 * @return int represents the state of the program
 * */
int main (int argc, char** argv) {

    if (argc != 3) {
        usage_message();
        return 1;
    }

    std::string filePath = argv[1];
    fileName = filePath.substr(filePath.rfind("/") + 1, filePath.rfind(".") - filePath.rfind("/") - 1);

    // Auxiliary class to be used that allows to create and write into files
    FileManager f(fileName);

    std::string au = argv[2];
    sizeAU = atoi(au.c_str());

    // full path of the file to be read
    BamFileIn bamFileIn(toCString(filePath));
    BamHeader header;
    readHeader(header, bamFileIn);

    BamAlignmentRecord record;
    std::cout << "Pairing reads ..." << std::endl;

    // loop that reads the file and pairs the mate reads
    while (!atEnd(bamFileIn)) {
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
        std::cout << "Reference ID = " << it->first << " " << "Start: " << it->second.first << " " << "End: " << it->second.second << std::endl;
    }
    std::cout << std::endl;

    // Use cases are shown
    std::cout << "PROTECTION     : 1 " << std::endl;
    std::cout << "RANDOM ACCESS  : 2 " << std::endl;
    std::cout << "ENCODE         : 3 " << std::endl;
    std::cout << "Enter use case : ";

    int useCase;
    std::vector<std::map<int, std::vector<std::string> > > useCase1Positions(references.size()), useCase2Positions(references.size());
    bool useCase1 = false, useCase2 = false;
    while (std::cin >> useCase) {

        if (useCase == 1) {

            // for each reference sequence write the end position and name of the tag
            for (auto &reference : references) {
                std::cout << "Use case : Protection " << std::endl;
                std::cout << "Enter positions and tags for reference sequence (Insert 0 and END to finish)" << " : " << reference.first << " (" << reference.second.first << " - " << reference.second.second << ")" << std::endl;
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
            // Default use case
            if (not useCase1 and not useCase2) std::cout << "Dispatching and encoding..." << std::endl;

            break;
        }

        if (useCase1) {
            std::cout << "Add use case 2? (Y/N)" << std::endl;
            char c; std::cin >> c;
            std::cout << std::endl;
            if (c == 'Y') {
                std::cout << "Use case: Random access " << std::endl;
                std::cout << "Insert Tags (insert END to finish) : " << std::endl;

                // write the list of tags
                std::string tag;
                std::vector<std::string> tags;
                while (std::cin >> tag and tag != "END") {
                    tags.emplace_back(tag);
                }

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
                break;
            } else break;
        } else if (useCase2) {
            std::cout << "Add use case 1? (Y/N)" << std::endl;
            char c; std::cin >> c;
            if (c == 'Y') {
                // for each reference sequence write the end position and name of the tag
                for (auto &reference : references) {
                    std::cout << "Use case : Protection " << std::endl;
                    std::cout << "Enter positions and tags for reference sequence (Insert 0 and END to finish)" << " : " << reference.first << " (" << reference.second.first << " - " << reference.second.second << ")" << std::endl;
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
                break;
            } else break;
        } else break;
    }

    // if two use cases have been introduced then we have the option to merge the positions
    if (useCase1 and useCase2) {
        std::cout << "Merge use cases? (Y/N) ";
        char c; std::cin >> c;
        std::cout << std::endl;
        if (c == 'Y') {
            std::cout << "Merging tags ..." << std::endl;
            std::vector<std::map<int, std::vector<std::string> > > result(references.size());
            mergeTags(useCase1Positions, useCase2Positions, result);

            std::cout << "Inserting tags ..." << std::endl;
            insertTagsToReads(result, f);
        } else if (c == 'N') {
            std::cout << "Encode use case 1 or 2? ";
            int useCase; std::cin >> useCase;
            std::cout << "Inserting tags ..." << std::endl;
            if (useCase == 1) insertTagsToReads(useCase1Positions, f);
            else if (useCase == 2) insertTagsToReads(useCase2Positions, f);
        }
    } else {
        if (useCase1 or useCase2) std::cout << "Inserting tags ..." << std::endl;
        if (useCase1) {
            insertTagsToReads(useCase1Positions, f);
        }
        else {
            insertTagsToReads(useCase2Positions, f);
        }
    }

    if (useCase1 or useCase2) std::cout << "Dispatching and encoding..." << std::endl;

    // start the dispatcher
    dispatcher(f);

    // close opened files
    f.closeFiles();
    return 0;
}
