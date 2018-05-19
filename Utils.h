#include "AccessUnit.h"
#include "MpeggRecord.h"
#include <sstream>
#include <ctype.h>

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
    static std::string getCigar(String <CigarElement<> >& rCigar);

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


    static uint16_t reads_distance(BamAlignmentRecord& record);

    /** \brief Extracts the value of the NM tag from the read
     *  \param record read to be treated
     *  \return integer containing the value of the NM tag
     * */
    int getNMtag(BamAlignmentRecord& record);

    /** \brief Extracts the value of the MD tag from the read
     *  \param record read to be treated
     *  \return string containing the value of the MD tag
     * */
    static std::string getMDtag(BamAlignmentRecord& record);

    /** \brief Converts the read or paired read to the mpegg output format
     *  \param record first read from the pair
     *  \return void
     * */
    void convertToMpeggRecord(MpeggRecord& result, BamAlignmentRecord& record);


    /** \brief Writes the mmpos descriptor value to the respective file
     *         according to the class type
     *  \param value value of the mmpos descriptor
     *  \param classType class type
     *   \return void
     * */
    std::vector<std::pair<uint16_t, std::string> > getMmposValues(BamAlignmentRecord& record);

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

    static bool isPaired(BamAlignmentRecord first, BamAlignmentRecord second);

    static uint8_t getSequenceLength(BamAlignmentRecord& record);
};

#endif
