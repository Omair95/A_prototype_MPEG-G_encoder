#include <boost/endian/conversion.hpp>
#include "Utils.h"

Utils::Utils() {
    record_id = 0;
}

Utils::~Utils() {

}

std::string Utils::getCigar(String <CigarElement<> >& rCigar) {
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

std::string Utils::getExtendedCigar(BamAlignmentRecord& record) {
    std::string cigar = getCigar(record.cigar);
    std::string MD = getMDtag(record);
    std::string result;
    std::map<int, std::string> mmpos;
    int sum = 0, softClipsIni = 0, softClipsFinal = 0;

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
                if (cigar[i] == 'S') {
                    if (softClipsIni == 0 and sum == 0) softClipsIni = a * 10 + b;
                    else softClipsFinal = a * 10 + b;
                }
                if (cigar[i] != 'D' and cigar[i] != 'S') sum += a * 10 + b;
                if (cigar[i] != 'M' and cigar[i] != 'S') {
                    std::string c;
                    c += std::to_string(a*10 + b);
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
                sum += a*10 + b;
                if (MD[i] != '^') {
                    ++sum;
                    std::string c;
                    c += std::to_string(sum);
                    c += MD[i];
                    mmpos.insert(std::make_pair(sum, c));
                } else {
                    while(not isdigit(MD[i])) ++i;
                }
            }
        }
    }

    if (cigar == "100M" and MD == "100") return "100=";

    int insertions = 0;
    int ant = 0;
    auto it = mmpos.begin();
    auto last = mmpos.end();

    if (it->first == last->first and it->second == last->second and (softClipsIni != 0 or softClipsFinal != 0)) {
        if (softClipsIni != 0) result += '(' + std::to_string(softClipsIni) + ')';
        result += std::to_string(100 - softClipsFinal - softClipsIni) + "=";
        if (softClipsFinal != 0) result += '(' + std::to_string(softClipsFinal) + ')';
        return result;
    }
    --last;

    if (softClipsIni != 0) result += '(' + std::to_string(softClipsIni) + ')';

    while (it != mmpos.end()) {
        if (it->second[it->second.size() - 1] == 'I' or
            it->second[it->second.size() - 1] == 'D') {
            int a = 0, b = 0;
            a = it->second[0] - '0';
            if (isdigit(it->second[1])) {
                b = it->second[1] - '0';
                result += std::to_string(it->first - ant - (a*10 + b));
                result += '=';
                if (it->second[it->second.size() - 1] == 'I') {
                    result += std::to_string(a*10 + b) + '+';
                    insertions += a*10 + b;
                }
                else result += std::to_string(a*10 + b) + '-';
            } else {
                result += std::to_string(it->first - ant - a);
                result += '=';
                if (it->second[it->second.size() - 1] == 'I') {
                    result += std::to_string(a) + '+';
                    insertions += a;
                }
                else result += std::to_string(a) + '-';
            }
            ant = it->first;
        } else {
            int d = it->first - ant;
            if (d != 0) {
                result += std::to_string(it->first - ant - 1);
                result += '=';
            }
            result += it->second[it->second.size() - 1];
            ant = it->first;
        }
        ++it;
    }

    result += std::to_string(100-sum-insertions-softClipsFinal-softClipsIni) + "=";

    if (softClipsFinal) result += '(' + std::to_string(softClipsFinal) + ')';

    return result;
}

BamAlignmentRecord Utils::getSecondRecord(BamAlignmentRecord& record) {
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

void Utils::updateRecord(BamAlignmentRecord& record, int& pos) {
    typedef std::multimap<int, std::pair<BamAlignmentRecord, BamAlignmentRecord> >::iterator it;
    std::pair<it, it> ret;
    ret = reads.equal_range(pos);
    std::string s = toCString(record.qName);
    bool found = false;
    for (auto it = ret.first; it != ret.second; ++it) {
        if (!s.compare(toCString(it->second.first.qName))) {
            it->second.second = record;
            found = true;
            break;
        }
    }

    if (not found) reads.insert(std::make_pair(record.beginPos, std::make_pair(record, record)));
}

bool Utils::isClassP(BamAlignmentRecord& record) {
    String<CigarElement<> > a = record.cigar;
    std::string firstCigar = getCigar(a);
    BamTagsDict tagsDict(record.tags);
    unsigned tagIdx = 0;
    if (!findTagKey(tagIdx, tagsDict, "MD")) std::cerr << "ERROR: Unknown key!" << std::endl;

    CharString tagValFirst = 0;
    if (!extractTagValue(tagValFirst, tagsDict, tagIdx)) std::cerr << "ERROR: There was an error extracting MD from tags!" << std::endl;

    BamAlignmentRecord second = getSecondRecord(record);
    if (not isPaired(record, second)) {
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

bool Utils::isClassN(BamAlignmentRecord& record) {
    CharString seq = record.seq;
    const char *s1 = toCString(seq);
    std::string str1(s1);
    std::string firstCigar = getCigar(record.cigar);
    int tagValFirst = getNMtag(record);
    auto nFirst = std::count(str1.begin(), str1.end(), 'N');

    BamAlignmentRecord second = getSecondRecord(record);
    if (not isPaired(record, second)) {
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

bool Utils::isClassM(BamAlignmentRecord& record) {
    CharString seq = record.seq;
    const char *s1 = toCString(seq);
    std::string str1(s1);
    std::string firstCigar = getCigar(record.cigar);
    int tagValFirst = getNMtag(record);
    auto nFirst = std::count(str1.begin(), str1.end(), 'N');

    BamAlignmentRecord second = getSecondRecord(record);
    if (not isPaired(record, second)) {
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

bool Utils::isClassI(BamAlignmentRecord& record) {
    std::string firstCigar = getCigar(record.cigar);;

    BamAlignmentRecord second = getSecondRecord(record);
    if (not isPaired(record, second)) {
        if (firstCigar != "100M") return true;
    } else {
        BamAlignmentRecord second = getSecondRecord(record);
        std::string secondCigar = getCigar(second.cigar);
        if (firstCigar != "100M" or secondCigar != "100M") return true;
    }
    return false;
}

uint8_t Utils::getClassType(BamAlignmentRecord& record) {
    if (isClassP(record)) return 1;
    else if (isClassN(record)) return 2;
    else if (isClassM(record)) return 3;
    else if (isClassI(record)) return 4;
}

uint16_t Utils::reads_distance(BamAlignmentRecord& record) {
    std::string name = toCString(record.qName);

    int distance = record.pNext - record.beginPos;
    if (distance < 0) distance = distance * (-1);

    int sign;

    if (record.flag & 128) sign = 1;
    else sign = 0;

    distance = distance << 1;
    distance |= sign;
    return distance;
}

int Utils::getNMtag(BamAlignmentRecord& record) {
    BamTagsDict tagsDict(record.tags);
    unsigned tagIdx = 0;
    if (!findTagKey(tagIdx, tagsDict, "NM")) std::cerr << "ERROR: Unknown key!\n";

    int tagVal = 0;
    if (!extractTagValue(tagVal, tagsDict, tagIdx)) std::cerr << "ERROR: There was an error extracting MD from tags!\n";

    return tagVal;
}

std::string Utils::getMDtag(BamAlignmentRecord& record) {
    BamTagsDict tagsDict(record.tags);
    unsigned tagIdx = 0;
    if (!findTagKey(tagIdx, tagsDict, "MD")) std::cerr << "ERROR: Unknown key!\n";

    CharString tagVal = 0;
    if (!extractTagValue(tagVal, tagsDict, tagIdx)) std::cerr << "ERROR: There was an error extracting MD from tags!\n";

    return toCString(tagVal);
}

void Utils::convertToMpeggRecord(MpeggRecord& result, BamAlignmentRecord& record) {
    result.global_Id = ++record_id;
    result.read_name = toCString(record.qName);
    result.class_type = getClassType(record);
    result.seq_Id = record.rID;
    result.read1_first = (record.flag & 64) ? 1 : 0;
    result.flags = record.flag;
    BamAlignmentRecord second = getSecondRecord(record);
    result.number_of_segments = (not isPaired(record, second)) ? 1 : 2;
    result.number_of_alignments = 1; // just put 1 for now

    CharString seq = record.seq;
    const char *s1 = toCString(seq);
    std::string sequence(s1);
    std::string qv = toCString(record.qual);

    result.read_len.push_back(sequence.size());
    result.quality_values.push_back(qv);
    result.sequence.push_back(sequence);

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

std::vector<std::pair<uint16_t, std::string> > Utils::getMmposValues(BamAlignmentRecord& record) {
    std::vector<std::pair<uint16_t, std::string> > mmpos;
    std::map<int, std::string> mmposread;
    CharString seq = record.seq;
    const char *s1 = toCString(seq);
    std::string str1(s1);
    std::string cigarRead1 = getCigar(record.cigar);
    std::string MD = getMDtag(record);
    int sum = 0, lastMismatch = 0;
    bool firstMismatch = true;

    if (cigarRead1 != "100M") {
        for (int i = 0; i < cigarRead1.size(); ++i) {
            if (not isdigit(cigarRead1[i])) {
                int a = 0, b = 0;
                if ((i-2) >= 0 and isdigit(cigarRead1[i-2]) and isdigit(cigarRead1[i-1])) {
                    a = cigarRead1[i-2] - '0';
                    b = cigarRead1[i-1] - '0';
                } else if (isdigit(cigarRead1[i-1])) {
                    b = cigarRead1[i-1] - '0';
                }
                if (cigarRead1[i] != 'S') sum += a*10 + b;

                if (cigarRead1[i] == 'I' or cigarRead1[i] == 'D') {
                    while (b > 0) {
                        --b;
                        if (cigarRead1[i] == 'I') {
                            char c = str1[sum - 1];
                            std::string a = "I";
                            a.insert(a.size(), 1, c);
                            mmposread.insert(std::make_pair(sum - b - 1, a));
                        } else {
                            mmposread.insert(std::make_pair(sum - b, std::string(1, cigarRead1[i])));
                        }
                    }
                    if (sum >= lastMismatch) {
                        lastMismatch = sum;
                    }
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
    int ant = 0;
    for (auto it = mmposread.begin(); it != mmposread.end(); ++it) {
        if (firstMismatch) {
            ant = it->first;
            mmpos.push_back(std::make_pair(it->first, it->second));
            firstMismatch = false;

            if (it->second[0] == 'I') ant = it->first - 1;
            else ant = it->first;
        } else {
            mmpos.push_back(std::make_pair(it->first - ant, it->second));
            if (it->second[0] == 'I') ant = it->first - 1;
            else ant = it->first;
        }
    }

    mmposread.clear();

    BamAlignmentRecord second = getSecondRecord(record);
    if (not isPaired(record, second)) return mmpos;

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
                if ((i-2) >= 0 and isdigit(cigarRead2[i-1]) and isdigit(cigarRead2[i-2])) {
                    a = cigarRead2[i-2] - '0';
                    b = cigarRead2[i-1] - '0';
                } else if (isdigit(cigarRead2[i-1])) {
                    b = cigarRead2[i-1] - '0';
                }
                if (cigarRead1[i] != 'S') sum += a*10 + b;

                if (cigarRead2[i] == 'I' or cigarRead2[i] == 'D') {
                    while (b > 0) {
                        --b;
                        if (cigarRead2[i] == 'I') {
                            char c = str2[sum - 1];
                            std::string a = "I";
                            a.insert(a.size(), 1, c);
                            mmposread.insert(std::make_pair(sum - b - 1, a));
                        } else {
                            mmposread.insert(std::make_pair(sum - b, std::string(1, cigarRead2[i])));
                        }
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
                if ((i-2) >= 0 and isdigit(MD2[i-1]) and isdigit(MD2[i-2])) {
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
    ant = 0;
    for (auto it = mmposread.begin(); it != mmposread.end(); ++it) {
        if (first) {
            if (lastMismatch != 0) mmpos.push_back(std::make_pair(100 - lastMismatch + it->first, it->second));
            else mmpos.push_back(std::make_pair(100 + it->first, it->second));

            if (it->second[0] == 'I') ant = it->first - 1;
            else ant = it->first;
            first = false;
        } else {
            mmpos.push_back(std::make_pair(it->first - ant, it->second));
            if (it->second[0] == 'I') ant = it->first - 1;
            else ant = it->first;
        }
    }
    mmposread.clear();

    return mmpos;
}

void Utils::removeFirstRead() {
    auto it = reads.begin();
    reads.erase(it++);
}

void Utils::getAllreads(std::multimap<int, std::pair<BamAlignmentRecord, BamAlignmentRecord> >& allReads) {
    allReads = reads;
}

void Utils::insertAccessUnit(AccessUnit au) {
    accessUnits.push_back(au);
}

std::vector<AccessUnit> Utils::getAllAccessUnits() {
    return accessUnits;
}

void Utils::insertRead(BamAlignmentRecord first, BamAlignmentRecord second) {
    reads.insert(std::make_pair(first.beginPos, std::make_pair(first, second)));
}

bool Utils::isPaired(BamAlignmentRecord first, BamAlignmentRecord second) {
    if (first.flag & 8) return false;

    return !(strcmp(toCString(first.qName), toCString(second.qName)) == 0
             and first.flag == second.flag
             and first.rID == second.rID
             and first.beginPos == second.beginPos
             and first.rNextId == second.rNextId
             and first.pNext == second.pNext
             and first.tLen == second.tLen
             and strcmp(toCString(first.qual), toCString(second.qual)) == 0
             and strcmp(toCString(first.tags), toCString(second.tags)) == 0);
}

uint8_t Utils::getSequenceLength(BamAlignmentRecord& record) {
    int8_t result;
    CharString seq = record.seq;
    const char *s1 = toCString(seq);
    std::string str(s1);
    result = str.size();
    return result;
}

std::pair<uint8_t, uint8_t> Utils::getRcompDescriptor(BamAlignmentRecord& record, BamAlignmentRecord& record2) {
    std::pair<uint8_t, uint8_t > result;
    if (Utils::isPaired(record, record2)) { // if 2 reads in pair are stored together
        if (record.flag & 16) {             // read1 is reverse
            if (record2.flag & 16) {        // read1 is reverse and read2 is reverse
                result.first = 3;
            } else {                        // read1 is reverse and read2 is forward
                result.first = 1;
            }
        } else {                            // read1 is forward
            if (record2.flag & 16) {        // read1 is forwand and read2 is reverse
                result.first = 2;
            } else {                        // read1 is forward and read2 is forward
                result.first = 0;
            }
        }
        if (record2.flag & 16) {
            if (record.flag & 16) {
                result.second = 3;
            } else {
                result.second += 1;
            }
        } else {
            if (record.flag & 16) {
                result.second = 2;
            } else {
                result.second = 0;
            }
        }
    } else {                                // reads are stored separately
        if (record.flag & 16) {             // read1 is reverse
            if (record2.flag & 16) {        // read1 is reverse and read2 is reverse
                result.first = 3;
                result.second = -1;
            } else {                        // read1 is reverse and read2 is forward
                result.first = 1;
                result.second = -1;
            }
        } else {                            // read1 is forward
            if (record2.flag & 16) {        // read1 is forward and read2 is reverse
                result.first = 2;
                result.second = -1;
            } else {                        // read1 is forward and read2 is forward
                result.first = 0;
                result.second = -1;
            }
        }
    }

    return result;
}

uint8_t Utils::getFlagDescriptor(BamAlignmentRecord& record) {
    uint8_t result = 0;
    if (record.flag & 1024) result = 1;
    if (record.flag & 512) result += 2;
    if (not(record.flag & 8)) result += 4;

    return result;
}

uint8_t Utils::getRlenDescriptor(BamAlignmentRecord& record) {
    return getSequenceLength(record);
}

std::vector<std::pair<std::string, uint8_t> > Utils::getPairDescriptor(BamAlignmentRecord& record, BamAlignmentRecord& record2) {
    std::vector<std::pair<std::string, uint8_t > > result;
    if (Utils::isPaired(record, record2)) {
        uint16_t distance = Utils::reads_distance(record);
        result.emplace_back(std::to_string(distance), 64);
        return result;
    }

    if (record.beginPos == record.pNext) {
        if (record.flag & 64) {
            std::string type ="7fff";
            result.emplace_back(type, 16);
        } else {
            std::string type = "8001";
            result.emplace_back(type, 16);
        }
        return result;
    }

    if (record.rID == record.rNextId) {
        if (record.flag & 64) {
            std::string type = "7ffd";
            uint32_t distance = record.pNext;

            result.emplace_back(type, 16);
            result.emplace_back(std::to_string(distance), 32);
            return result;
        } else {
            std::string type = "8003";
            uint32_t distance = record.pNext;

            result.emplace_back(type, 16);
            result.emplace_back(std::to_string(distance), 32);
            return result;
        }
    } else {
        if (record.flag & 64) {
            std::string type = "7ffe";
            uint8_t referenceID = record.rNextId;
            uint32_t distance = record.pNext;

            result.emplace_back(type, 16);
            result.emplace_back(std::to_string(referenceID), 8);
            result.emplace_back(std::to_string(distance), 32);

            return result;
        } else {
            std::string type = "8002";
            uint8_t referenceID = record.rNextId;
            uint32_t distance = record.pNext;

            result.emplace_back(type, 16);
            result.emplace_back(std::to_string(referenceID), 8);
            result.emplace_back(std::to_string(distance), 32);

            return result;
        }
    }
}

std::vector<uint8_t> Utils::getMmtypeDescriptor(std::vector<std::pair<uint16_t, std::string> >& mmpos) {
    std::vector<uint8_t > result;
    for (int i = 0; i < mmpos.size(); ++i) {
        if (mmpos[i].second[0] == 'I') {
            if (mmpos[i].second[1] == 'A') result.push_back(6);
            else if (mmpos[i].second[1] == 'C') result.push_back(7);
            else if (mmpos[i].second[1] == 'G') result.push_back(8);
            else if (mmpos[i].second[1] == 'T') result.push_back(9);
        }
        else if (mmpos[i].second[0] == 'D') result.push_back(5);
        else if (mmpos[i].second[0] == 'A') result.push_back(0);
        else if (mmpos[i].second[0] == 'C') result.push_back(1);
        else if (mmpos[i].second[0] == 'G') result.push_back(2);
        else if (mmpos[i].second[0] == 'T') result.push_back(3);
    }
    return result;
}

std::vector<std::pair<std::string, uint8_t> > Utils::getClipsDescriptor(MpeggRecord& record, uint32_t id) {
    std::vector <std::pair<std::string, uint8_t> > clips_descriptor;
    std::string read1_cigar = record.ecigar_string[0][1], read2_cigar;
    std::string read1_sequence = record.sequence[0], read2_sequence;
    auto n = std::count(read1_cigar.begin(), read1_cigar.end(), '(');

    for (int i = 0; i < n; ++i) {
        std::string result;

        // get and write id to file
        uint32_t idclip = id;
        uint32_t littleEndianID = boost::endian::native_to_little(idclip);
        clips_descriptor.emplace_back(std::to_string(idclip), 32);

        // get and write pos of the soft clip
        uint8_t flagPos;
        int pos;
        if (i == 0) {
            pos = read1_cigar.find('(');
            if (pos == 0) flagPos = 0x00;
            else {
                flagPos = 0x01;
                pos = read1_cigar.rfind('(');
            }
        } else {
            flagPos = 0x01;
            pos = read1_cigar.rfind('(');
        }
        uint8_t littleEndianFlagPos = boost::endian::native_to_little(flagPos);
        clips_descriptor.emplace_back(std::to_string(flagPos), 8);

        // get and write base sequences
        if (flagPos == 0x00) {
            int a = 0, b = 0, f;

            if (isdigit(read1_cigar[pos + 1]) and
                isdigit(read1_cigar[pos + 2])) {
                a = read1_cigar[pos+1] - '0';
                b = read1_cigar[pos+2] - '0';
                f = a * 10 + b;
            }
            else if (isdigit(read1_cigar[pos + 1])) {
                a = read1_cigar[pos+1] - '0';
                f = a;
            }

            for (int i = 0; i < f; ++i) {
                uint8_t c = read1_sequence[i];
                clips_descriptor.emplace_back(std::to_string(c), 8);
            }

            if (n > 1) {
                uint8_t terminator = 0xfe;
                uint8_t littleEndianTerminator = boost::endian::native_to_little(terminator);
                clips_descriptor.emplace_back(std::to_string(terminator), 8);
            }
        } else if (flagPos == 0x01) {
            int a = 0, b = 0, f = 0;

            if (isdigit(read1_cigar[pos + 1]) and
                isdigit(read1_cigar[pos + 2])) {
                a = read1_cigar[pos + 1] - '0';
                b = read1_cigar[pos + 2] - '0';
                f = a * 10 + b;
            }
            else if (isdigit(read1_cigar[pos + 1])) {
                a = read1_cigar[pos + 1] - '0';
                f = a;
            }

            for (int i = read1_sequence.size() - 1 - f; i < read1_sequence.size() - 1; ++i) {
                uint8_t c = read1_sequence[i];
                clips_descriptor.emplace_back(std::to_string(c), 8);
            }
            result += read1_sequence.substr(read1_sequence.size() - f, read1_sequence.size() - 1);
        }
    }

    if (record.number_of_segments == 1) {
        uint8_t terminator = 0xff;
        uint8_t littleEndianTerminator = boost::endian::native_to_little(terminator);
        clips_descriptor.emplace_back(std::to_string(terminator), 8);
        return clips_descriptor;
    }


    read2_cigar = record.ecigar_string[1][1];
    read2_sequence = record.sequence[1];

    auto k = std::count(read2_cigar.begin(), read2_cigar.end(), '(');

    if (k == 0) {
        uint8_t terminator = 0xff;
        clips_descriptor.emplace_back(std::to_string(terminator), 8);
        return clips_descriptor;
    } else if (n != 0) {
        uint8_t terminator = 0xfe;
        clips_descriptor.emplace_back(std::to_string(terminator), 8);
    }

    for (int i = 0; i < k; ++i) {
        // get and write id to file
        uint32_t idclip = id;
        uint32_t littleEndianID = boost::endian::native_to_little(idclip);
        clips_descriptor.emplace_back(std::to_string(idclip), 32);

        // get and write pos of the soft clip
        uint8_t flagPos;
        int pos;
        if (i == 0) {
            pos = read2_cigar.find('(');
            if (pos == 0) flagPos = 0x02;
            else {
                flagPos = 0x03;
                pos = read2_cigar.rfind('(');
            }
        } else {
            flagPos = 0x03;
            pos = read2_cigar.rfind('(');
        }
        uint8_t littleEndianFlagPos = boost::endian::native_to_little(flagPos);
        clips_descriptor.emplace_back(std::to_string(flagPos), 8);

        // get and write base sequences
        if (flagPos == 0x02) {
            int a = 0, b = 0, f;
            if (isdigit(read2_cigar[pos + 1]) and
                isdigit(read2_cigar[pos + 2])) {
                a = read2_cigar[pos + 1] - '0';
                b = read2_cigar[pos + 2] - '0';
                f = a * 10 + b;
            }
            else if (isdigit(read2_cigar[pos + 1])) {
                a = read2_cigar[pos + 1] - '0';
                f = a;
            }

            for (int i = 0; i < f; ++i) {
                uint8_t c = read2_sequence[i];
                clips_descriptor.emplace_back(std::to_string(c), 8);
            }
            uint8_t terminator;

            if (k > 1) terminator = 0xfe;
            else terminator = 0xff;
            clips_descriptor.emplace_back(std::to_string(terminator), 8);

        } else if (flagPos == 0x03) {
            int a = 0, b = 0, f;

            if (isdigit(read2_cigar[pos + 1]) and isdigit(read2_cigar[pos + 2])) {
                a = read2_cigar[pos + 1] - '0';
                b = read2_cigar[pos + 2] - '0';
                f = a * 10 + b;
            }
            else if (isdigit(read2_cigar[pos + 1])) {
                a = read2_cigar[pos + 1] - '0';
                f = a;
            }

            for (int i = read2_sequence.size() - f; i < read2_sequence.size(); ++i) {
                uint8_t c = read2_sequence[i];
                clips_descriptor.emplace_back(std::to_string(c), 8);
            }

            uint8_t terminator = 0xff;
            clips_descriptor.emplace_back(std::to_string(terminator), 8);
        }
    }

    return  clips_descriptor;
}
