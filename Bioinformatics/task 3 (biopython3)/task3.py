from Bio.SeqRecord import SeqRecord
from Bio.SeqIO import parse
from Bio.SeqIO import write

file = open('./data/orchid.fasta')
for record in parse(file, "fasta"):
    print(record.id)

first_seq_record = next(parse(open("orchid.fasta"), 'fasta'))

first_seq_record.id
first_seq_record.name
first_seq_record.seq
first_seq_record.description
first_seq_record.annotations

seq_iter = parse(open('orchid.fasta'), 'fasta')
all_seq = [s for s in seq_iter]
len(all_seq)

seq_iter = parse(open('orchid.fasta'), 'fasta')
max_seq = max(len(s) for s in seq_iter)
max_seq

seq_iter = parse(open('orchid.fasta'), 'fasta')
seq_under_600 = [s for s in seq_iter if len(s.seq) < 600]
for s in seq_under_600:
    print(s.id)

file = open("converted.fasta", 'w')
write(seq_under_600, file, 'fasta')
file.close()