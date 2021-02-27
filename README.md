# svtwgap

This uses the JSON library: https://github.com/nlohmann/json

The codes were implemnted by C++11 using boost library and Python 3.7.

# Dataset

kosarak: http://www.philippe-fournier-viger.com/spmf/index.php?link=datasets.php

(direct): http://www.philippe-fournier-viger.com/spmf/datasets/kosarak_sequences.txt

BMS-POS: https://www.kdd.org/kdd-cup/view/kdd-cup-2000

(direct): http://www.kdd.org/cupfiles/KDDCup2000.zip

T40I10D100K: http://fimi.uantwerpen.be/data/

(direct): http://fimi.uantwerpen.be/data/T40I10D100K.dat


# Run

```
./download_dataset.sh
python data_process.py
./run_rnm.sh
./run_svt.sh
```

You can show the figures in the paper by running plot.ipynb.
