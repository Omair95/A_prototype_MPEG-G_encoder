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

    for (auto it = ret.first; it != ret.second; ++it) {
        if (!s.compare(toCString(it->second.first.qName))) {
            BamAlignmentRecord first = it->second.first;
            reads.erase(it);
            reads.insert(std::make_pair(pos, std::make_pair(first, record)));
            break;
        }
    }
}

bool Utils::isClassP(BamAlignmentRecord& record) {
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

bool Utils::isClassN(BamAlignmentRecord& record) {
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

bool Utils::isClassM(BamAlignmentRecord& record) {
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

bool Utils::isClassI(BamAlignmentRecord& record) {
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

bool Utils::isClassHM(BamAlignmentRecord& record) {
    if (record.flag & 128 and record.flag & 1) return true;
    return false;
}

bool Utils::isClassU(BamAlignmentRecord& record) {
    if (record.flag & 4) return true;
    return false;
}

uint8_t Utils::getClassType(BamAlignmentRecord& record) {
    if (isClassP(record)) return 1;
    else if (isClassN(record)) return 2;
    else if (isClassM(record)) return 3;
    else if (isClassI(record)) return 4;
    else if (isClassHM(record)) return 5;
    else if (isClassU(record)) return 6;
}

uint8_t Utils::getRcompDescriptor(BamAlignmentRecord& record) {
    uint8_t result = 0;
    bool pair1 = record.flag & 16;

    if (record.flag & 8) {
        if (pair1) result = 0;
        else result = 1;
    }
    else {
        bool pair2 = record.flag & 32;
        bool firstRead = record.flag & 64;

        if (firstRead) {
            if (not pair1 and not pair2) result = 3;  // both reads on forward
            else if (pair1 and pair2) result = 0;     // both reads on reverse
            else if (not pair1 and pair2) result = 1; // first read on forward, 2nd on reverse
            else if (pair1 and not pair2) result = 2; // first read on reverse, 2nd on forward
        } else {
            if (not pair1 and not pair2) result = 3;  // both reads on forward
            else if (pair1 and pair2) result = 0;     // both reads on reverse
            else if (not pair1 and pair2) result = 2; // second on forward and 1st on reverse
            else if (pair1 and not pair2) result = 1; // second on reverse and 1st on forward
        }
    }
    return result;
}

uint8_t Utils::getFlagDescriptor(BamAlignmentRecord& record) {
    int flag = record.flag;
    uint8_t result = 0;
    if (flag & 1024) result = 1;
    if (flag & 512) result += 2;
    if (not(flag & 8)) result += 4;
    return result;
}

std::vector<std::pair<uint16_t, std::string> > Utils::getmmposDescriptor(BamAlignmentRecord& record) {
    std::vector<std::pair<uint16_t, std::string> > mmpos;
    std::map<int, std::string> mmposread;
    CharString seq = record.seq;
    const char *s1 = toCString(seq);
    std::string str1(s1);
    std::string cigarRead1 = getCigar(record.cigar);
    std::string MD = getMDtag(record);
    int sum = 0, lastMismatch = 0;
    bool firstMismatch = true;
    int insertions = 0;

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
                sum += a*10 + b;

                if (cigarRead1[i] == 'I' or cigarRead1[i] == 'D') {
                    while (b > 0) {
                        --b;
                        if (cigarRead1[i] == 'I') {
                            char c = str1[sum - 1];
                            std::string a = "I";
                            a.insert(a.size(), 1, c);
                            ++insertions;
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
                mmposread.insert(std::make_pair(sum + insertions, std::string(1, MD[i])));
            }
        }
    }
    int ant = 0;
    for (auto it = mmposread.begin(); it != mmposread.end(); ++it) {
        if (firstMismatch) {
            ant = it->first;
            mmpos.push_back(std::make_pair(it->first, it->second));
            firstMismatch = false;
        } else {
            mmpos.push_back(std::make_pair(it->first - ant, it->second));
            ant = it->first;
        }
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
    insertions = 0;

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
                sum += a*10 + b;

                if (cigarRead2[i] == 'I' or cigarRead2[i] == 'D') {
                    while (b > 0) {
                        --b;
                        if (cigarRead2[i] == 'I') {
                            char c = str2[sum - 1];
                            std::string a = "I";
                            a.insert(a.size(), 1, c);
                            ++insertions;
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
                mmposread.insert(std::make_pair(sum + insertions, std::string(1, MD2[i])));
            }
        }
    }

    bool first = true;
    ant = 0;
    for (auto it = mmposread.begin(); it != mmposread.end(); ++it) {
        if (first) {
            if (lastMismatch != 0) {
                mmpos.push_back(std::make_pair(100 - lastMismatch + it->first, it->second));
                ant = it->first;
            } else {
                mmpos.push_back(std::make_pair(100 + it->first, it->second));
                ant = it->first;
            }
            first = false;
        } else {
            mmpos.push_back(std::make_pair(it->first - ant, it->second));
            ant = it->first;
        }
    }
    mmposread.clear();

    return mmpos;
}

std::vector<uint8_t> Utils::getmmtypeDescriptor(std::vector<std::pair<uint16_t, std::string> >& mmpos) {
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

uint8_t Utils::getRlenDescriptor(BamAlignmentRecord& record) {
    CharString seq = record.seq;
    const char *s1 = toCString(seq);
    std::string str(s1);
    return str.size();
}

// 3661
std::string Utils::getPairDescriptor(BamAlignmentRecord& record) {
    std::string result;
    bool firstRead = record.flag & 64;

    if (record.flag & 4) {
        result = "8000"; // read unmapped
        return result;
    }
    if (record.flag & 8) {
        if (firstRead) result = "8001"; // read2 unpaired
        else result = "7fff";           // read1 unpaired
        return result;
    }

    if (record.flag & 2048) result = int16_to_hex(record.rNextId) + int32_to_hex(record.beginPos);

    if (firstRead) {
        if (record.rID == record.rNextId) {
            result = "7ffd" + int32_to_hex(reads_distance(record)); // read2 in pair is on the same reference but coded separately
        } else {
            uint16_t referenceID = record.rNextId;
            uint32_t pos = record.pNext;
            result = "7ffe" + int16_to_hex(referenceID) + int32_to_hex(pos); // read2 is on a diferent reference
        }
    } else {
        if (record.rID == record.rNextId) {
            result = "8003" + int32_to_hex(reads_distance(record)); // read1 in pair is on the same reference but coded separately
        } else {
            result = "8002" + int16_to_hex(record.rNextId) + int32_to_hex(record.pNext); // read1 is on a diferente reference
        }
    }
    return result;
}

uint16_t Utils::reads_distance(BamAlignmentRecord& record) {
    int distance = record.tLen;
    int sign;
    if (distance < 0) {
        sign = 1;
        distance = distance * (-1);
    } else sign = 0;

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
    result.seq_Id = (result.class_type == 6) ? 0 : record.rID;
    result.read1_first = (record.flag & 64) ? 1 : 0;
    result.flags = record.flag;
    result.number_of_segments = (record.flag & 8) ? 1 : 2;
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

std::string Utils::toLittleEndian_hex(std::string value) {
    std::string result = "";
    if (value.size() % 2 != 0) value = std::string(1, '0').append(value);
    int j = value.size() - 1;
    while (j > 0) {
        result += value[j - 1];
        result += value[j];
        j = j - 2;
    }
    return result;
}

uint64_t Utils::hex_to_int(std::string value) {
    uint64_t x;
    std::stringstream ss;
    ss << std::hex << value;
    ss >> x;
    return x;
}

std::string Utils::int32_to_hex(int32_t value) {
    std::stringstream stream;
    stream << std::setfill ('0') << std::setw(sizeof(value)*2)
           << std::hex << value;
    return stream.str();
}

std::string Utils::int16_to_hex(int16_t value) {
    std::stringstream stream;
    stream << std::setfill ('0') << std::setw(sizeof(value)*2)
           << std::hex << value;
    return stream.str();
}

std::vector<std::string> Utils::getClipsDescriptor(MpeggRecord& record) {
    std::vector <std::string> clips_descriptor;
    std::string cigar = record.ecigar_string[0][1];
    std::string sequence = record.sequence[0];

    auto n = std::count(cigar.begin(), cigar.end(), '(');

    for (int i = 0; i < n; ++i) {
        std::string result;
        uint32_t id = record.global_Id;
        result += std::to_string(id);           // id

        if (i == 0) {
            result += "00";                     // position
            auto pos = cigar.find('(');
            result += sequence.substr(0, cigar[pos + 1] - '0');  // M bases
            if (n > 1) result += "fe";          // soft clips terminator
        } else {
            result += "01";
            auto posL = sequence.size() - (cigar[cigar.size() - 2] - '0');
            result += sequence.substr(posL, cigar.size() - 1);
        }
        if (i == (n - 1)) result += "ff";
        clips_descriptor.push_back(result);
    }

    return clips_descriptor;
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

void Utils::getAllAccessUnits(std::vector<AccessUnit>& au) {
    au = accessUnits;
}

void Utils::insertRead(BamAlignmentRecord first, BamAlignmentRecord second) {
    reads.insert(std::make_pair(first.beginPos, std::make_pair(first, second)));
}