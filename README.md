In this repository, different versions of EPOS file reader ```fileReader.C```, with accompanied files, are maintained.
To get particular version, simply do:

```bash
# clone locally this repository:
git clone https://github.com/abilandz/epos.git epos
cd epos

# check the available tags:
git tag

# checkout the desired version of fileReader.C by specifying corresponding tag, e.g.:
git checkout v2.0.0
```

Currently available versions of ```fileReader.C```:

* **main** &mdash; This is the latest and greatest version, available in the "main" branch. All latest improvements and bug fixes are available in this version. 
Tested successfully also for the latest EPOS 4.0.3 productions.

* **v2.0.0** &mdash; Developed for large-scale EPOS 3.117 and EPOS 4.0.0 productions as of 2022, using ROOT 6. An ASCII file with the list of ROOT files containing EPOS events has to be provided.
This macro then simply processes line-by-line that ASCII file. A list of ROOT files can be prepared by using 'find' command. The ROOT files on that list can be on 
different /scratch disks. See the preamble of ```fileReader.C``` for further details (how to compile, run, extract EPOS variables, etc.)

* **v1.0.0** &mdash; Legacy file reader, used for all local EPOS 3.117 productions in the period 2016-2021. Tested only under ROOT 5. For the specified directory, 
it searches for all ROOT files inside containing EPOS events. It is not recommended to use this macro for new EPOS productions starting with 2022, use instead 
**v2.0.0**. See the preamble of ```fileReader.C``` for further details (how to compile, run, extract EPOS variables, etc.)  

