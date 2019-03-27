# Download datasets

## Disk space required

The files required by the speedup analysis will be stored in the folder:

```
dataset                     [~ 133.6M]
 └── speedup_analysis_files [~ 133.6M]
```

or, if you want to download a larger dataset

```
dataset                  [~ 3.6 GB]
 ├── human_genome        [~ 3.2 GB]
 ├── large_english_text  [~ 77 MB]
 └── large_source_code   [~ 325 MB]
```

## Automatic datasets download

Make the required scripts executable

```bash
$ chmod +x ./script/*
```

and download the files for speedup analysis by

```bash
$ ./script/download_files.sh
```

If instead you prefer to download all datasets

```bash
$ ./script/download_all.sh
```

---

Optionally, to download the datasets individually

```bash
$ ./script/download_human_genome.sh

$ ./script/download_large_english_text.sh

$ ./script/download_large_source_code.sh
```
---

## Remarks on skip download and skip unzipping

The download scripts are designed to skip the download of any resource (file and directory) that is already present on disk.

To force the download/unzip of a resource: delete the already present one and run the script again.
