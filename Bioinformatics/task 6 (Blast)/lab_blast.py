from Bio.Blast import NCBIWWW
from Bio import SeqIO

sequence_data = open("blast_example.fasta").read() 
result_handle = NCBIWWW.qblast("blastn", "nt", sequence_data) 

seq_record = next(SeqIO.parse(open('blast_example.fasta'),'fasta')) 
result_handle = NCBIWWW.qblast("blastn", "nt", seq_record.seq) 

with open('results.xml', 'w') as save_file: 
    blast_results = result_handle.read() 
    save_file.write(blast_results)