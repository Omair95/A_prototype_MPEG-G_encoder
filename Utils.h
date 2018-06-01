#include "AccessUnit.h"
#include "MpeggRecord.h"
#include <sstream>
#include <cctype>


/**
 * @file Utils.h
 * @Author Omair Iqbal
 * @date 04/2018
 * @struct Utils
 * @brief Auxiliary class that contains useful methods from the main class
 * @contact Omair95@protonmail.com
*/

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_UTILS_H
#define A_PROTOTYPE_MPEG_G_ENCODER_UTILS_H

class Utils {

private:
    std::vector<AccessUnit> accessUnits;                                            /// contains all kind of access units
    std::multimap<int, std::pair<BamAlignmentRecord, BamAlignmentRecord> > reads;   /// contains reads that can be paired or single paired
    uint64_t record_id;                                                             /// identifier of the genomic record in the output mpegg format

public:

    /** @brieg Constructor method
     * @param void
     * @return void
     * */
    Utils();

    /** @brieg Destructor method
     * @param void
     *  @return void
     * */
    ~Utils();

    /** @brieg Convert from a string<cigarElement> to a std::string
     *  @param rCigar value to be converted
     *  @return converted value to string
     * */
    static std::string getCigar(String <CigarElement<> >& rCigar);

    /** @brieg Get extended cigar of the read
     * @param record read to be treated
     * @return Extended cigar of the read
     */
    std::string getExtendedCigar(BamAlignmentRecord& record);

    /** @brieg Get the second read for the read pair
     *  @param record first read from the the pair
     *  @return second read of the pair
     * */
    BamAlignmentRecord getSecondRecord(BamAlignmentRecord& record);

    /** @brieg Update the second read from the pair with a new value
     *  @param record new value of the second pair
     *  @param pos position of the first read in the pair
     * */
    void updateRecord(BamAlignmentRecord& record, int& pos);

    /** @brieg Determine if the read belongs to the class P, reads
     *         perfectly matching the reference sequence
     *  @param record first read of the pair
     *  @return bool indicating if the read belongs to class P
     * */
    bool isClassP(BamAlignmentRecord& record);

    /** @brieg Determine if the read belongs to class N, reads
     *         containing mismatches which are unknonwn bases only
     *  @param record first read from the pair
     *  @return bool indicating if the read belongs to class N
     * */
    bool isClassN(BamAlignmentRecord& record);

    /** @brieg Determine if the read belongs to class M, reads contanining
     *         at least one substitution, optionally unknown bases and no
     *         insertion, deletions and clipped bases
     *  @param record first read from the pair
     *  @return bool indicating if the read belongs to class M
     * */
    bool isClassM(BamAlignmentRecord& record);

    /** @brieg Determine if the read belongs to class I, reads contanining
     *         at least one insertion, deletion or clippled base, and
     *         optionally unknown bases or substitution
     *  @param record is the first read from the pair
     *  @return bool indicating if the read belongs to class I
     * */
    bool isClassI(BamAlignmentRecord& record);

    /** @brieg Determine the data class where a read belongs to
     *  @param record is the first read from the pair
     *  @return integer indicating the type of data class of the read
     * */
    uint8_t getClassType(BamAlignmentRecord& record);

    /** @brieg Get the distance between two read pairs
     *  @param record is the first read from the pair
     *  @return integer representing the distance of the read pair
     * */
    static uint16_t reads_distance(BamAlignmentRecord& record);

    /** @brieg Extract the value of the NM tag from the read
     *  @param record read to be treated
     *  @return integer containing the value of the NM tag
     * */
    int getNMtag(BamAlignmentRecord& record);

    /** @brieg Extract the value of the MD tag from the read
     *  @param record read to be treated
     *  @return string containing the value of the MD tag
     * */
    static std::string getMDtag(BamAlignmentRecord& record);

    /** @brieg Converts the read or paired read to the mpegg output format
     *  @param record first read from the pair
     *  @return void
     * */
    void convertToMpeggRecord(MpeggRecord& result, BamAlignmentRecord& record);

    /** @brieg Return and writes the mmpos descriptor values to the respective file
     *         according to the class type
     *  @param record first read from the read pair
     *   @return a vector containing the positions and type of mismatches of the read or read pair
     * */
    std::vector<std::pair<uint16_t, std::string> > getMmposValues(BamAlignmentRecord& record);

    /** @brieg Remove the first read from the list of paired reads
     * @param void
     * @return void
     * */
    void removeFirstRead();

    /** @brieg Get all the paired reads
     * @param allReads will contain the value of the paired readss
     * @return void
     * */
    void getAllreads(std::multimap<int, std::pair<BamAlignmentRecord, BamAlignmentRecord> >& allReads);

    /** @brieg Inserts a new access unit to the list of access units
     * @param au access unit to be inserted
     * @return void
     * */
    void insertAccessUnit(AccessUnit au);

    /** @brieg Get all the access units stored
     * @param au will contain the value of all access units
     * @return void
     * */
    std::vector<AccessUnit> getAllAccessUnits();

    /** @brieg Inserts a new read pair to the list of paired reads
     * @param first first read from the pair
     * @param second second read from the pair
     * @return void
     * */
    void insertRead(BamAlignmentRecord first, BamAlignmentRecord second);

    /** @brieg Check if a read is paired or not
     * @param first first read from the pair
     * @param second second read from the pair
     * @return void
     * */
    static bool isPaired(BamAlignmentRecord first, BamAlignmentRecord second);

    /** @brieg get the sequence length of the read
     * @param record read record
     * @return unsigned integer representing the length of the sequence
     * */
    static uint8_t getSequenceLength(BamAlignmentRecord& record);

    /** @brieg get rcomp descriptor value of the read pair
     * @param first first read from the pair
     * @param second second read from the pair
     * @return pair representing the first and second value of the rcomp descriptor
     * */
    std::pair<uint8_t, uint8_t> getRcompDescriptor(BamAlignmentRecord& record, BamAlignmentRecord& record2);

    /** @brieg get flags descriptor of the read
     * @param record read
     * @return pair representing the first and second value of the rcomp descriptor
     * */
    uint8_t getFlagDescriptor(BamAlignmentRecord& record);

    /** @brieg get rlen value of the read pair
     * @param record read
     * @return uinsigned integer representing the value of the rlen descriptor
     * */
    uint8_t getRlenDescriptor(BamAlignmentRecord& record);

    /** @brieg get pair descriptor values of the read pair
     * @param record first read from the read pair
     * @param record2 second read from the read pair
     * @return vector representing the values of the pair descriptor for the read pair
     * */
    std::vector<std::pair<std::string, uint8_t> > getPairDescriptor(BamAlignmentRecord& record, BamAlignmentRecord& record2);

    /** @brieg get mmtype descriptor values for the read pair
     * @param mmpos vector representing the mismatmatch positions and the type of mistmatch
     * @return vector representing the type of mismatches
     * */
    std::vector<uint8_t> getMmtypeDescriptor(std::vector<std::pair<uint16_t, std::string> >& mmpos);

    /** @brieg get soft clip descriptor value of the mpegg record
     * @param record mpegg record
     * @param id represents the number of reads stored in the access unit
     * @return vector representing the values of the soft clips descriptor
     * */
    std::vector<std::pair<std::string, uint8_t> > getClipsDescriptor(MpeggRecord& record, uint32_t id);
};

#endif