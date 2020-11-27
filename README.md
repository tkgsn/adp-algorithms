# svtwgap

This uses the JSON library: https://github.com/nlohmann/json

# Dataset

kosarak: http://www.philippe-fournier-viger.com/spmf/index.php?link=datasets.php

(direct): http://www.philippe-fournier-viger.com/spmf/datasets/kosarak_sequences.txt

BMS-POS: https://www.kdd.org/kdd-cup/view/kdd-cup-2000

(direct): http://www.kdd.org/cupfiles/KDDCup2000.zip

T40I10D100K: http://fimi.uantwerpen.be/data/

(direct): http://fimi.uantwerpen.be/data/T40I10D100K.dat


# Install

`pip install pathlib`

# How to UnitTest

This uses googletest for UnitTest. Googletest requires cmake to install it.

```
git clone https://github.com/google/googletest.git
cd googletest
mkdir build
cd build
cmake ..
make
```
