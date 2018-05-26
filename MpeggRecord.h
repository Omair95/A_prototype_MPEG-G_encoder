#include <iostream>
#include <vector>

/**
 * @file MpeggRecord.h
 * @Author Omair Iqbal
 * @date 04/2018
 * @struct MpeggRecord
 * @brief Format of the output mpegg record
 * @contact Omair95@protonmail.com
*/

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_MPEGG_RECORD_H
#define A_PROTOTYPE_MPEG_G_ENCODER_MPEGG_RECORD_H

struct MpeggRecord {
    uint64_t global_Id;                                       /// unique 64-bit unsigment identifier
    std::string read_name;                                    /// read identifier
    uint8_t class_type;                                       /// class type of the read
    uint16_t seq_Id;                                          /// sequence identifier of the read
    uint8_t read1_first;                                      /// flag signaling if read 1 is the left most mapped read in the genomic record
    uint8_t flags;                                            /// unsigned integer obtained by setting the flags
    uint8_t number_of_segments;                               /// numbers of segments of the first read
    uint16_t number_of_alignments;                            /// number of alignments of the first read

    std::vector<uint32_t> read_len;                           /// represents the length of each read in the genomic record expressed as number of nucleoids
    std::vector<std::string> quality_values;                  ///  Quality Values associated to each nucleotide contained in the sequence vectors
    std::vector<std::string> sequence;                        /// ASCII representation of the nucleotides

    std::vector<uint32_t> mapping_pos;                        /// mapping position of each aligned segment in the Genomic Record as 0-based coordinate from the reference sequence
    std::vector < std::vector<uint16_t> > cigar_size;         /// size in characters of the cigar_size string
    std::vector < std::vector<std::string> > ecigar_string;   /// the extended cigar of the read
    std::vector < std::vector<uint8_t> > reverse_comp;        /// flag associated to each alignmnent in the Genomic Record signaling if the alignment is on forward or reverse strand
};

#endif //A_PROTOTYPE_MPEG_G_ENCODER_MPEGG_RECORD_H