# Tool : dudumper

simple disk usage dumper

# Goal

The goal of the tool is to parse a directory like `du` would, and dump the size 
of every file and folder, either in json or sqlite.

Because it's intended to be used on very big trees, it tries to be as fast as possible, 
still not storing everything in memory.

# Usage

```
Usage: dudumper [OPTION]...
Disk usage dumper.
Dump disk usage into files that can used latter to browse,
display, query disk usage.

  --dir DIRNAME          the directory to parse
  --out FILENAME         the base filename to parse.
                         extension will be appened based
                         on file type
  --display-dir DIRNAME  the directory to display instead of --dir
  --depth DEPTH          the depth to output in the dump
  --sqlite               dump sqlite script
  --json                 dump json sctructure
  --html                 dump html page
  --cddaf[-]             consider dot dir as file (with - to disable). (Eg: .svn, .git, .hg, etc.)
  --win                  display dir as windows dir (replace / by \)
  --help                 display this help and exit
```

# Example

```
./dudumper --out z-output --dir . --depth 4 --sqlite
```

will produce a file `z-output.sql` with the content of all files in the `.` directory, 
and will only write fields with depth <= 4.

# Usage with docker

Only work with x86_64, unless you replace the image name by an image you have built yourself.

Support `/path/to/inputfolder` and `/path/to/outputfolder` are existing folders.

```
docker run -ti -v "/path/to/inputfolder:/data:ro" -v "/path/to/outputfolder:/tmp:rw" gissehel/dudumper --dir /data --out /tmp/inputdescription --html
```

will produce a file `/path/to/outputfolder/inputdescription.html` containing the structure of `/path/to/inputfolder`.

## Example

Requirements:
* Linux x86_64
* Docker installed and working
* Possibility to add bash functions to your environnement

Add the following function to your environnement:

```
function get-path-map {
    INPUT="$1"
    OUTPUT="$2"
    [ -z "${INPUT}" ] && INPUT="."
    input_path="$(readlink -f "${INPUT}")"
    if [ -z "${OUTPUT}" ]
    then
        output_path="/tmp"
        output_file="$(echo "${input_path}" | sed -e 's/\//-/g; s/^-/dudumper-/')"
    else
        output_path="$(dirname "$(readlink -f "${OUTPUT}")")"
        output_file="$(basename "${OUTPUT}")"
    fi
    user="$(id -u):$(id -g)"
    echo docker run -ti --user "${user}" -v "${input_path}:/data:ro" -v "${output_path}:/tmp:rw" gissehel/dudumper --dir /data --out /tmp/"${output_file}" --html --display-dir "${input_path}"
    docker run -ti --user "${user}" -v "${input_path}:/data:ro" -v "${output_path}:/tmp:rw" gissehel/dudumper --dir /data --out /tmp/"${output_file}" --html --display-dir "${input_path}"
    echo "${output_path}/${output_file}.html"
}sss
```

Usage: `get-path-map [INPUT_FOLDER] [OUTPUT_FILE_BASENAME]`

# References

* Project page: **https://github.com/gissehel/dudumper**
* Docker hub project page : **https://hub.docker.com/repository/docker/gissehel/dudumper**
* Image on docker hub: **gissehel/dudumper**
* Image on github registry: **ghcr.io/gissehel/dudumper**

