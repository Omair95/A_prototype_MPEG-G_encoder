file1 = open("../Files/9827_2#49.mpegg.ircomp", "rb");
file2 = open("../../TestFiles/9827_2#49.bam.grcomp", "rb");

file1.seek(0);
file2.seek(3661);
count = 0;
couple_bytes_file1 = file1.read(1).encode("hex");
couple_bytes_file2 = file2.read(1).encode("hex");
while couple_bytes_file1 != "":
	if couple_bytes_file1 != couple_bytes_file2:
		print(str(count) + " file1: " + couple_bytes_file1 + " file2: " + couple_bytes_file2);

	couple_bytes_file1 = file1.read(1).encode("hex");
	couple_bytes_file2 = file2.read(1).encode("hex");
	count = count + 1;
