#include "AccessUnit.h"
#include <sstream>
#include <ctype.h>
#include <fstream>
#include "MpeggRecord.h"

/*! \file Utils.h */

/** \class Utils class
 *
 *  \brief Auxiliary class that contains useful methods from the main class
 *
 * \author Omair Iqbal
 *
 * \date 04/2018
 *
 * Contact: Omair95@protonmail.com
 * */

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_UTILS_H
#define A_PROTOTYPE_MPEG_G_ENCODER_UTILS_H

class Utils {

private:
    std::vector<AccessUnit> accessUnits;                                            /// contains all kind of access units
    std::multimap<int, std::pair<BamAlignmentRecord, BamAlignmentRecord> > reads;   /// contains reads that can be paired or single paired
    uint64_t record_id;                                                             /// identifier of the genomic record in the output mpegg format

public:

    /** \brief Constructor method
     * \param void
     * \return void
     * */
    Utils();

    /** \brief Destructor method
     * \param void
     *  \return void
     * */
    ~Utils();

    /** \brief Convert from a string<cigarElement> to a std::string
     *  \param rCigar value to be converted
     *  \return converted value
     * */
    std::string getCigar(String <CigarElement<> >& rCigar);

    /** \brief Gets the extended cigar of the read
     * \param record read to be treated
     * \return Extended cigar of the read
     */
    std::string getExtendedCigar(BamAlignmentRecord& record);

    /** \brief Get the second read for the read pair
     *  \param record first read from the the pair
     *  \return second read of the pair
     * */
    BamAlignmentRecord getSecondRecord(BamAlignmentRecord& record);

    /** \brief Updates the second read from the pair with a new value
     *  \param record new value of the second pair
     *  \param pos position of the first read in the pair
     * */
    void updateRecord(BamAlignmentRecord& record, int& pos);

    /** \brief To determine if the read belongs to class P, reads
     *         perfectly matching the reference sequence
     *  \param record first read of the pair
     *  \return bool indicating if the read belongs to class P
     * */
    bool isClassP(BamAlignmentRecord& record);

    /** \brief To determine if the read belongs to class N, reads
     *         containing mismatches which are unknonwn bases only
     *  \param record first read from the pair
     *  \return bool indicating if the read belongs to class N
     * */
    bool isClassN(BamAlignmentRecord& record);

    /** \brief To determine if the read belongs to class M, reads contanining
     *         at least one substitution, optionally unknown bases and no
     *         insertion, deletions and clipped bases
     *  \param record first read from the pair
     *  \return bool indicating if the read belongs to class M
     * */
    bool isClassM(BamAlignmentRecord& record);

    /** \brief To determine if the read belongs to class I, reads contanining
     *         at least one insertion, deletion or clippled base, and
     *         optionally unknown bases or substitution
     *  \param record is the first read from the pair
     *  \return bool indicating if the read belongs to class I
     * */
    bool isClassI(BamAlignmentRecord& record);

    /** \brief To determine if the read belongs to class HM, reads where only
     *         one read is mapped
     *  \param record is the first read from the pair
     *  \return bool indicating if the read belongs to class HM
     * */
    bool isClassHM(BamAlignmentRecord& record);

    /** \brief To determine if the read belongs to class U, unmapped reads only
     *  \param record is the first read from the pair
     *  \return bool indicating if the read belongs to class U
     * */
    bool isClassU(BamAlignmentRecord& record);

    /** \brief To determine the data class where a read belongs to
     *  \param record is the first read from the pair
     *  \return integer indicating the type of data class of the read
     * */
    uint8_t getClassType(BamAlignmentRecord& record);

    /** \brief Gets the rcomp descriptor for the read
     *  \param record first read from the pair
     *  \return value of the rcomp descriptor of the read
     * */
    uint8_t getRcompDescriptor(BamAlignmentRecord& record);

    /** \brief Gets the flags descriptor for the read
     *  \param record is the first read from the pair
     *  \return value of the flags descriptor of the read
     * */
    uint8_t getFlagDescriptor(BamAlignmentRecord& record);

    /** \brief Gets the mmpos descriptor for the read
     *  \param record is the first read from the pair
     *  \return vector with all the mismatches of the read with the reference sequence
     * */
    std::vector<std::pair<uint16_t, std::string> > getmmposDescriptor(BamAlignmentRecord& record);

    /** \brief Gets the mmtype descriptor for the read using alphabet 0
     *  \param mmpos contains all the mismatches of the read
     *  \return vector containing the type of mismatches
     * */
    std::vector<uint8_t> getmmtypeDescriptor(std::vector<std::pair<uint16_t, std::string> >& mmpos);

    /** \brief Gets the rlen descriptor for the read
     *  \param record first read from the pair
     *  \return value of the rlen descriptor of the read
     * */
    uint8_t getRlenDescriptor(BamAlignmentRecord& record);

    /** \brief Gets the pair descriptor for the read
     *  \param record first read from the pair
     *  \return value of the pair descriptor of the read
     * */
    std::string getPairDescriptor(BamAlignmentRecord& record);


    uint16_t reads_distance(BamAlignmentRecord& record);

    /** \brief Extracts the value of the NM tag from the read
     *  \param record read to be treated
     *  \return integer containing the value of the NM tag
     * */
    int getNMtag(BamAlignmentRecord& record);

    /** \brief Extracts the value of the MD tag from the read
     *  \param record read to be treated
     *  \return string containing the value of the MD tag
     * */
    std::string getMDtag(BamAlignmentRecord& record);

    /** \brief Converts the read or paired read to the mpegg output format
     *  \param record first read from the pair
     *  \return void
     * */
    void convertToMpeggRecord(MpeggRecord& result, BamAlignmentRecord& record);

    /** \brief Convert a hexadecimal value stored in a string to little endian
     * \param value hexadecimal value
     * \return value but in little endian
     */
    std::string toLittleEndian_hex(std::string value);

    static uint64_t hex_to_int(std::string value);

    /** \brief A simple function that converts a value from a int to a hex
     *  \param value int value to be converted
     *  \return hexadecimal value converted
     * */
    std::string int32_to_hex(int32_t value);

    std::string int16_to_hex(int16_t value);

    /** \brief Removes the first read from the list of paired reads
     * \param void
     * \return void
     * */
    void removeFirstRead();

    /** \brief Gets all the paired reads
     * \param allReads will contain the value of the paired readss
     * \return void
     * */
    void getAllreads(std::multimap<int, std::pair<BamAlignmentRecord, BamAlignmentRecord> >& allReads);

    /** \brief Inserts a new access unit to the list of access units
     * \param au access unit of any kind
     * \return void
     * */
    void insertAccessUnit(AccessUnit au);

    /** \brief Gets all the access units stored
     * \param au will contain the value of all access units
     * \return void
     * */
    void getAllAccessUnits(std::vector<AccessUnit>& au);

    /** \brief Inserts a new read pair to the list of paired reads
     * \param first first read from the pair
     * \param second second read from the pair
     * \return void
     * */
    void insertRead(BamAlignmentRecord first, BamAlignmentRecord second);
};

#endif
