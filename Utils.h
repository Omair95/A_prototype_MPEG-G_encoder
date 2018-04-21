#include "AccessUnit.h"
#include <sstream>
#include <ctype.h>
#include <fstream>
#include "MpeggRecord.h"

/** \class Utils class
 *  \brief Auxiliary class that contains useful methods from the main class
 *  \author Omair Iqbal
 * */

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_UTILS_H
#define A_PROTOTYPE_MPEG_G_ENCODER_UTILS_H

std::multimap<int, std::pair<BamAlignmentRecord, BamAlignmentRecord> > reads;
std::vector<AccessUnit> accessUnits;
uint64_t record_id = 0;

class Utils {

public:

    /** \brief Convert from a string<cigarElement> to a std::string
     *  \param The string<cigarElement> to be converted
     *  \return The converted std::string
     * */
    std::string getCigar(String <CigarElement<> >& rCigar) {
        std::string result;
        int size = length(rCigar);
        int count = 0;
        while (count < size)
        {
            CigarElement<> b = rCigar[count];
            int c = b.count;
            char d = b.operation;
            result += std::to_string(c);
            result += d;
            ++count;
        }
        return result;
    }

    /** \brief Gets the extended cigar of the read
     * @param A read
     * @return The extended cigar
     */
    std::string getExtendedCigar(BamAlignmentRecord& record) {
        std::string cigar = getCigar(record.cigar);
        std::string MD = getMDtag(record);
        std::string result;
        std::map<int, std::string> mmpos;
        int sum = 0;

        if (cigar != "100M") {
            for (int i = 0; i < cigar.size(); ++i) {
                if (not isdigit(cigar[i])) {
                    int a = 0, b = 0;
                    if ((i - 2) >= 0 and isdigit(cigar[i - 2]) and isdigit(cigar[i - 1])) {
                        a = cigar[i - 2] - '0';
                        b = cigar[i - 1] - '0';
                    } else if (isdigit(cigar[i - 1])) {
                        b = cigar[i - 1] - '0';
                    }
                    if (cigar[i] != 'D') sum += a * 10 + b;
                    if (cigar[i] != 'M') {
                        std::string c;
                        c += std::to_string(a * 10 + b);
                        c += cigar[i];
                        mmpos.insert(std::make_pair(sum, c));
                    }
                }
            }
        }
        sum = 0;

        if (MD != "100") {
            for (int i = 0; i < MD.size(); ++i) {
                if (not isdigit(MD[i])) {
                    int a = 0, b = 0;
                    if ((i - 2) >= 0 and isdigit(MD[i - 2]) and isdigit(MD[i - 1])) {
                        a = MD[i - 2] - '0';
                        b = MD[i - 1] - '0';
                    } else if (isdigit(MD[i - 1])) {
                        b = MD[i - 1] - '0';
                    }
                    if (MD[i] != 'D') sum += a*10 + b;
                    if (MD[i] != 'M' and MD[i] != '^') {
                        ++sum;
                        std::string c;
                        c += std::to_string(sum);
                        c += MD[i];
                        mmpos.insert(std::make_pair(sum, c));
                    }
                }
            }
        }

        if (cigar == "100M" and MD == "100") return "100=";

        int ant = 0;
        auto it = mmpos.begin();
        auto last = mmpos.end();
        --last;
        while (it != mmpos.end()) {
            if (it->second[it->second.size() - 1] == 'S') {
                if (ant == 0 and it == last) {
                    int a = it->second[0] - '0', b = it->second[1] - '0';
                    result += std::to_string(100 - a*10 - b);
                    result += '=';
                }
                if (ant != 0) {
                    int a = it->second[0] - '0';
                    if (isdigit(it->second[1])) {
                        int b = it->second[1] - '0';
                        result += std::to_string(it->first - ant - (a * 10 + b));
                    }
                    else result += std::to_string(it->first - ant - a);
                    result += '=';
                }
                int a = it->second[0] - '0';
                if (isdigit(it->second[1])) {
                    int b = it->second[1] - '0';
                    result += "(" + std::to_string(a*10 + b) + ")";
                } else result += "(" + std::to_string(a) + ")";
                ant = it->first;
            } else if (it->second[it->second.size() - 1] == 'I' or
                       it->second[it->second.size() - 1] == 'D') {
                int a = 0, b = 0;
                a = it->second[0] - '0';
                if (isdigit(it->second[1])) {
                    b = it->second[1] - '0';
                    result += std::to_string(it->first - ant - (a*10 + b));
                    result += '=';
                    if (it->second[it->second.size() - 1] == 'I') result += std::to_string(a*10 + b) + '+';
                    else result += std::to_string(a*10 + b) + '-';
                } else {
                    result += std::to_string(it->first - ant - a);
                    result += '=';
                    if (it->second[it->second.size() - 1] == 'I') result += std::to_string(a) + '+';
                    else result += std::to_string(a) + '-';
                }
                ant = it->first;
            } else {
                int d = it->first - ant - 1;
                if (d != 0) {
                    result += std::to_string(it->first - ant - 1);
                    result += '=';
                }
                result += it->second[it->second.size() - 1];
                ant = it->first;
            }
            ++it;
        }

        if (last->first != 100) {
            result += std::to_string(100 - last->first);
            result += '=';
        }

        return result;
    }

    /** \brief Gets the second read for the pair
     *  \param The first read of the pair
     *  \return The second read
     * */
    BamAlignmentRecord getSecondRecord(BamAlignmentRecord& record) {
        typedef std::multimap<int, std::pair<BamAlignmentRecord, BamAlignmentRecord> >::iterator it;
        std::pair<it, it> ret;
        ret = reads.equal_range(record.beginPos);
        std::string s = toCString(record.qName);

        for (auto it = ret.first; it != ret.second; ++it) {
            if (!s.compare(toCString(it->second.first.qName))) {
                return it->second.second;
            }
        }
    }

    /** \brief Updates the second read from the pair with a new value
     *  \param Second read from the pair
     * */
    void updateRecord(BamAlignmentRecord& record, int& pos) {
        typedef std::multimap<int, std::pair<BamAlignmentRecord, BamAlignmentRecord> >::iterator it;
        std::pair<it, it> ret;
        ret = reads.equal_range(pos);
        std::string s = toCString(record.qName);

        for (auto it = ret.first; it != ret.second; ++it) {
            if (!s.compare(toCString(it->second.first.qName))) {
                BamAlignmentRecord first = it->second.first;
                reads.erase(it);
                reads.insert(std::make_pair(pos, std::make_pair(first, record)));
                break;
            }
        }
    }

    /** \brief To determine if the read belongs to class P, reads
     *         perfectly matching the reference sequence
     *  \param The first read
     *  \return boolean indicating if the read belongs to class P
     * */
    bool isClassP(BamAlignmentRecord& record) {
        String<CigarElement<> > a = record.cigar;
        std::string firstCigar = getCigar(a);
        BamTagsDict tagsDict(record.tags);
        unsigned tagIdx = 0;
        if (!findTagKey(tagIdx, tagsDict, "MD")) std::cerr << "ERROR: Unknown key!" << std::endl;

        CharString tagValFirst = 0;
        if (!extractTagValue(tagValFirst, tagsDict, tagIdx)) std::cerr << "ERROR: There was an error extracting MD from tags!" << std::endl;

        if (record.flag & 8) {
            if (firstCigar == "100M" and tagValFirst == "100") return true;
        } else {
            BamAlignmentRecord second = getSecondRecord(record);
            String<CigarElement<> > b = second.cigar;
            std::string secondCigar = getCigar(b);

            BamTagsDict tagsDict(second.tags);
            unsigned tagIdx = 0;
            if (!findTagKey(tagIdx, tagsDict, "MD")) std::cerr << "ERROR: Unknown key!\n";

            CharString tagValSecond = 0;
            if (!extractTagValue(tagValSecond, tagsDict, tagIdx))
                std::cerr << "ERROR: There was an error extracting MD from tags!\n";

            if (firstCigar == "100M" and secondCigar == "100M"
                and tagValFirst == "100" and tagValSecond == "100") return true;
        }
        return false;
    }

    /** \brief To determine if the read belongs to class N, reads
     *         containing mismatches which are unknonwn bases only
     *  \param The first read
     *  \return boolean indicating if the read belongs to class N
     * */
    bool isClassN(BamAlignmentRecord& record) {
        CharString seq = record.seq;
        const char *s1 = toCString(seq);
        std::string str1(s1);
        std::string firstCigar = getCigar(record.cigar);
        int tagValFirst = getNMtag(record);
        auto nFirst = std::count(str1.begin(), str1.end(), 'N');

        if (record.flag & 8) {
            if (str1.find("N") != std::string::npos and nFirst == tagValFirst and firstCigar == "100M") return true;
        } else {
            BamAlignmentRecord second = getSecondRecord(record);
            CharString SecondSeq = second.seq;
            const char *s2 = toCString(SecondSeq);
            std::string str2(s2);
            std::string secondCigar = getCigar(second.cigar);
            int tagValSecond = getNMtag(second);
            auto nSecond = std::count(str2.begin(), str2.end(), 'N');

            if (firstCigar == "100M" and nFirst == tagValFirst and secondCigar == "100M" and
                    nSecond == tagValSecond) {
                return true;
            }
        }
        return false;
    }

    /** \brief To determine if the read belongs to class M, reads contanining
     *         at least one substitution, optionally unknown bases and no
     *         insertion, deletions and clipped bases
     *  \param The first read
     *  \return boolean indicating if the read belongs to class M
     * */
    bool isClassM(BamAlignmentRecord& record) {
        CharString seq = record.seq;
        const char *s1 = toCString(seq);
        std::string str1(s1);
        std::string firstCigar = getCigar(record.cigar);
        int tagValFirst = getNMtag(record);
        auto nFirst = std::count(str1.begin(), str1.end(), 'N');

        if (record.flag & 8) {
            if (firstCigar == "100M" and tagValFirst != nFirst) return true;
        } else {
            BamAlignmentRecord second = getSecondRecord(record);
            CharString SecondSeq = second.seq;
            const char *s2 = toCString(SecondSeq);
            std::string str2(s2);
            std::string secondCigar = getCigar(second.cigar);
            int tagValSecond = getNMtag(second);
            auto nSecond = std::count(str2.begin(), str2.end(), 'N');

            if (firstCigar == "100M" and secondCigar == "100M" and (tagValFirst != nFirst or
                    tagValSecond != nSecond)) return true;
        }
        return false;
    }

    /** \brief To determine if the read belongs to class I, reads contanining
     *         at least one insertion, deletion or clippled base, and
     *         optionally unknown bases or substitution
     *  \param The first read
     *  \return boolean indicating if the read belongs to class I
     * */
    bool isClassI(BamAlignmentRecord& record) {
        std::string firstCigar = getCigar(record.cigar);;

        if (record.flag & 8) {
            if (firstCigar != "100M") return true;
        } else {
            BamAlignmentRecord second = getSecondRecord(record);
            std::string secondCigar = getCigar(second.cigar);
            if (firstCigar != "100M" or secondCigar != "100M") return true;
        }
        return false;
    }

    /** \brief To determine if the read belongs to class HM, reads where only
     *         one read is mapped
     *  \param The first read
     *  \return boolean indicating if the read belongs to class HM
     * */
    bool isClassHM(BamAlignmentRecord& record) {
        if (record.flag & 128 and record.flag & 1) return true;
        return false;
    }

    /** \brief To determine if the read belongs to class HM, unmapped reads only
     *  \param The first read
     *  \return boolean indicating if the read belongs to class U
     * */
    bool isClassU(BamAlignmentRecord& record) {
        if (record.flag & 4) return true;
        return false;
    }

    uint8_t getClassType(BamAlignmentRecord& record) {
        if (isClassP(record)) return 1;
        else if (isClassN(record)) return 2;
        else if (isClassM(record)) return 3;
        else if (isClassI(record)) return 4;
        else if (isClassHM(record)) return 5;
        else if (isClassU(record)) return 6;
    }

    /** \brief Gets the rcomp descriptor for the read
     *  \param The first read
     *  \return The corresponding int value
     * */
    std::string getRcompDescriptor(BamAlignmentRecord& record) {
        int result;
        if (record.flag & 8) {
            if (record.flag & 16) result = 0;
            else result = 1;
        } else {
            bool pair1 = record.flag & 16;
            bool pair2 = record.flag & 32;

            if (pair1 == 0 and pair2 == 0) result = 0;
            else if (pair1 == 0 and pair2 == 1) result = 1;
            else if (pair1 == 1 and pair2 == 0) result = 2;
            else if (pair1 == 1 and pair2 == 1) result = 3;
        }
        return int_to_hex(result);
    }

    /** \brief Gets the flag descriptor for the read
     *  \param The first read
     *  \return The corresponding int value
     * */
    std::string getFlagDescriptor(BamAlignmentRecord& record) {
        int flag = record.flag;
        int result = 0;
        if (flag & 1024) result = 1;
        if (flag & 512) result += 2;
        if (not(flag & 8)) result += 4;
        return int_to_hex(result);
    }

    /** \brief Gets the mmpos descriptor for the read
    *  \param The first read
    *  \return List of mismatch positions
    * */
    std::vector<std::pair<int, std::string> > getmmposDescriptor(BamAlignmentRecord& record) {
        std::vector<std::pair<int, std::string> > mmpos;
        std::map<int, std::string> mmposread;
        CharString seq = record.seq;
        const char *s1 = toCString(seq);
        std::string str1(s1);
        std::string cigarRead1 = getCigar(record.cigar);
        std::string MD = getMDtag(record);
        int sum = 0, lastMismatch = 0;

        if (cigarRead1 != "100M") {
            for (int i = 0; i < cigarRead1.size(); ++i) {
                if (not isdigit(cigarRead1[i])) {
                    int a = 0, b = 0;
                    if ((i-2) >= 0 and isdigit(cigarRead1[i-2])) {
                        a = cigarRead1[i-2] - '0';
                        b = cigarRead1[i-1] - '0';
                    } else {
                        b = cigarRead1[i-1] - '0';
                    }
                    sum += a*10 + b;

                    if (cigarRead1[i] == 'I' or cigarRead1[i] == 'D') {
                        while (b > 0) {
                            --b;
                            if (cigarRead1[i] == 'I') {
                                char c = str1[sum - 1];
                                std::string a = "I";
                                a.insert(a.size(), 1, c);
                                mmposread.insert(std::make_pair(sum - b, a));
                            } else mmposread.insert(std::make_pair(sum - b, std::string(1, cigarRead1[i])));
                        }
                        if (sum >= lastMismatch) lastMismatch = sum;
                    }
                }
            }
        }

        sum = 0;

        if (MD != "100") {
            for (int i = 0; i < MD.size(); ++i) {
                if (not isdigit(MD[i])) {
                    int a = 0, b = 0;
                    if ((i-2) >= 0 and isdigit(MD[i-2]) and isdigit(MD[i-1])) {
                        a = MD[i-2] - '0';
                        b = MD[i-1] - '0';
                    } else if (isdigit(MD[i-1])) {
                        b = MD[i-1] - '0';
                    }
                    sum += a*10 + b;
                    ++sum;
                    if (sum >= lastMismatch) lastMismatch = sum;
                    mmposread.insert(std::make_pair(sum, std::string(1, MD[i])));
                }
            }
        }

        for (auto it = mmposread.begin(); it != mmposread.end(); ++it) {
            mmpos.push_back(std::make_pair(it->first, it->second));
        }
        mmposread.clear();

        if (record.flag & 8) return mmpos;

        BamAlignmentRecord second = getSecondRecord(record);
        CharString seq2 = second.seq;
        const char *s2 = toCString(seq2);
        std::string str2(s2);
        std::string cigarRead2 = getCigar(second.cigar);
        std::string MD2 = getMDtag(second);
        sum = 0;
        if (cigarRead2 != "100M") {
            for (int i = 0; i < cigarRead2.size(); ++i) {
                if (not isdigit(cigarRead2[i])) {
                    int a = 0, b = 0;
                    if ((i-2) >= 0 and isdigit(cigarRead2[i-2])) {
                        a = cigarRead2[i-2] - '0';
                        b = cigarRead2[i-1] - '0';
                    } else {
                        b = cigarRead2[i-1] - '0';
                    }
                    sum += a*10 + b;

                    if (cigarRead2[i] == 'I' or cigarRead2[i] == 'D') {
                        while (b > 0) {
                            --b;
                            if (cigarRead2[i] == 'I') {
                                char c = str2[sum - 1];
                                std::string a = "I";
                                a.insert(a.size(), 1, c);
                                mmposread.insert(std::make_pair(sum - b, a));
                            } else mmposread.insert(std::make_pair(sum - b, std::string(1, cigarRead2[i])));
                        }
                    }
                }
            }
        }

        sum = 0;

        if (MD2 != "100") {
            for (int i = 0; i < MD2.size(); ++i) {
                if (not isdigit(MD2[i])) {
                    int a = 0, b = 0;
                    if ((i-2) >= 0 and isdigit(MD2[i-2]) and isdigit(MD2[i-1])) {
                        a = MD2[i-2] - '0';
                        b = MD2[i-1] - '0';
                    } else if (isdigit(MD2[i-1])) {
                        b = MD2[i-1] - '0';
                    }
                    sum += a*10 + b;
                    ++sum;
                    mmposread.insert(std::make_pair(sum, std::string(1, MD2[i])));
                }
            }
        }

        bool first = true;
        for (auto it = mmposread.begin(); it != mmposread.end(); ++it) {
            if (first) {
                if (lastMismatch > 100) {
                    mmpos.push_back(std::make_pair(100 + it->first, it->second));
                }
                else {
                    mmpos.push_back(std::make_pair((100 - lastMismatch) + it->first, it->second));
                }
                first = false;
            }
            mmpos.push_back(std::make_pair(it->first, it->second));
        }
        mmposread.clear();

        return mmpos;
    }

    /** \brief Gets the mmtype descriptor for the read using alphabet 0
     *  \param List of mismatches
     *  \return List of type of mismatches
     * */
    std::vector<std::string> getmmtypeDescriptor(std::vector<std::pair<int, std::string> >& mmpos) {
        std::vector<std::string> result;
        for (int i = 0; i < mmpos.size(); ++i) {
            if (mmpos[i].second[0] == 'I') {
                if (mmpos[i].second[1] == 'A') result.push_back(int_to_hex(6));
                else if (mmpos[i].second[1] == 'C') result.push_back(int_to_hex(7));
                else if (mmpos[i].second[1] == 'G') result.push_back(int_to_hex(8));
                else if (mmpos[i].second[1] == 'T') result.push_back(int_to_hex(9));
            }
            else if (mmpos[i].second[0] == 'D') result.push_back(int_to_hex(5));
            else if (mmpos[i].second[0] == 'A') result.push_back(int_to_hex(0));
            else if (mmpos[i].second[0] == 'C') result.push_back(int_to_hex(1));
            else if (mmpos[i].second[0] == 'G') result.push_back(int_to_hex(2));
            else if (mmpos[i].second[0] == 'T') result.push_back(int_to_hex(3));
        }
        return result;
    }

    /** \brief Gets the rlen descriptor for the read
     *  \param The first read
     *  \return The corresponding int value
     * */
    std::string getRlenDescriptor(BamAlignmentRecord& record) {
        CharString seq = record.seq;
        const char *s1 = toCString(seq);
        std::string str(s1);
        return int_to_hex(str.size());
    }

    /** \brief Gets the pair descriptor for the read
     *  \param The first read
     *  \return The corresponding value in little endian hexadecimal
     * */
    std::string getPairDescriptor(BamAlignmentRecord& record) {
        bool firstRead = record.flag & 64;
        std::string result;
        if (record.flag & 8) (record.flag & 64) ? result = "8001" : result = "7fff";
        if (record.flag & 4) result = "8000";
        if (record.flag & 2048) result = int_to_hex(record.rNextId) + int_to_hex(record.beginPos);

        if (firstRead) {
            if (record.rID == record.rNextId) {
                result = "7ffd" + int_to_hex(record.tLen);
            } else {
                result = "7ffe" + int_to_hex(record.rNextId) + int_to_hex(record.pNext);
            }
        } else {
            if (record.rID == record.rNextId) {
                result = "8003" + int_to_hex(record.pNext);
            } else {
                result = "8002" + int_to_hex(record.rNextId) + int_to_hex(record.pNext);
            }
        }
        return toLittleEndian(result);
    }

    /** \brief Extracts the value of the NM tag from the read
     *  \param The read
     *  \return The corresponding NM value
     * */
    int getNMtag(BamAlignmentRecord& record) {
        BamTagsDict tagsDict(record.tags);
        unsigned tagIdx = 0;
        if (!findTagKey(tagIdx, tagsDict, "NM")) std::cerr << "ERROR: Unknown key!\n";

        int tagVal = 0;
        if (!extractTagValue(tagVal, tagsDict, tagIdx)) std::cerr << "ERROR: There was an error extracting MD from tags!\n";

        return tagVal;
    }

    /** \brief Extracts the value of the ND tag from the read
     *  \param The read
     *  \return The corresponding MD value
     * */
    std::string getMDtag(BamAlignmentRecord& record) {
        BamTagsDict tagsDict(record.tags);
        unsigned tagIdx = 0;
        if (!findTagKey(tagIdx, tagsDict, "MD")) std::cerr << "ERROR: Unknown key!\n";

        CharString tagVal = 0;
        if (!extractTagValue(tagVal, tagsDict, tagIdx)) std::cerr << "ERROR: There was an error extracting MD from tags!\n";

        return toCString(tagVal);
    }

    void convertToMpeggRecord(MpeggRecord& result, BamAlignmentRecord& record) {
        result.global_Id = ++record_id;
        result.read_name = toCString(record.qName);
        result.class_type = getClassType(record);
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
            second = getSecondRecord(record);
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

        result.cigar_size.at(0).push_back(getCigar(record.cigar).size());
        if (result.number_of_segments > 1) result.cigar_size.at(1).push_back(getCigar(second.cigar).size());

        result.ecigar_string.resize(result.number_of_segments);
        for (int i = 0; i < result.number_of_segments; ++i) result.ecigar_string[i].resize(1);

        result.ecigar_string.at(0).push_back(getExtendedCigar(record));
        if (result.number_of_segments > 1) result.ecigar_string.at(1).push_back(getExtendedCigar(second));

        result.reverse_comp.resize(result.number_of_segments);
        for (int i = 0; i < result.number_of_segments; ++i) result.reverse_comp[i].resize(1);

        result.reverse_comp.at(0).push_back((record.flag & 16) ? 0: 1);
        if (result.number_of_segments > 1) result.reverse_comp.at(1).push_back((second.flag & 16) ? 0: 1);
    }

    /** Convert a hexadecimal value stored in a string to little endian
     * @param The hexadecimal value stored in stored
     * @return The same value but in little endian
     */
    std::string toLittleEndian(std::string a) {
        std::string result = "";
        if (a.size() % 2 != 0) a = std::string(1, '0').append(a);
        int j = a.size() - 1;
        while (j > 0) {
            result += a[j - 1];
            result += a[j];
            j = j - 2;
        }
        return result;
    }

    /** \brief A simple function that converts a value from a int to a hex
     *  \param The int value to be converted
     *  \return The hexadecimal value converted
     * */
    std::string int_to_hex(int32_t a) {
        std::stringstream stream;
        stream << std::setfill ('0') << std::setw(sizeof(a)*1)
               << std::hex << a;
        return stream.str();
    }
};

#endif
