# Tool : dudumper

simple disk usage dumper

# Goal

The goal of the tool is to parse a directory like `du` would, and dump the size 
of every file and folder, either in json or sqlite.

Because it's intended to be used on very big trees, it tries to be as fast as possible, 
still not storing everything in memory.

# Usage

as `--help` is not supported right now, the usage is on this page only.

```
dudumper [--out FILENAME] [--dir .] [--depth MAX_DEPTH_TO_OUTPUT] (--json|--sqlite)
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

# References

* Project page: https://github.com/gissehel/dudumper
* Docker hub project page : https://hub.docker.com/repository/docker/gissehel/dudumper

