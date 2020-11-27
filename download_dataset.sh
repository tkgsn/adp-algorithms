mkdir raw_data
wget http://www.philippe-fournier-viger.com/spmf/datasets/kosarak_sequences.txt -P raw_data
wget http://www.kdd.org/cupfiles/KDDCup2000.zip -P raw_data
wget http://fimi.uantwerpen.be/data/T40I10D100K.dat -P raw_data
unzip raw_data/KDDCup2000.zip -d raw_data/bmspos

mkdir result
python data_process.py