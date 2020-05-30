# A Brief Guide to the `rexec` Utility

The `rexec` utility is still on its early development stage and is far from stable. The usage guide is also a work in progress.

Besides, we would make the `rexec` utility project open soon.

## Preliminaries

### Docker Installation

- Windows (other than home editions): [Docker Desktop for Windows](https://hub.docker.com/editions/community/docker-ce-desktop-windows)
- macOS: [Docker Desktop for Mac](https://hub.docker.com/editions/community/docker-ce-desktop-mac)
- Linux: [Docker CE](https://mirror.tuna.tsinghua.edu.cn/help/docker-ce/)

### Pull the Image for Local Compilation

This image contains the full CUDA 10.2 toolkit and so is quite large (~4GB). It might take you some time to pull all layers.

```bash
docker pull registry.cn-shenzhen.aliyuncs.com/multicoresysu2020/cuda-cpu-dev:v0.1.6
```

### Download the Utility

- [Latest Windows release](../rexec/rexec.exe)
- [Latest macOS release](../rexec/rexec.macos)
- [Latest Linux release](../rexec/rexec)

### Create Your Credential

Most of the functionalities will require your credential. It is a good idea to persist your credential in a configuration file first. `rexec` reads in a configuration file with the `.toml` format, and the default path it looks for is `./config.toml` (relative to the work directory).

In the current version, you should provide your `sn` (student number) and `pass` (password) in the configuration file and can specify the project `home` directory for convenience.

```yaml
sn = "{student number}"
pass = "{password}"

# The project `home` must be relative to the work directory:
#   the program will join the `home` variable as follows: $(pwd)/$home.
# All source code and resource files should be gathered in the project `home`:
#   resources outside the folder through linking are not supported.
# Be default (when `home` is an empty string), the work directory will be
#   treated as the home directly.
# Besides, you can specify the `home` through command line arguments, e.g.,
#   `--home vecadd` asks the program to look into $(pwd)/vecadd.
home = ""
```

With all these preliminaries satisfied, you can go with the functionalities `rexec` provides.

## Get the Help Message

### Command

```bash
./rexec.macos --help
```

### Example Output

```bash
Usage of ./rexec.macos:
      --co               compile only
  -c, --config string    configuration file (default ./config.toml)
      --devices string   specify the CUDA devices to use (examples: 0|1|2|3|1,2|1-2)
      --force            force to re-run
      --home string      project home relative to the work dir
      --nvidia           run nvidia-smi
      --tag string       executable tag to query
pflag: help requested
```

## Query the Remote Devices Statuses

### Command

Occasionally, some GPU devices can be busy and all their graphical memory spaces are taken. In these cases, you might not be able to run your program successfully. To this concern, you can query devices' statuses first and then decide the most suitable one you want to use by specifying the `--device` option.

```bash
~ ./rexec.macos --nvidia
```

### Example Output

```bash
INFO[0000] Sat May 30 11:07:38 2020
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 440.33.01    Driver Version: 440.33.01    CUDA Version: 10.2     |
|-------------------------------+----------------------+----------------------+
| GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|===============================+======================+======================|
|   0  Tesla V100-SXM2...  Off  | 00000000:1A:00.0 Off |                    0 |
| N/A   49C    P0    57W / 300W |  32396MiB / 32510MiB |      0%      Default |
+-------------------------------+----------------------+----------------------+
|   1  Tesla V100-SXM2...  Off  | 00000000:3D:00.0 Off |                    0 |
| N/A   48C    P0    58W / 300W |  31041MiB / 32510MiB |      0%      Default |
+-------------------------------+----------------------+----------------------+
|   2  Tesla V100-SXM2...  Off  | 00000000:89:00.0 Off |                    0 |
| N/A   48C    P0    59W / 300W |   5708MiB / 32510MiB |      0%      Default |
+-------------------------------+----------------------+----------------------+
|   3  Tesla V100-SXM2...  Off  | 00000000:B2:00.0 Off |                    0 |
| N/A   61C    P0   107W / 300W |   2181MiB / 32510MiB |     69%      Default |
+-------------------------------+----------------------+----------------------+

+-----------------------------------------------------------------------------+
| Processes:                                                       GPU Memory |
|  GPU       PID   Type   Process name                             Usage      |
|=============================================================================|
+-----------------------------------------------------------------------------+
```

## Compile a CUDA Program Locally (CUDA GPUs are not required!)

### Command

Compile-only mode could be useful when you only want to confirm that your project can be compiled without errors.

```bash
~ ./rexec.macos --co --home vecadd
```

### Example Output

```bash
INFO[0014] [run -t --rm -v $(pwd)/vecadd:/mnt -e GROUP_ID=20 -e USER_ID=501 registry.cn-shenzhen.aliyuncs.com/multicoresysu2020/cuda-cpu-dev:v0.1.6 mkdir -p /mnt/build && cd /mnt/build && cmake .. && make]
[ mkdir -p /mnt/build && cd /mnt/build && cmake .. && make ]
addgroup: The GID `20' is already in use.
Adding user `nameless' ...
Adding new user `nameless' (501) with group `dialout' ...
Creating home directory `/home/nameless' ...
Copying files from `/etc/skel' ...
-- The C compiler identification is GNU 7.5.0
-- The CXX compiler identification is GNU 7.5.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Looking for pthread_create
-- Looking for pthread_create - not found
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/build
[ 50%] Building NVCC (Device) object CMakeFiles/main.dir/src/main_generated_main.cu.o
Scanning dependencies of target main
[100%] Linking CXX executable main
 100%] Built target main
```

## Compile a CUDA Program Locally and Offload it to the Remote Server

### Command

This is the full mode --- with which `rexec` compiles the project for you, offloads the executable to the remote server on behalf of you, and queries the corresponding result. Note that it is possible you will have to wait for some time before querying the result back (depending on the buffer size), but it is safe to cancel the querying process. You can query the result manually some time later.

{{% notice note %}}
CAVEAT: currently `rexec` only supports offloading a single-filed executable (i.e., your executable should not rely on extra resources). Besides, the executable must be `main` (we don't see any marginal benefits from allowing you to specify the executable name in the current version).
{{% /notice %}}

```bash
~ ./rexec.macos --home vecadd
```

### Example Output

```bash
INFO[0016] [run -t --rm -v $(pwd)/vecadd:/mnt -e GROUP_ID=20 -e USER_ID=501 registry.cn-shenzhen.aliyuncs.com/multicoresysu2020/cuda-cpu-dev:v0.1.6 mkdir -p /mnt/build && cd /mnt/build && cmake .. && make]
[ mkdir -p /mnt/build && cd /mnt/build && cmake .. && make ]
addgroup: The GID `20' is already in use.
Adding user `nameless' ...
Adding new user `nameless' (501) with group `dialout' ...
Creating home directory `/home/nameless' ...
Copying files from `/etc/skel' ...
-- The C compiler identification is GNU 7.5.0
-- The CXX compiler identification is GNU 7.5.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Looking for pthread_create
-- Looking for pthread_create - not found
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/build
[ 50%] Building NVCC (Device) object CMakeFiles/main.dir/src/main_generated_main.cu.o
Scanning dependencies of target main
[100%] Linking CXX executable main
 100%] Built target main
INFO[0016] Offloaded successfully (a10120e08db55e989711ffd83a4be3dc).
INFO[0016] Press <Ctrl>+<C> to stop querying.
INFO[0017] v0: 0
v1: 2
v2: 4
v3: 6
v4: 8
v5: 10
v6: 12
v7: 14
```

## Query the Output of a Previously Offloaded CUDA Program

### Command

When you offloads an executable successfully, you would get a tag of the executable (e.g., `a10120e08db55e989711ffd83a4be3dc` in the previous section). You can query the output of it manually by specifying the `--tag` option.

```bash
~ ./rexec.macos --tag a10120e08db55e989711ffd83a4be3dc
```

### Example Output

```bash
INFO[0000] Press <Ctrl>+<C> to stop querying.
INFO[0000] v0: 0
v1: 2
v2: 4
v3: 6
v4: 8
v5: 10
v6: 12
v7: 14
```

## Force to Re-run Your Executable

### Command

By default, the remote server enforces a policy that if an executable has been offloaded and run before, it would not be executed for a second time but instead, the previous output will be returned.

However, as we mentioned earlier, there are chances that your CUDA programs can fail owing to the lack of memory spaces. In these cases, you may specify the `--force` option to force the server to re-run your executable. And to avoid memory space shortage again, you'd better specify the device to use by using the `--devices` option.

```bash
~ ./rexec.macos --home vecadd --force --devices 2
```

## Other Resources

- You can find the example project `vecadd` [here](https://github.com/lukeluocn/multicoresysu2020/tree/master/content/rexec/vecadd).