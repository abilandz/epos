In this repository, different versions of EPOS file readers are maintained.
To get particular version, simply do:

```bash
# clone locally this repository:
git clone https://github.com/abilandz/epos.git epos
cd epos

# check the available tags:
git tag

# checkout the desired version by specifying corresponding tag, e.g.:
git checkout v2.0.0
```

Currently available versions:

* **v2.0.0** &mdash; Developed for large-scale EPOS 3.117 productions as of 2022. An ASCII file with the list of ROOT files containing EPOS readers has to be provided.
This macro then simply processes line-by-line that ASCII file. A list of ROOT files can be prepared by using 'find' command. The ROOT files on that list can be on 
different /scratch disks.

* **v1.0.0** &mdash; legacy file reader, used for all local EPOS 3.117 productions in the period 2016-2021. For the specified directory, 
it searches for all ROOT files containing EPOS events. Can be used also for new EPOS 3.117 productions as of 2022, but it is not recommended, use instead 
v2.0.0, or later.  

