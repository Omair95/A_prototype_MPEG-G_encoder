#include <iostream>
#include <vector>

#ifndef A_PROTOTYPE_MPEG_G_ENCODER_MPEGG_RECORD_H
#define A_PROTOTYPE_MPEG_G_ENCODER_MPEGG_RECORD_H

class MpeggRecord {

private:
    uint64_t global_Id;                          // unique 64-bit unsigment identifier
    std::string read_name;                       // read identifier
    uint8_t class_type;                          // class type of the read
    uint16_t seq_Id;                             // sequence identifier of the read
    uint8_t read1_first;                         // flag signaling if read 1 is the left most mapped read in the genomic record
    uint8_t flags;                               // unsigned integer obtained by setting the flags
    uint8_t number_of_segments;                  // numbers of segments of the first read
    uint16_t number_of_alignments;               // number of alignments of the first read

    std::vector<uint32_t> read_len;              // represents the length of each read in the genomic record expressed as number of nucleoids
    std::vector<std::string> quality_values;     //  Quality Values associated to each nucleotide contained in the sequence vectors
    std::vector<std::string> sequence;           // ASCII representation of the nucleotides

    std::vector<uint32_t> mapping_pos;
    std::vector < std::vector<uint16_t> > cigar_size;
    std::vector < std::vector<std::string> > ecigar_string;
    std::vector < std::vector<uint8_t> > reverse_comp;

    uint8_t split_aligments;

public:

    MpeggRecord() { }

};


#endif //A_PROTOTYPE_MPEG_G_ENCODER_MPEGG_RECORD_H
