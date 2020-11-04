import pathlib
dataset_dir = pathlib.Path("dataset")
dataset_dir.mkdir(exist_ok=True)

kosarak_dir = dataset_dir / "kosarak"
kosarak_dir.mkdir(exist_ok=True)

bmspos_dir = dataset_dir / "bms_pos"
bmspos_dir.mkdir(exist_ok=True)

t40_dir = dataset_dir / "t40"
t40_dir.mkdir(exist_ok=True)

data_name = "data.txt"
with open("raw_data/kosarak_sequences.txt") as f:
    with open(kosarak_dir / data_name, "w") as fw:
        for line in f:
            line = " ".join(set([num for num in line.split() if not num.startswith("-")]))
            fw.write(line + "\n")

with open("raw_data/bmspos/assoc/BMS_ASSOC_DATA/BMS-POS.dat") as f:
    with open(bmspos_dir / data_name, "w") as fw:
        pre_line = f.readline().split()
        pre = pre_line[0]
        accum_line = [pre_line[1]]
        for line in f:
            line = line.split()
            if pre != line[0]:
                pre = line[0]
                fw.write(" ".join(accum_line) + "\n")
                accum_line = [line[1]]
            else:
                accum_line.append(line[1])
        accum_line.append(line[1])
        
with open("raw_data/T40I10D100K.dat") as f:
    with open(t40_dir / data_name, "w") as fw:
        for line in f:
            fw.write(line)