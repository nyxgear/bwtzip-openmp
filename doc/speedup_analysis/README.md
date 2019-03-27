# Speedup analysis of pbwtzip and pbwtzip2

- [pbwtzip_speedup_analysis.ipynb](./pbwtzip_speedup_analysis.ipynb)
- [pbwtzip2_speedup_analysis.ipynb](./pbwtzip2_speedup_analysis.ipynb)


## Run the notebooks

1.  Install Python virtualenv

```bash
$ pip install virtualenv
```

2. Create a new virtual env inside the doc/speedup_analysis folder

```bash
$ cd doc/speedup_analysis
$ python3 -m venv ./venv
```

3.  Install Python dependencies

```bash
# activate virtual env
$ source venv/bin/activate

# first upgrade pip
(venv) $ pip install --upgrade pip

(venv) $ pip install --upgrade -r requirements.txt

# deactivate virtual env
$ deactivate
```


4. Activate virtualenv and run Jupiter Notebook

```bash
$ source venv/bin/activate

(venv) $ jupyter notebook .
```

A new page on your browser should open with Jupyter running.
